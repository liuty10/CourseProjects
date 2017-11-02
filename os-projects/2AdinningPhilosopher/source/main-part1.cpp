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
#include <pthread.h>

#define PHILOSOPHERS 5

pthread_t tidTable[PHILOSOPHERS]; //locks for chopsticks

void *PhilosopherThread(void *str){
  long threadX = (long)str;
  printf("This is philosopher %d\n",threadX);
}

void createPhilosophers(int iNumThread){
  long i,err;
  for(i = 0; i< iNumThread; i++){
    err = pthread_create(&tidTable[i],NULL,PhilosopherThread,(void*)i);
    if(err){                    //return !NULL, succeed.
      printf("create %dth thread failed\n",i);
      exit(0);
    }
  }
  for(i=0;i<PHILOSOPHERS;i++){
    pthread_join(tidTable[i],NULL);
  }
  printf("N threads have been joined successfully\n");
}


int main(int argc, char *argv[]){
  int iNumThread;
  
  if(argc!=2){
    printf("Use %s #-of-iterations.\n", argv[0]);
    exit(0);
  }else
    iNumThread = abs(atoi(argv[1]));
  
  printf("Tianyi Liu is creating %d threads\n", iNumThread);
  createPhilosophers(iNumThread);

  return 0;
}















