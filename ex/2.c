#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define TrucksNumber 4
#define CarsNumber 8
#define TotalVehicles  (TrucksNumber+CarsNumber)


#define waiting 1
#define crossing 3
#define other 2


// semaphores
pthread_mutex_t mutex;
sem_t sempriv[TotalVehicles];

int state[TotalVehicles]; // state of each vehicle
int blockedTrucks=0; // number of blocked trucks
int threshold=0; // sum of the tons of the vehicles crossing the bridge

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
void accessBridge(int tons, int id){
	pthread_mutex_lock(&mutex);
	// if the bridge supports the vehicle weight, vehicle can cross
	if(threshold+tons<=15){
		state[id]=crossing;
		threshold+=tons;
		sem_post(&sempriv[id]);
	} else {
		// if the vehicle is a truck, it can block the bridge
		if(tons==15){
			blockedTrucks++;
		} 
		// if the vehicle is a car, it can't block the bridge
		state[id]=waiting;
	}
	pthread_mutex_unlock(&mutex);
	sem_wait(&sempriv[id]);
}

void leaveBridge(int tons, int id){
	int i;
	pthread_mutex_lock(&mutex);
	state[id]=other;
	threshold-=tons;
	// verify if there are blocked trucks
	while((i<TotalVehicles)&&(threshold==0)){
		if(state[i]==waiting){
			threshold=15;
			blockedTrucks--;
			sem_post(&sempriv[i]);
		}
		i++;
	}

	for(i=TrucksNumber; i<TotalVehicles; i++){
		if(state[i]==waiting && threshold<15 && blockedTrucks==0){
			threshold+=5;
			sem_post(&sempriv[i]);
		}
	}
	pthread_mutex_unlock(&mutex);
}

// threads
void* car(void* arg){
	wait(5);
	accessBridge(5, *((int*)arg));
	printf("car %d is crossing the bridge\n",*((int*)arg));
	wait(5);
	printf("car %d has left the bridge\n",*((int*)arg));
	leaveBridge(5, *((int*)arg));
	pthread_exit(NULL);
}

void* truck(void* arg){
	wait(5);
	accessBridge(15, *((int*)arg));
	printf("truck %d is crossing the bridge\n",*((int*)arg));
	wait(5);
	printf("truck %d has left the bridge\n",*((int*)arg));
	leaveBridge(15, *((int*)arg));

	pthread_exit(NULL);
}

// main
void main(){
	int i;
	pthread_t id;

	for (i=0; i<TotalVehicles; i++){
		state[i]=other;
		sem_init(&sempriv[i],0,0);
	}

	pthread_mutex_init(&mutex,NULL);

	for(i=0; i<TotalVehicles;i++){
		int* r=(int*)malloc(sizeof(int));
		*r=i;
		if (i<TrucksNumber)
			pthread_create(&id,NULL,truck,r);
		else	pthread_create(&id,NULL,car,r);
	}
	
	pthread_exit(NULL);
}
