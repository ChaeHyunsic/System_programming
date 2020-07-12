#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo);
int try1 = 1;
int try2 = 1;

int main() {
	signal(SIGINT, sig_handler);
	signal(SIGTSTP, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);
	for(;;)
		pause();
}

void sig_handler(int signo) {
	pid_t pid;

	switch(signo) {
		case SIGINT:
			if((pid = fork()) < 0) {
				fprintf(stderr,"fork error\n");
			}
			else if(pid == 0) {
				if(execlp("./sig","sig","sigint", (char*)0) < 0) 
					fprintf(stderr,"execl error\n");
			}
			break;
		case SIGTSTP:
			if((pid = fork()) < 0) {
				fprintf(stderr, "fork error\n");
			}
			else if(pid == 0) {
				if(execlp("./sig","sig","sigtstp", (char*)0) < 0)
					fprintf(stderr,"execl error\n");
			}
			break;
		case SIGUSR1:
			if(try1 == 1) {
				try1 = 0;
				kill(0, SIGUSR1);
			}
			else {
				try1 = 1;
			}
			break;
		case SIGUSR2:
			if(try2 == 1) {
				try2 = 0;
				kill(0, SIGUSR2);
			}
			else {
				try2 = 1;
			}
			break;	
	}
}
