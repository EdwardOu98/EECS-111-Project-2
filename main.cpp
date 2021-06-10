#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

struct timeval t_global_start;

Restroom restroom;

int main(int argc, char** argv)
{
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);


	pthread_t       tid = 0;
	int             status = 0;
	int             work = 0;
	int i;

	if((argc != 2) || (atoi(argv[1]) <= 0)){
		printf("[ERROR] Expecting 1 integer argument with value greater than 0, but got none.\n");
		printf("[USAGE] ./p2_exec <number>\n");
		exit(1);
	}

	const int num_people = 2 * atoi(argv[1]);
	int num_male = atoi(argv[1]);
	int num_female = atoi(argv[1]);
	struct timeval t_curr;
	
	pthread_t people[num_people];
	srand(time(0));

	for(i = 0; i < num_people; i++){
		int gender = rand() % 2;
		if(((gender == 0) && (num_male != 0)) || (num_female == 0)){
			gettimeofday(&t_curr, NULL);
			printf("[%lu ms][Input] A person (Man) goes into the queue.\n", get_elasped_time(t_global_start, t_curr));
			if(pthread_create(&people[i], NULL, male_thread, NULL)){
				fprintf(stderr, "Error creating thread");
			}
			num_male--;
		}
		else{
			gettimeofday(&t_curr, NULL);
			printf("[%lu ms][Input] A person (Woman) goes into the queue.\n", get_elasped_time(t_global_start, t_curr));
			if(pthread_create(&people[i], NULL, female_thread, NULL)){
				fprintf(stderr, "Error creating thread");
			}
			num_female--;
		}
		int interval = rand() % 5 + 1;
		usleep(MSEC(interval));
	}

	for(i = 0; i < num_people; i++){
		if(pthread_join(people[i], NULL)){
			fprintf(stderr, "Error joining thread");
		}
	}

	return 0;


}

