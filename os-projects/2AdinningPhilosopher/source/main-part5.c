/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/01/2017 10:02:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PHILOSOPHER 1000

pthread_t tidTable[MAX_PHILOSOPHER]; 
pthread_mutex_t chopsticksMutex[MAX_PHILOSOPHER]; //locks for chopsticks
pthread_cond_t chopsticksCond[MAX_PHILOSOPHER]; //locks for chopsticks
int iNumThread;
//--------------------------------------------------------------------------------------

void thinking(){
  srandom(time(0));
  usleep(random()%500);
}

void pickUpChopsticks(long threadX){
  int bfailLeft=1;
  int bfailRight=1;
  while(bfailLeft || bfailRight){
    //printf("Philosopher%ld is getting the Left chopstic...\n",threadX);
    bfailLeft = pthread_mutex_trylock(&chopsticksMutex[threadX]);
    if(bfailLeft){
      pthread_cond_wait(&chopsticksCond[threadX], &chopsticksMutex[threadX]);
    }
    if(bfailLeft || bfailRight)
      break;
   // printf("Philosopher%ld get the Left chopstic\n",threadX);

    //printf("Philosopher%ld is getting the Right chopstic...\n",threadX);
    bfailRight = pthread_mutex_trylock(&chopsticksMutex[(threadX+1)%iNumThread]);
    if(bfailLeft || bfailRight)
      break;
    if(bfailRight){
      pthread_mutex_unlock(&chopsticksMutex[threadX]);
      bfailLeft = 1;
      pthread_cond_wait(&chopsticksCond[threadX], &chopsticksMutex[threadX]);
      bfailRight = 0;
    }
  }
  //printf("Philosopher%ld get the Right chopstic...\n",threadX);
}

void eating(){
  srandom(time(0));
  usleep(random()%500);
}

void putDownChopsticks(long threadX){
 //printf("Philosopher%ld is putting the Left chopstic...\n",threadX);
 pthread_cond_signal(&chopsticksCond[threadX]);
 pthread_mutex_unlock(&chopsticksMutex[threadX]);
 //printf("Philosopher%ld put the Left chopstic\n",threadX);
 
 //printf("Philosopher%ld is putting the Right chopstic...\n",threadX);
 pthread_cond_signal(&chopsticksCond[(threadX+1)%iNumThread]);
 pthread_mutex_unlock(&chopsticksMutex[(threadX+1)%iNumThread]);
 //printf("Philosopher%ld put the Right chopstic...\n",threadX);
}

//---------------------------------------------------------------------------------------

void *PhilosopherThread(void *str){
  long threadX = (long)str;
  //printf("This is philosopher %ld\n",threadX);
  thinking();

  pickUpChopsticks(threadX);
  fprintf(stderr,"Philosopher %ld is eating\n", threadX);  
  eating();
  
  putDownChopsticks(threadX);
}

void createPhilosophers(int iNumThread){
  long i,err;
  for(i = 0; i< iNumThread; i++){
    err = pthread_create(&tidTable[i],NULL,PhilosopherThread,(void*)i);
    if(err){                    //return !NULL, succeed.
      printf("create %ldth thread failed\n",i);
      exit(0);
    }
  }
  for(i=0;i<iNumThread;i++){
    pthread_join(tidTable[i],NULL);
  }
  printf("N threads have been joined successfully\n");
}


int main(int argc, char *argv[]){
  int i;
  
  if(argc!=2){
    printf("Use %s #-of-threads.\n", argv[0]);
    exit(0);
  }else{
    iNumThread = abs(atoi(argv[1]));
    if(iNumThread > MAX_PHILOSOPHER)
    { printf("iNumThread should be smaller than %d\n",MAX_PHILOSOPHER);
      exit(0);
    }
  }
  printf("Tianyi Liu is creating %d threads\n", iNumThread);
  for(i=0;i<iNumThread;i++){
    pthread_mutex_init(&chopsticksMutex[i],NULL);
    pthread_cond_init(&chopsticksCond[i],NULL);
  }
  createPhilosophers(iNumThread);

  return 0;
}















