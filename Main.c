#include <stdio.h>
#include <stdlib.h>

int main(){
	int times = 0;
	for(times = 1; times <= 50; times++){
		TaskSwitch();
		TaskPreempt();
		SemaphoreShuffle();
		Deadlock();
		MessageLatency();
		printf("@@@@@@@@@@@@@@@@@@@times : %d @@@@@@@@@@@@@\n",times);
		sleep(1);
	}
//	testime();
	return 0;
}


