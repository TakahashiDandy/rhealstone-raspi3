#include <stdio.h>
#include <stdlib.h>

void showHeader(char Message[50]);
void showTaskSwitch(double totalC, double lO, double dO, int times);

void showHeader(char Message[50]){
	printf("=====================================================\n");
	printf("=========Rhealstone Benchmark: %s ==========\n", Message);
}

void showTaskSwitch(double totS, double lO, double dO, int times){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 2) - 1);
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver time              : %lf seconds.\n",lO);
	printf("Yield Time                 : %lf seconds. \n", dO);
	double cSwitch = totS - (lO + dO);
	printf("Context Switch time        : %lf seconds.\n",cSwitch );
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/taskSwitch.txt", cSwitch,(times * 2) - 1);
}

void showPreempt(double totS, double lO, double tS, double dO, int times){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 2) - 1);
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver time              : %lf seconds.\n",lO);
	printf("TaskSwitch time            : %lf seconds.\n",tS);
	printf("Yield Time                 : %lf seconds. \n", dO);
	double tPreempt = totS - (lO + tS + dO);
	printf("Preemption time            : %lf seconds.\n",tPreempt );
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/taskPreempt.txt", tPreempt,(times * 2) - 1);
}

void showSSN(double totS, int times, double tS){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 2));
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver Time              : %lf seconds.\n",tS);
	double semShuf = totS - (tS);
	printf("Semaphore Shuffling time   : %lf seconds.\n",semShuf);
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/semaphoreShuffleNo.txt", semShuf,(times * 2));
}

void showSS(double totS, int times, double tS){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 2));
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver Time              : %lf seconds.\n",tS);
	double semShuf = totS - (tS);
	printf("Semaphore Shuffling time   : %lf seconds.\n",semShuf);
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/semaphoreShuffleYes.txt", semShuf,(times * 2));
}

void showDDN(double totS, int times, double tSw, double tObt){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 3));
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver Time              : %lf seconds.\n",tSw);
	printf("Semaphore obtain           : %lf seconds.\n",tObt);
	double dl = totS - (tSw + tObt);
	printf("Semaphore Shuffling time   : %lf seconds.\n",dl);
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/deadlockBreaktimeNo.txt", dl,(times * 3));
}

void showDD(double totS, int times, double tSw, double tObt){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times * 3));
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver Time              : %lf seconds.\n",tSw);
	printf("Semaphore obtain           : %lf seconds.\n",tObt);
	double dl = totS - (tSw + tObt);
	printf("Semaphore Shuffling time   : %lf seconds.\n",dl);
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/deadlockBreaktimeYes.txt", dl,(times * 3));
}

void showML(double totS, int times, double tloop, double tRec){
	printf("====================BENCHMARK RESULT=================\n");
	printf("Total Benchmark            : %d times  \n",(times - 1));
	printf("Total Clock from task      : %lf seconds.\n",totS);
	printf("LoopOver Time              : %lf seconds.\n",tloop);
	printf("Message Receive            : %lf seconds.\n",tRec);
	double ML = totS - (tloop + tRec);
	printf("Message Latency time       : %lf seconds.\n",ML);
	printf("=====================================================\n");
	outTxt("/home/pi/RPi/messageLatency.txt", ML,(times - 1));

}

void outTxt(char out_namaFile[30], double out_nilai, int times){
	FILE *file;
	file = fopen(out_namaFile,"a+");
	fprintf(file,"%ld; ",times);
	fprintf(file,"%lf; \n",out_nilai);
	fclose(file);
	printf("File Created.\n");
}

