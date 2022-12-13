
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define TrucksNumber	5
#define CarsNumber	5
#define TotalVehicles  (TrucksNumber+CarsNumber)


#define Waiting 1
#define Other 2
#define Crossing 3


void wait(double max);
int randomDraw(double max);


void* car(void* arg);
void* truck(void* arg);

pthread_mutex_t sc;


sem_t sempriv[TotalVehicles];


int state[TotalVehicles];

int blockedTrucks=0;


int threshold=0;




void accessBridge(int tons, int id) 
{
	pthread_mutex_lock(&sc);

	if(threshold+tons <= 15)
		{
            threshold=threshold+tons;
            state[id]=Crossing;
            sem_post(&sempriv[id]);
		}
	else	{
		state[id]=Waiting;
		if(tons==15)
			blockedTrucks++;
		}
	
	pthread_mutex_unlock(&sc);
	sem_wait(&sempriv[id]);
}



void leaveBridge(int tons, int id)
{
	int i;

	pthread_mutex_lock(&sc);

	state[id]=Other;
	threshold=threshold-tons;

	for(i=0; i<TrucksNumber;i++)
		if ( (state[i]==Waiting) && (threshold==0) )
			{
                sem_post(&sempriv[i]);
                threshold=15;
                blockedTrucks--;
			}

	for(i=TrucksNumber; i<TotalVehicles;i++)
		if ( (threshold<15) && (blockedTrucks==0)
			&& (state[i]==Waiting) )
			{
                threshold=threshold+5;
                sem_post(&sempriv[i]);
			}

	pthread_mutex_unlock(&sc);
}





int main()
{
	int i;
	pthread_t id;


	for(i=0; i<TotalVehicles;i++)
			{
                state[i]=Other;
                sem_init(&sempriv[i],0,0);
			}

	pthread_mutex_init(&sc,0);
	
	for(i=0; i<TotalVehicles;i++)
			{
			int* j=(int*)malloc(sizeof(int));
			*j=i;
			if (i<TrucksNumber)
				pthread_create(&id,NULL,truck,j);
			else	
                pthread_create(&id,NULL,car,j);
			}


	pthread_exit(NULL);
};



void* car(void* arg)
{
	int id=*((int*)arg);

	wait(5.0);
	accessBridge(5, id);
	printf("car %d is crossing the bridge\n",id);
	wait(5.0);
	printf("car %d has left the bridge\n",id);
	leaveBridge(5, id);

	pthread_exit(NULL);
}

void* truck(void* arg)
{
	int id=*((int*)arg);

	wait(5.0);
	accessBridge(15, id);
	printf("truck %d is crossing the bridge\n",id);
	wait(5.0);
	printf("truck %d has left the bridge\n",id);
	leaveBridge(15, id);

	pthread_exit(NULL);
}





int randomDraw(double m)
{
        int j=(int) (m*rand()/(RAND_MAX+1.0));
        if(j<1)
                j=1;
        return j;
}



void wait(double m)
{
        struct timespec delay;

        delay.tv_sec=randomDraw(m);
        delay.tv_nsec=0;
        nanosleep(&delay,NULL);
}

