#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define TrucksNumber 5
#define CarsNumber 5
#define TotalVehicles  10


// semaphores
sem_t semCounter;
pthread_mutex_t truckMutex;

// random number generator
int randomDraw(double m){
	int r=(int) (m*rand()/(RAND_MAX+1.0));
	if(r<1)
		r=1;
	return r;
}

// wait for a random time
void wait(double m){
	struct timespec delay;
	delay.tv_sec=randomDraw(m);
	delay.tv_nsec=0;
	nanosleep(&delay,NULL);
}

// bridge
void accessBridge(int tons){
	if (tons==15){
		pthread_mutex_lock(&truckMutex);
		while(tons>0){
			sem_wait(&semCounter);
			tons=tons-5;
		}
		pthread_mutex_unlock(&truckMutex);
	} else  sem_wait(&semCounter);
}

void leaveBridge(int tons){
	while(tons>0){
		sem_post(&semCounter);
		tons=tons-5;
	}
}

// threads
void* car(void* arg){
	wait(5);
	accessBridge(5);
	printf("car %d is crossing the bridge\n",*((int*)arg));
	wait(5);
	printf("car %d has left the bridge\n",*((int*)arg));
	leaveBridge(5);
	
	pthread_exit(NULL);
}

void* truck(void* arg){
	wait(5);
	accessBridge(15);
	printf("truck %d is crossing the bridge\n",*((int*)arg));
	wait(5);
	printf("truck %d has left the bridge\n",*((int*)arg));
	leaveBridge(15);

	pthread_exit(NULL);
}


// main
void main(){
	int i;
	pthread_t id;

	sem_init(&semCounter,0,3);
	
	for(i=0; i<TotalVehicles;i++){
		int* r=(int*)malloc(sizeof(int));
		*r=i;
		if (i<TrucksNumber)
			pthread_create(&id,NULL,truck,r);
		else	
			pthread_create(&id,NULL,car,r);
	}

	pthread_exit(NULL);
}
