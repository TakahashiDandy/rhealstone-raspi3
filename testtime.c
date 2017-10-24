/*
 * testtime.c
 *
 *  Created on: Dec 19, 2016
 *      Author: takahashidandy
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void testime(){
	clock_t startime;
	clock_t endtime;

	for(int loop = 0; loop < 100; loop++){
		startime = clock();
		for(int i = 0; i < 100; i++){
			for(int j = 0; j < 100; j++){
					//printf("%d , %d \n", i, j);
				}
		}
		endtime = clock();
		clock_t total = (double) endtime - startime;
		double totaltime = (double) total / CLOCKS_PER_SEC;
		printf("%lf\n",totaltime);
	}
}

