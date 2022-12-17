
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

static volatile sig_atomic_t n = 0;
//const char message[] = "Press it again :) \n";

void exitHandler() {
	fprintf(stdout, "\n Call of exit handler\n");
	fprintf(stdout, "Exit from %d\n", getpid());
  fprintf(stdout, "Oh noooo, you could do it\n I die. Bye\n");
}

void sigIntHandler(int signum) {
	fprintf(stdout, "\n %d SIGINT caught from evil Wolf\n", getpid());
	fprintf(stdout, "%d (%s)\n", signum, strsignal(signum));
  ++n;
  fprintf(stdout, "\n Nice try. Press it again)\n");
}

void sigTermHandler(int signum) {
	fprintf(stdout, "\n %d SIGTERM caught from evil Wolf \n", getpid());
	fprintf(stdout, "%d (%s)\n", signum, strsignal(signum));
  fprintf(stdout, "Why? I will play :) \n");
}
/*
void handler(int s){
	int save_errno = errno;
	signal(SIGINT, handler);
	n++;
	write(1, message, sizeof(message) - 1);
	errno = save_errno;
}
*/

int main(){
  printf("[CURRENT]: PID = %6d; PPID = %6d\n", getpid(), getppid());

	if (atexit(exitHandler)) {
		fprintf(stderr, "Set atexit error. I will cry:(\n");
		exit(-2);
	}
	signal(SIGINT, sigIntHandler);
  struct sigaction sigTerm;
  sigTerm.sa_handler = sigTermHandler;
  sigaction(SIGTERM, &sigTerm, NULL);
  fprintf(stdout, "Forked\n");
	pid_t pid = fork();
	switch(pid) {
		case -1:
			fprintf(stderr, "Fork error: %d", errno);
			exit(-1);
		case 0:
			fprintf(stdout, "[CHILD]: PID = %6d; PPID = %6d\n", getpid(), getppid());
			sleep(30);
			break;
		default:
			fprintf(stdout, "[PARENT]: PID = %6d; PPID = %6d\n", getpid(), getppid());
			sleep(30);
			break;
	}
	while(n<25)
		sleep(1);
	return 0;
}