

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


clock_t getClock(void);
double divClocks(clock_t endC, clock_t startC);


clock_t getClock(){
	clock_t clocks = clock();
	return clocks;
}

double clocksToSec(int endClock,int startClock){
	double doubleclock = (double) (endClock - startClock) / CLOCKS_PER_SEC;
	printf("Difftime Clock is %lf. \n",  doubleclock);
	return 0;
}

double divClocks(clock_t endClock, clock_t startClock){
	double total = (double) endClock - startClock;
	//printf("Div total is %lf \n", total);
	return total;
}




