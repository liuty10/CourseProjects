/*
 * =====================================================================================
 *
 *       Filename:  multi-thread.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/10/2017 10:39:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void *create(void *arg){
  long a = (long)arg;
  printf("New thread %ld .. \n",a);
  printf("This thread's id is %u \n", (unsigned int)pthread_self());
  printf("The add of a : %lx \n", &a);
  while(1){
    ;
  }  
  return (void*)0;
}

int main(){
   
  pthread_t tid1, tid2;
  printf("Main thread is starting..\n");
  printf("The main process's pid is %d\n", getpid());
  long error=pthread_create(&tid1,NULL,create,(void*)1);
  if(error){
    printf("thread1 is not created .. \n");
    return -1;
  }else
    printf("thread1 is created .. \n");

  error=pthread_create(&tid2,NULL,create,(void*)2);
  if(error){
    printf("thread2 is not created .. \n");
    return -1;
  }else
    printf("thread2 is created .. \n");
  
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  return 0;
}
