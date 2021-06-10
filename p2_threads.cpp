#include "p2_threads.h"
#include "utils.h"

using namespace std;

pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t emutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;
bool overtime = false;
extern Restroom restroom;
extern struct timeval t_global_start;
vector<Person> waiting_queue;

void *threadfunc(void *parm)
{

	int status;
	printf(" [Thread] Start\n");

	printf(" [Thread] Locks\n");
	status = pthread_mutex_lock(&mutex_1);

    printf(" [Thread] Blocked\n");
    status = pthread_cond_wait(&cond, &mutex_1);

	printf(" [Thread] Starts again.\n");
	for (int i=0; i<3; i++) {
		printf(" [Thread] Complete thread after (%d) seconds\n", (3-i));
		usleep(MSEC(1000));
	}

	printf(" [Thread] Unlocks\n");
	status = pthread_mutex_unlock(&mutex_1);
	printf(" [Thread] Complete\n");
}

void* male_thread(void* param){
	Person p;
	p.set_gender(0);
	// srand(time(NULL));
	
	struct timeval t_curr, t_inline;
	// gettimeofday(&t_curr, NULL);
	// printf("[%lu ms][Input] A person (Man) goes into the queue.\n", get_elasped_time(t_global_start, t_curr));
	pthread_mutex_lock(&wmutex);
	restroom.add_person(p);
	waiting_queue.push_back(p);
	pthread_mutex_unlock(&wmutex);
	long t_waiting;
	bool entered = false;
	bool exitted = false;

	gettimeofday(&t_inline, NULL);
	while(true){
		gettimeofday(&t_curr, NULL);
		t_waiting = get_elasped_time(t_inline, t_curr);
		if(t_waiting > 30){
			pthread_mutex_lock(&emutex);
			while(true){
				pthread_mutex_lock(&wmutex);
				if(restroom.cleared_to_enter(p)){
					restroom.man_wants_to_enter(p);
					entered = true;
				}
				pthread_mutex_unlock(&wmutex);
				if(entered){
					pthread_mutex_unlock(&emutex);
					break;
				}
			}
		}
		else{
			pthread_mutex_lock(&emutex);
			pthread_mutex_lock(&wmutex);
			if(restroom.cleared_to_enter(p)){
				restroom.man_wants_to_enter(p);
				entered = true;
			}
			pthread_mutex_unlock(&wmutex);
			pthread_mutex_unlock(&emutex);
		}
		if(entered){
			break;
		}
		usleep(MSEC(0.5));
	}
	
	while(true){
		pthread_mutex_lock(&emutex);
		pthread_mutex_lock(&wmutex);
		if(p.ready_to_leave()){
			restroom.man_leaves(p);
			exitted = true;
		}
		pthread_mutex_unlock(&wmutex);
		pthread_mutex_unlock(&emutex);
		if(exitted){
			break;
		}
		usleep(MSEC(0.5));
	}

	pthread_exit(0);
}

void* female_thread(void* param){
	Person p;
	p.set_gender(1);
	// srand(time(NULL));
	struct timeval t_curr, t_inline;
	// gettimeofday(&t_curr, NULL);
	// printf("[%lu ms][Input] A person (Woman) goes into the queue.\n", get_elasped_time(t_global_start, t_curr));
	pthread_mutex_lock(&wmutex);
	restroom.add_person(p);
	waiting_queue.push_back(p);
	pthread_mutex_unlock(&wmutex);
	long t_waiting;
	bool entered = false;
	bool exitted = false;

	gettimeofday(&t_inline, NULL);
	while(true){
		gettimeofday(&t_curr, NULL);
		t_waiting = get_elasped_time(t_inline, t_curr);
		if(t_waiting > 30){
			pthread_mutex_lock(&emutex);
			while(true){
				pthread_mutex_lock(&wmutex);
				if(restroom.cleared_to_enter(p)){
					restroom.woman_wants_to_enter(p);
					entered = true;
				}
				pthread_mutex_unlock(&wmutex);
				if(entered){
					pthread_mutex_unlock(&emutex);
					break;
				}
			}
		}
		else{
			pthread_mutex_lock(&emutex);
			pthread_mutex_lock(&wmutex);
			if(restroom.cleared_to_enter(p)){
				restroom.woman_wants_to_enter(p);
				entered = true;
			}
			pthread_mutex_unlock(&wmutex);
			pthread_mutex_unlock(&emutex);
		}
		if(entered){
			break;
		}
		usleep(MSEC(0.5));
	}
	
	while(true){
		pthread_mutex_lock(&emutex);
		pthread_mutex_lock(&wmutex);
		if(p.ready_to_leave()){
			restroom.woman_leaves(p);
			exitted = true;
		}
		pthread_mutex_unlock(&wmutex);
		pthread_mutex_unlock(&emutex);
		if(exitted){
			break;
		}
		usleep(MSEC(0.5));
	}

	pthread_exit(0);
}
