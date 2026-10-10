CC := gcc

thread_exit_test : thread_exit_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

thread_cancel_test : thread_cancel_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

mutex_test : mutex_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

rw_lock_test : rw_lock_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

rw_lock_hungry_test : rw_lock_hungry_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

cond_test : cond_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

sem_test : sem_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@


unname_sem_process_test : unname_sem_process_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

name_sem_process_test : name_sem_process_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

thread_pool_test : thread_pool_test.c
	$(CC) $^ -o $@
	./$@
	rm -f $@