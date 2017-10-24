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

void showTaskSwitch(double totalC, double lO, double dO, int times);
double divClocks(clock_t endC, clock_t startC);
clock_t getClock(void);
void showHeader(char Message[50]);
void *Task1();
void *Task2();

struct sched_param param;
pthread_attr_t task_attr;
pthread_t task1,task2;
double totalClock = 0, loopOver = 0, dirOver = 0 ;
int count1 = 0,count2 = 0;

void *Task1(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int a = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	if(a < 0){
		perror("sched_setscheduler error");
		exit(1);
	}
	printf("Task 1 running : %d \n", a);

	pthread_create( &task2 ,&task_attr,&Task2,NULL);

	for(count2=0; count2 < LOOP;count2++){
		sleep(0);
	}

	return 0;
}

void *Task2(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	if(b < 0){
		perror("sched_setscheduler error");
		exit(1);
	}
	printf("Task 2 running : %d \n", b);

	clock_t sClock = getClock();
	for(count1 =0; count1 < LOOP - 1;count1++){
		sleep(0);
	}
	clock_t eClock = getClock();

	totalClock = (double) divClocks(eClock,sClock) / CLOCK_ARM;



	return 0;
}


void TaskSwitch(){
	showHeader("Task Switch");
	//loop_overhead
	clock_t startOverhead = getClock();
	for(count1 =0; count1 < LOOP - 1;count1++){
			//pthread_yield();
	}

	for(count2=0; count2 < LOOP;count2++){
			//pthread_yield();
	}
	clock_t endOverhead = getClock();
	loopOver = (double) divClocks(endOverhead,startOverhead) / CLOCK_ARM;


	//yield time
	clock_t startdir = getClock();
	sleep(0);
	clock_t enddir = getClock();
	dirOver = (double) divClocks(enddir,startdir) / CLOCK_ARM;

	pthread_attr_init(&task_attr);
	pthread_attr_setschedpolicy(&task_attr, SCHED_RR);
	pthread_create( &task1 ,&task_attr,&Task1,NULL);

	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);
	showTaskSwitch(totalClock,loopOver,dirOver, LOOP);
}


