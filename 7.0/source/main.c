#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

pthread_rwlock_t rwlocker;
char arrCounter[] = {'0', '\0'};

void* writer() {
	while (1) {
		pthread_rwlock_wrlock(&rwlocker);
    printf("[writer] make counter +1\n");
		arrCounter[0] += 1;
		sleep(1);
		pthread_rwlock_unlock(&rwlocker);
		sleep(1);
	}
	pthread_exit(0);
}

void* reader() {
	while (1) {
		pthread_rwlock_rdlock(&rwlocker);
		printf("[reader] %lx reading array: %s\n", pthread_self(), arrCounter);
		sleep(1);		
		pthread_rwlock_unlock(&rwlocker);
		sleep(1);
	}

	pthread_exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	pthread_rwlock_init(&rwlocker, NULL);
	int threadsCount = 11;

	pthread_t thread[threadsCount];
	pthread_create(&thread[0], NULL, writer, NULL);
	for (int i = 1; i < threadsCount; ++i) {
		pthread_create(&thread[i], NULL, reader, NULL);
	}

	for (int i = 0; i < threadsCount; ++i) {
		pthread_join(thread[i], NULL);
	}

	pthread_rwlock_destroy(&rwlocker);
	return 0;
}