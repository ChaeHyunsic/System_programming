#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER;
int cursor = 0;
int argv1;
int argv2;

void *print_second(void *arg);

int main(int argc, char** argv) {
	argv1 = atoi(argv[1]);
	argv2 = atoi(argv[2]);

	pthread_t thread[argv1];
	int r_arr[argv2];
	long int status;

	srand(time(NULL));
	for(int i = 0; i < argv2; i++) {
		r_arr[i] = rand() % 6 + 5;
		printf("%d ", r_arr[i]);
	}
	printf("\n");

	for(int i = 0; i < argv1; i++) {
		pthread_create(&thread[i], NULL, print_second, (void *)r_arr);
	}

	for(int i = 0; i < argv1; i++) {
		pthread_join(thread[i], (void **)&status);
	}

	return 0;
}

void *print_second(void *arg) {
	pthread_t tid = pthread_self();
	int *arr = (int*)arg;
	int index;
	
	while(1) {
		pthread_mutex_lock(&region_mutex);	
		index = cursor;
		if(index == argv2) {
			pthread_mutex_unlock(&region_mutex);
			pthread_mutex_destroy(&region_mutex);
			break;
		}
		else {
			cursor++;
			pthread_mutex_unlock(&region_mutex);
		}
		sleep(arr[index]);
		printf("%x - element[%d] : %d\n", (unsigned int)tid, index, arr[index]);
	}
	pthread_exit((void*)0);
}