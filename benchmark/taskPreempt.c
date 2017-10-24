#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>

#define MAX_THREAD 2
#define CLOCK_ARM CLOCKS_PER_SEC
#define LOOP 50000

void showPreempt(double totalC, double lO, double tS, double dO, int times);
double divClocks(clock_t endC, clock_t startC);
clock_t getClock(void);
void showHeader(char Message[50]);
void *Taskp1();
void *Taskp2();


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct sched_param param;
pthread_cond_t cond;
pthread_attr_t task_attr;
pthread_t task1,task2;

int mutex_flag = 0;
double tSwitch = 0, looppOver = 0, dirpOver = 0, yTime = 0, rTime=0;
int countp1 = 0,countp2 = 0;
clock_t eClock, sClock;

void *Taskp1(){
	param.sched_priority = sched_get_priority_min(SCHED_RR);
	int a = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 1 running : %d \n", a);

	// taskswitch time
	clock_t stClock = getClock();
	pthread_create( &task2 ,&task_attr,Taskp2,NULL);
	clock_t etClock = getClock();
	tSwitch = (double) divClocks(etClock,stClock) / CLOCK_ARM;


	sClock = getClock();
	for(countp2=0; countp2 < LOOP;countp2++){
		pthread_kill(&task2,SIGCONT);
	}
	printf("%ld \n", sClock);
	return 0;
}

void *Taskp2(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);
	printf("Task 2 running : %d \n", b);

	// get taskswitch time.
	pthread_kill(&task2,SIGSTOP);


	for(countp1 =0; countp1 < LOOP - 1;countp1++){
		pthread_kill(&task2,SIGSTOP);
	}
	eClock = getClock();
	printf("%ld \n", eClock);
	rTime = (double) divClocks(eClock,sClock) / CLOCK_ARM;


	return 0;
}

void TaskPreempt(){
	showHeader("Task Preempt");

	yTime = (double) 0;

	//loop_overhead
	clock_t startOverhead = getClock();
	for(countp1 =0; countp1 < LOOP - 1;countp1++){

	}
	clock_t endOverhead = getClock();
	looppOver = (double) divClocks(endOverhead,startOverhead) / CLOCK_ARM;

	pthread_attr_init(&task_attr);
	pthread_attr_setschedpolicy(&task_attr, SCHED_RR);

	pthread_create( &task1 ,&task_attr,Taskp1,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);

	showPreempt(rTime,looppOver,tSwitch, yTime, LOOP);

}

