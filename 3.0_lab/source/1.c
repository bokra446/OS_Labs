#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>


const char* shmName = "shmem_file";
size_t shmSize = 64;
char* shm_ptr;
int shmid;

void handler(){
	int save_errno = errno;
	errno = save_errno;
  shmdt(shm_ptr);
	shmctl(shmid, IPC_RMID, NULL);

	printf("Destroying the shared memory segment\n");
	remove(shmName);

  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	signal(SIGINT, handler);
	signal(SIGTERM, handler);

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
	printf("[first] key: %d\n[first] mem_id: %d\n", shm_key, shmid);

	shm_ptr = shmat(shmid, NULL, 0);
	printf("[first] shm_ptr: %p\n", shm_ptr);

	while(1) {
		struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char str[1024];
		sprintf(str, "[PARENT] {%2d:%2d:%2d} pid = %d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, getpid());
		strcpy(shm_ptr, str);
		sleep(5);
	}	
	return 0;
}