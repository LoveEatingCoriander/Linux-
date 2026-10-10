#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <linux/videodev2.h>

struct buffer {
    void *start;
    size_t length;
};

static int xioctl(int fd, int request, void *arg) {
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while (r == -1 && errno == EINTR);
    return r;
}

static int clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return v;
}

/* Convert YUYV to RGB and save as PPM */
static void save_ppm(const char *filename, const unsigned char *data,
                     unsigned int width, unsigned int height,
                     unsigned int bytesperline) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen ppm");
        return;
    }

    fprintf(fp, "P6\n%u %u\n255\n", width, height);

    for (unsigned int y = 0; y < height; y++) {
        const unsigned char *row = data + y * bytesperline;

        for (unsigned int x = 0; x < width; x += 2) {
            int y0 = row[0];
            int u  = row[1];
            int y1 = row[2];
            int v  = row[3];
            row += 4;

            int c = y0 - 16;
            int d = u - 128;
            int e = v - 128;

            int r = clamp((298 * c + 409 * e + 128) >> 8);
            int g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
            int b = clamp((298 * c + 516 * d + 128) >> 8);

            fputc(r, fp);
            fputc(g, fp);
            fputc(b, fp);

            if (x + 1 < width) {
                c = y1 - 16;
                r = clamp((298 * c + 409 * e + 128) >> 8);
                g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
                b = clamp((298 * c + 516 * d + 128) >> 8);

                fputc(r, fp);
                fputc(g, fp);
                fputc(b, fp);
            }
        }
    }

    fclose(fp);
}

int main(int argc, char **argv) {
    const char *dev_name = argc > 1 ? argv[1] : "/dev/video0";

    int fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if (fd == -1) {
        perror("open camera device");
        return 1;
    }

    struct v4l2_capability cap;
    memset(&cap, 0, sizeof(cap));

    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("VIDIOC_QUERYCAP");
        close(fd);
        return 1;
    }

    unsigned int caps = cap.capabilities;
    if (caps & V4L2_CAP_DEVICE_CAPS)
        caps = cap.device_caps;

    if (!(caps & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "%s is not a video capture device\n", dev_name);
        close(fd);
        return 1;
    }

    if (!(caps & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "%s does not support streaming\n", dev_name);
        close(fd);
        return 1;
    }

    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 640;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (xioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("VIDIOC_S_FMT");
        close(fd);
        return 1;
    }

    if (fmt.fmt.pix.bytesperline == 0)
        fmt.fmt.pix.bytesperline = fmt.fmt.pix.width * 2;

    printf("Actual format: %c%c%c%c, %ux%u, bytesperline=%u\n",
           fmt.fmt.pix.pixelformat & 0xff,
           (fmt.fmt.pix.pixelformat >> 8) & 0xff,
           (fmt.fmt.pix.pixelformat >> 16) & 0xff,
           (fmt.fmt.pix.pixelformat >> 24) & 0xff,
           fmt.fmt.pix.width,
           fmt.fmt.pix.height,
           fmt.fmt.pix.bytesperline);

    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV) {
        fprintf(stderr, "Warning: device is not in YUYV format. PPM output may be incorrect.\n");
    }

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("VIDIOC_REQBUFS");
        close(fd);
        return 1;
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory\n");
        close(fd);
        return 1;
    }

    struct buffer *buffers = calloc(req.count, sizeof(*buffers));
    if (!buffers) {
        perror("calloc");
        close(fd);
        return 1;
    }

    for (unsigned int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
            perror("VIDIOC_QUERYBUF");
            close(fd);
            return 1;
        }

        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, buf.m.offset);

        if (buffers[i].start == MAP_FAILED) {
            perror("mmap");
            close(fd);
            return 1;
        }
    }

    for (unsigned int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("VIDIOC_QBUF");
            close(fd);
            return 1;
        }
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        perror("VIDIOC_STREAMON");
        close(fd);
        return 1;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    int r = select(fd + 1, &fds, NULL, NULL, &tv);
    if (r == -1) {
        perror("select");
        close(fd);
        return 1;
    }
    if (r == 0) {
        fprintf(stderr, "Timeout waiting for camera data\n");
        close(fd);
        return 1;
    }

    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        perror("VIDIOC_DQBUF");
        close(fd);
        return 1;
    }

    printf("Captured one frame: %u bytes\n", buf.bytesused);

    FILE *fp = fopen("frame.yuyv", "wb");
    if (fp) {
        fwrite(buffers[buf.index].start, 1, buf.bytesused, fp);
        fclose(fp);
        printf("Raw frame saved to frame.yuyv\n");
    }

    save_ppm("frame.ppm", buffers[buf.index].start,
             fmt.fmt.pix.width, fmt.fmt.pix.height,
             fmt.fmt.pix.bytesperline);

    printf("PPM saved to frame.ppm\n");

    xioctl(fd, VIDIOC_QBUF, &buf);

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMOFF, &type);

    for (unsigned int i = 0; i < req.count; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }

    free(buffers);
    close(fd);
    return 0;
}