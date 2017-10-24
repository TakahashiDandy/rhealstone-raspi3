#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <sched.h>

#define MAX_THREAD 2
#define CLOCK_ARM CLOCKS_PER_SEC
#define LOOP 50000

double divClocks(clock_t endC, clock_t startC);
clock_t getClock(void);
void showHeader(char Message[50]);
void *TaskDB1();
void *TaskDB2();
void *TaskDB3();


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct sched_param param;
pthread_attr_t task_attr;
pthread_t task1,task2,task3;

int mutex_dflag;
double DelapsedTime = 0, DswOverhead = 0, DobtainOverhead = 0;
int countd1 = 0;
int highprio, lowprio, midprio;
clock_t eClock, sClock;

void *TaskDB1(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int a = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 1 running : %d \n", a);

	sClock = getClock();
	for(countd1 = 0; countd1 < LOOP;countd1++){
		if (mutex_dflag == 1){
			pi_semaphore_obtain(mutex);
		}
		if (mutex_dflag == 1){
			pi_semaphore_release(mutex);
		}
		pthread_kill(&task1, SIGSTOP);
	}
	eClock = getClock();
	DelapsedTime = (double) divClocks(eClock,sClock) / CLOCK_ARM;

	return 0;
}

void *TaskDB2(){
	param.sched_priority = sched_get_priority_max(SCHED_RR) / 2;
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 2 running : %d \n", b);

	if(mutex_dflag == 1){
		pthread_create( &task1 ,&task_attr,&TaskDB1,NULL);
	} else{
		pthread_create( &task1 ,&task_attr,&TaskDB1,NULL);
	}

	for(countd1 = 0; countd1 < LOOP; countd1++){
		pthread_kill(&task2, SIGSTOP);
		pthread_kill(&task1, SIGCONT);
	}

	return 0;
}

void *TaskDB3(){
	param.sched_priority = sched_get_priority_min(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 3 running : %d \n", b);

	if(mutex_dflag==1){
		pi_semaphore_obtain(mutex);
	}

	if(mutex_dflag == 1){
		pthread_create( &task2 ,&task_attr,&TaskDB2,NULL);
	} else{
		pthread_create( &task2 ,&task_attr,&TaskDB2,NULL);
	}

	for(countd1 = 0; countd1 < LOOP; countd1++){
		if (mutex_dflag ==1){
			pi_semaphore_release(mutex);
		}

		if (mutex_dflag == 1){
			pi_semaphore_obtain(mutex);
		}
		pthread_kill(&task2, SIGCONT);
	}

	return 0;
}



void Deadlock(){
	showHeader("Deadlock Breaktime");

	midprio = (int) sched_get_priority_max(SCHED_RR) / 2;
	lowprio = sched_get_priority_min(SCHED_RR);
	highprio = sched_get_priority_max(SCHED_RR) - 4;

	//obtain loop overhead
	clock_t swStart = getClock();
	for(countd1 = 0;countd1 < LOOP * 3;countd1++){

	}
	clock_t swEnd = getClock();
	DswOverhead = (double) divClocks(swEnd,swStart) / CLOCK_ARM;

	// obtain semaphore time
	clock_t semStart = getClock();
	pi_semaphore_obtain(mutex);
	clock_t semEnd = getClock();
	pi_semaphore_release(mutex);
	DobtainOverhead = (double) divClocks(semEnd,semStart) / CLOCK_ARM;

	//set init priority to low
	param.sched_priority = lowprio;
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task Init running : %d \n", b);

	pthread_mutex_init(&mutex,NULL);
	pthread_attr_init(&task_attr);
	pthread_attr_setschedpolicy(&task_attr, SCHED_RR);

	mutex_dflag = 0;
	pthread_create( &task3 ,&task_attr,&TaskDB3,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);
	pthread_join( task3 , NULL);
	showDDN(DelapsedTime,LOOP,DswOverhead,DobtainOverhead);

	DelapsedTime = 0;
	mutex_dflag = 1;
	pthread_create( &task3 ,&task_attr,&TaskDB3,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);
	pthread_join( task3 , NULL);
	showDD(DelapsedTime,LOOP,DswOverhead,DobtainOverhead);


}


