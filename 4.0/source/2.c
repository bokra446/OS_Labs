#include <stdio.h>
#include <stdlib.h>
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

const char* shmName = "shmem_file";
size_t shmSize = 64;
char* shm_ptr;
int shmid;


int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	struct sembuf unsetSem = {0, -1, 0};
	struct stat buff;

	if (stat(shmName, &buff) != 0) {
		fprintf(stderr, "Cannot find shared memory file. Run prog1.\n");
		exit(1);
	}
		
	key_t shm_key = ftok(shmName, 1);

	shm_ptr = shmat(shmid, NULL, SHM_RDONLY);
	printf("[second] shm_ptr: %p\n", shm_ptr);

	int semid = semget(shm_key, 1, IPC_CREAT | 0666);
	if (semid == -1) {
		fprintf(stderr, "semget failed: %s(%d)\n", strerror(errno), errno);
		exit(1);
	}

	shmid = shmget(shm_key, shmSize, IPC_CREAT | 0666);
	if (shmid == -1) {
		fprintf(stderr, "%s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	printf("[second] key: %d\n[second] mem_id: %d\n[second] sem_id: %d\n", shm_key, shmid, semid);

	shm_ptr = shmat(shmid, NULL, SHM_RDONLY);
	printf("[second] shm_ptr: %p\n", shm_ptr);

	printf("[second] Waiting...\n");
	
	struct tm* time_info;
  time_t rawtime;
  time(&rawtime);
	time_info = localtime(&rawtime);
	printf("{%2d:%2d:%2d} pid = %d reading shmem: %s\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, getpid(), shm_ptr);
	shmdt(shm_ptr);	
	
	int res = semop(semid, &unsetSem, 1);
	if (res == -1) {
		int err = errno;
		printf("[second] semop: %s(%d)\n", strerror(err), err);
	}
	else {
		printf("[second] unset semaphore\n");
	}

	return 0;
}