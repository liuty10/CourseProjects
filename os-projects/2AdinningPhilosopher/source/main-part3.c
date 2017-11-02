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
  printf("%ld is trying to pick the chopsticks\n",threadX); 
  pthread_mutex_lock(&mutex_one);
  printf("%ld get the chopsticks\n",threadX); 
  while(index != threadX){ 
      printf("%ld give up chopsticks and sleep\n",threadX); 
      pthread_cond_wait(&cond_one,&mutex_one);
      printf("%ld wake up and try to get lock again\n",threadX); 
  }
  printf("%ld start to use the chopsticks\n",threadX); 
}

void eating(){
  srandom(time(0));
  usleep(random()%500);
}

void putDownChopsticks(long threadX){
 printf("%ld eat\n",threadX); 
 index = (threadX + 1)%iNumThread;
 pthread_cond_broadcast(&cond_one);
 printf("%ld signal cond\n",threadX); 
 pthread_mutex_unlock(&mutex_one);
 printf("%ld signal release lock\n",threadX); 
}

//---------------------------------------------------------------------------------------

void *PhilosopherThread(void *str){
  long threadX = (long)str;
  thinking();

  pickUpChopsticks(threadX);
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















