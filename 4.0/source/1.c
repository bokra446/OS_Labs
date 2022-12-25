#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

bool flag = true;
const char* shmName = "shmem_file";
size_t shmSize = 64;
char* shm_ptr;
int shmid;
int semid;

void handler(){
	int save_errno = errno;
	errno = save_errno;
  flag = false;
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

  signal(SIGINT, handler);
	signal(SIGTERM, handler);

	struct sembuf setSem = {0, 1, 0};
	struct stat buff;
	FILE* file;
	
	if (stat(shmName, &buff) != 0) {
		if (!(file = fopen(shmName, "w"))) {
			fprintf(stderr, "Shared memory file cannot be created: %s(%d)\n", strerror(errno), errno);
			exit(1);
		}
		fclose(file);
	}
	
	key_t shm_key = ftok(shmName, 1);
	shmid = shmget(shm_key, shmSize, IPC_CREAT | IPC_EXCL | 0666);
	if (shmid == -1 && errno == EEXIST) {
		fprintf(stderr, "The is running: %s(%d)", strerror(errno), errno);
		exit(1);
	}
	if (shmid == -1) {
		fprintf(stderr, "%s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	
	int semid = semget(shm_key, 1, IPC_CREAT | 0666);
	if (semid == -1) {
		fprintf(stderr, "semget failed: %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	
	printf("[first] key: %d\n[first] mem_id: %d\n[first] sem_id: %d\n", shm_key, shmid, semid);

	shm_ptr = shmat(shmid, NULL, 0);
	printf("[first] shm_ptr: %p\n", shm_ptr);

	semctl(semid, 0, SETVAL, (int)0); // init semaphore value with 0
	
	while(flag) {
		struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char str[1024];
		sprintf(str, "[PARENT] {%.2d:%.2d:%.2d} pid = %d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, getpid());
		strcpy(shm_ptr, str);
		int res = semop(semid, &setSem, 1);
		if (res == -1) {
			int err = errno;
			printf("[first] semop: %s(%d)\n", strerror(err), err);
		}
		sleep(5);
	}
  shmdt(shm_ptr);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 0, IPC_RMID);

	printf("[first] Destroying the shared memory segment and semaphore\n");
	remove(shmName);	
	return 0;
}