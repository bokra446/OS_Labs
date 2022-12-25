#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main() {
	char* fifo_file = "./fifo_file";
	int ff = mkfifo(fifo_file, 0777);
	if (ff != 0) {
		int save_error = errno;
		fprintf(stderr, "Error %d: %s", save_error, strerror(save_error));
		exit(save_error);
	}

	int fd;
	pid_t pid;
	
	pid = fork();
	if (pid > 0) {
    struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char str[1024];
		sprintf(str, "[PARENT] {%.2d:%.2d:%.2d} pid = %d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, getpid());
		
		fd = open(fifo_file, O_WRONLY);
		int i = write(fd, str, sizeof(str));
    if(i == -1){ 
      printf("Write error in fifo_file");
    }
		close(fd);
	}
	else {
		sleep(5);
		struct tm* time_info;
    time_t rawtime;
    time(&rawtime);
		time_info = localtime(&rawtime);
		char buf[1024];
		printf("[CHILD] {%.2d:%.2d:%.2d}: ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
		
		fd = open(fifo_file, O_RDONLY);
    int len;
		if ((len = read(fd, buf, 1024)) != 0) {
			printf("%s\n", buf);
		}
    else{
       printf("Read error in fifo_file");
    }
		close(fd);
	}
	return 0;
}