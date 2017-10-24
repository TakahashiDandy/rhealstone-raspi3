#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/perf_event.h>
#include <sched.h>



#define MAX_THREAD 2
#define CLOCK_ARM CLOCKS_PER_SEC
#define LOOP 50000

//void showSemaphore(double totalC, double lO, double dO, int times);
double divClocks(clock_t endC, clock_t startC);
clock_t getClock(void);
void showHeader(char Message[50]);
void *TaskSS1();
void *TaskSS2();
void pi_semaphore_release();
void pi_semaphore_obtain();
void showSS();



pthread_attr_t task_attr;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t task1,task2;
struct sched_param param;
double SStotalClock = 0, SSloopOver = 0, SSdirOver = 0, SSTime = 0 ;
int counts;
int sem_flag = 1;


void *TaskSS2(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int a = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 1 running : %d \n", a);

	clock_t sClock = getClock();
	for(counts = 0; counts  < LOOP;counts++){
		if(sem_flag==1){
			pi_semaphore_obtain(mutex);
			//printf("task2 obtain \n");
		}

		sleep(0);

		if(sem_flag==1){
			pi_semaphore_release(mutex);
		}

		sleep(0);
	}

	clock_t eClock = getClock();
	SStotalClock = (double) divClocks(eClock,sClock)/ CLOCK_ARM;

	return 0;
}

void *TaskSS1(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 1 running : %d \n", b);
	if(sem_flag==0){
		pthread_create( &task2 ,&task_attr,&TaskSS2,NULL);
	}
	else{
		pthread_create( &task2 ,&task_attr,&TaskSS2,NULL);
	}
	sleep(0);

	for(counts = 0; counts < LOOP; counts++){
		if(sem_flag==1){
			pi_semaphore_obtain(mutex);
			//printf("task1 obtain \n");
		}
		sleep(0);

		if(sem_flag==1){
			pi_semaphore_release(mutex);
		}
		sleep(0);
	}

	return 0;
}


void SemaphoreShuffle(){
	showHeader("Semaphore Shuffle");

	clock_t sClock = getClock();
	for(counts=0; counts < LOOP*2;counts++){

	}
	clock_t eClock = getClock();
	SSloopOver = (double) divClocks(eClock,sClock) / CLOCK_ARM;


	pthread_attr_init(&task_attr);
	pthread_attr_setschedpolicy(&task_attr, SCHED_RR);

	param.sched_priority = sched_get_priority_min(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task Init running : %d \n", b);

	// no semaphore;
	sem_flag = 0;
	pthread_create( &task1 ,&task_attr,&TaskSS1,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);
	showSSN(SStotalClock,LOOP,SSloopOver);

	//with semaphore;
	sem_flag = 1;
	pthread_create( &task1 ,&task_attr,&TaskSS1,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);
	showSS(SStotalClock,LOOP,SSloopOver);

}



