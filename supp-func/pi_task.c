#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

void pi_semaphore_release(pthread_mutex_t mutex){
	//pthread_mutex_lock(&mutex);
	//sem_flag = 0;
	//pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}

void pi_semaphore_obtain(pthread_mutex_t mutex){
	pthread_mutex_lock(&mutex);
	//sem_flag = 1;
	//pthread_yield();
	//pthread_cond_wait(&cond,&mutex);
	//pthread_mutex_unlock(&mutex);
}
