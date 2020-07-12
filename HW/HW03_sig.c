#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signo);
char* argv1;

int main(int argc, char* argv[]) {
	fprintf(stderr,"Start [%s] - %d\n", argv[1], getpid());
	argv1 = argv[1];

	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);

	for(;;)
		pause();
}

void sig_handler(int signo) {
	switch(signo) {
		case SIGUSR1:
			fprintf(stderr,"Running [%s] - %d\n", argv1, getpid());
			break;
		case SIGUSR2:
			fprintf(stderr,"Exit [%s] - %d\n", argv1, getpid());
			exit(0);
			break;
	}
}
