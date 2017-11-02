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
 *         Author:  Tianyi Liu, liuty10@gmail.com
 *   Organization:  UT SanAntonio
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PHILOSOPHER 1000

pthread_t tidTable[MAX_PHILOSOPHER]; 
//pthread_mutex_t chopsticksMutex[MAX_PHILOSOPHER]; //locks for chopsticks
pthread_mutex_t mutex_one;
pthread_cond_t  cond_one;
long index = 0;

int iNumThread;
//--------------------------------------------------------------------------------------

void thinking(){
  srandom(time(0));
  usleep(random()%500);
}

void pickUpChopsticks(long threadX){
  
  printf("Philosopher%ld is getting the chopstics\n",threadX);
  pthread_mutex_lock(&mutex_one);
  while(index!=threadX){
    pthread_cond_wait(&cond_one,&mutex_one);
  }
  printf("Philosopher%ld get the chopstics\n",threadX);

  /* printf("Philosopher%ld is getting the Left chopstic...\n",threadX);
  pthread_mutex_lock(&chopsticksMutex[threadX]);
  printf("Philosopher%ld get the Left chopstic\n",threadX);

  printf("Philosopher%ld is getting the Right chopstic...\n",threadX);
  pthread_mutex_lock(&chopsticksMutex[(threadX+1)%iNumThread]);
  printf("Philosopher%ld get the Right chopstic...\n",threadX);*/
}

void eating(){
  srandom(time(0));
  usleep(random()%500);
}

void putDownChopsticks(long threadX){
 printf("Philosopher%ld is putting the chopsticks...\n",threadX);
 
 index = threadX + 1;
 pthread_cond_signal(&cond_one);
 pthread_mutex_unlock(&mutex_one);
 /*printf("Philosopher%ld is putting the Left chopstic...\n",threadX);
 pthread_mutex_unlock(&chopsticksMutex[threadX]);
 printf("Philosopher%ld put the Left chopstic\n",threadX);
  
 printf("Philosopher%ld is putting the Right chopstic...\n",threadX);
 pthread_mutex_unlock(&chopsticksMutex[(threadX+1)%iNumThread]);
 printf("Philosopher%ld put the Right chopstic...\n",threadX);*/
}

//---------------------------------------------------------------------------------------

void *PhilosopherThread(void *str){
  long threadX = (long)str;
  printf("This is philosopher %ld\n",threadX);
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
  //for(i=0;i<iNumThread;i++){
  //  pthread_mutex_init(&chopsticksMutex[i],NULL);
  //}
  pthread_mutex_init(&mutex_one,NULL);
  pthread_cond_init(&cond_one,NULL);
  createPhilosophers(iNumThread);

  return 0;
}















