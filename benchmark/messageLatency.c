#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>


#define MAX_THREAD 2
#define CLOCK_ARM CLOCKS_PER_SEC
#define LOOP 50000
#define MSGSIZE 1000

typedef struct msgbuf{
	long mtype;
	char mtext[MSGSIZE];
} message_buff;

void showML();
double divClocks(clock_t endC, clock_t startC);
clock_t getClock(void);
void showHeader(char Message[50]);
void *TaskML1();
void *TaskML2();

struct sched_param param;
pthread_cond_t cond;
pthread_attr_t task_attr;
pthread_t task1,task2;


int countML;
double tMLElapsed, tMLloopOver, tMLrecOver;

void *TaskML1(){
	param.sched_priority = sched_get_priority_min(SCHED_RR);
	int a = (int) sched_setscheduler(getpid(),SCHED_RR,&param);

	key_t key;
	message_buff sbuf;
	int mesgid, mesgflg;
	size_t buf_length;

	mesgflg = IPC_CREAT | 0666;
	key = 1234;

	mesgid = msgget(key, mesgflg);

	sbuf.mtype = 1;
	(void) strcpy(sbuf.mtext, "TEST MESSAGE");
	buf_length = strlen(sbuf.mtext) + 1;

	printf("Task 1 running : %d \n", a);
	msgsnd(mesgid, &sbuf, buf_length, IPC_NOWAIT);

	pthread_create( &task2 ,&task_attr,&TaskML2,NULL);

	for(countML = 0; countML < LOOP; countML++){
		msgsnd(mesgid, &sbuf, buf_length, IPC_NOWAIT);
	}

	return 0;
}

void *TaskML2(){
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	int b = (int) sched_setscheduler(getpid(),SCHED_RR,&param);

	key_t key;
	message_buff rbuf;
	int mesgid;
	key = 1234;

	mesgid = msgget(key, 0666);

	printf("Task 2 running : %d \n", b);
	clock_t recStart = getClock();
	if (msgrcv(mesgid,&rbuf,MSGSIZE, 1,0) < 0) {
	        perror("msgrcv");
	        exit(1);
	}
	clock_t recEnd = getClock();
	tMLrecOver = (double) divClocks(recEnd, recStart) / CLOCK_ARM;


	clock_t startTime = getClock();
	for(countML = 0; countML < LOOP - 1; countML++){
		msgrcv(mesgid,&rbuf,MSGSIZE, 1,0);
	}
	clock_t endTime = getClock();
	tMLElapsed = (double) divClocks(endTime,startTime) / CLOCK_ARM;

	return 0;
}

void MessageLatency(){
	showHeader("Message Latency");

	//loop_overhead
	clock_t startOverhead = getClock();
	for(countML =0; countML < LOOP - 1;countML++){

	}
	clock_t endOverhead = getClock();
	tMLloopOver = (double) divClocks(endOverhead,startOverhead) / CLOCK_ARM;

	pthread_attr_init(&task_attr);
	pthread_attr_setschedpolicy(&task_attr, SCHED_RR);

	pthread_create( &task1 ,&task_attr,&TaskML1,NULL);
	pthread_join( task1 , NULL);
	pthread_join( task2 , NULL);

	showML(tMLElapsed,LOOP,tMLloopOver, tMLrecOver);

}
