all: shared_memory shm_with_sem producer_consumer
shared_memory:
	gcc -Wall ../ipc_shared_memory.c test_shared_memory.c -o test_shared_memory -I.. -I../../common
shm_with_sem:
	gcc -Wall ../ipc_shared_memory.c  ../ipc_semaphore.c test_shm_with_sem.c -o test_shm_with_sem -I.. -I../../common
producer_consumer:
	gcc -Wall ../ipc_shared_memory.c  ../ipc_semaphore.c test_producer_consumer.c -o test_producer_consumer -I.. -I../../common
clean:
	rm -f *.o test_shared_memory test_shm_with_sem test_producer_consumer