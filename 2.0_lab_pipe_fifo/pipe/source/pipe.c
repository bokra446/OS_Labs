#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main() {
	int fd[2];
	pid_t pid;
  int i = pipe(fd);
	if (i != 0){
    int save_error = errno;
		fprintf(stderr, "Error %d: %s", save_error, strerror(save_error));
		exit(save_error);
  }
	
	pid = fork();
	if (pid > 0) {
    struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char str[1024];
		sprintf(str, "[PARENT] {%2d:%2d:%2d} pid = %d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, getpid());
		
		close(fd[0]);
		int i = write(fd[1], str, sizeof(str));
    if(i == -1){ 
      printf("Write error in fifo_file");
    }
		close(fd[1]);
	}
	else {
		sleep(5);
		struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char buf[1024];
		printf("[CHILD] {%2d:%2d:%2d}: ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		
		close(fd[1]);
    int len;
		if ((len = read(fd[0], buf, 1024)) != 0) {
			printf("%s\n", buf);
		}
    close(fd[0]);
	}
	return 0;
}