#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

pthread_mutex_t mtx;
char arrCounter[] = {'0', '\0'};

void* writer() {
	while (1) {
		pthread_mutex_lock(&mtx);
    		printf("[writer] make counter +1\n");
		arrCounter[0] += 1;
		sleep(1);
		pthread_mutex_unlock(&mtx);
		sleep(5);
	}

	pthread_exit(0);
}

void* reader() {
	while (1) {
		pthread_mutex_lock(&mtx);
		printf("[reader] %lx reading array: %s\n", pthread_self(), arrCounter);
		sleep(1);
		pthread_mutex_unlock(&mtx);
		sleep(5);
	}

	pthread_exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	pthread_mutex_init(&mtx, NULL);
	int threadsCount = 11;

	pthread_t thread[threadsCount];
	pthread_create(&thread[0], NULL, writer, NULL);
	for (int i = 1; i < threadsCount; ++i) {
		pthread_create(&thread[i], NULL, reader, NULL);
	}

	for (int i = 0; i < threadsCount; ++i) {
		pthread_join(thread[i], NULL);
	}

	pthread_mutex_destroy(&mtx);
	return 0;
}
