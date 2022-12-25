#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

pthread_mutex_t mutex;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

char arrCounter[] = {'0', '\0'};

void* writer() {
	while (1) {
		pthread_mutex_lock(&mutex);
		arrCounter[0] += 1;
		pthread_cond_broadcast(&condition);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	pthread_exit(0);
}

void* reader() {
	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&condition, &mutex);
		printf("[reader] %lx reading array: %s\n", pthread_self(), arrCounter);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	pthread_exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	pthread_mutex_init(&mutex, NULL);
	int threadsCount = 11;

	pthread_t thread[threadsCount];
	for (int i = 0; i < threadsCount - 1; ++i) {
		pthread_create(&thread[i], NULL, reader, NULL);
	}
  pthread_create(&thread[10], NULL, writer, NULL);

	for (int i = 0; i < threadsCount; ++i) {
		pthread_join(thread[i], NULL);
	}

	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&mutex);
	return 0;
}