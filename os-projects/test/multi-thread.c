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
int haha = 123;

void *create1(void *arg){
  printf("haha in create 1: %d\n", haha);
  long a = (long)arg;
  printf("New thread %ld .. \n",a);
  printf("The addr of a : %p \n", &a);
  int * aa = (int*)malloc(10*sizeof(int));
  printf("addr of aa is:%p\n", aa);
  int i =0;
  for(i=0;i<10;i++){
    aa[i] = i;
    printf("addr of aa is: 0x%lx : %d\n", &aa[i], aa[i]);
  }
  sleep(20);
  free(aa);
  return (void*)0;
}

void *create2(void *arg){
  printf("haha in create 2: %d\n", haha);
  long b = (long)arg;
  printf("New thread %ld .. \n",b);
  printf("The addr of b : %lx \n", &b);
  int * bb = (int*)malloc(10*sizeof(int));
  printf("addr of bb is:%lx\n", bb);
  int i =0;
  for(i=0;i<10;i++){
    bb[i] = 100;
    printf("addr of bb is: 0x%lx : %d\n", &bb[i], bb[i]);
  }
  sleep(20);
  free(bb);
  return (void*)0;
}
int main(){
   
  printf("haha in main: %d\n", haha);
  pthread_t tid1, tid2;
  printf("Main thread is starting..\n");
  printf("The main process's pid is %d\n", getpid());

  long error=pthread_create(&tid1,NULL,create1,(void*)1);
  printf("The thread1's tid is %d\n", tid1);
  if(error){
    printf("thread1 is not created .. \n");
    return -1;
  }else
    printf("thread1 is created .. \n");

  error=pthread_create(&tid2,NULL,create2,(void*)2);
  printf("The thread2's tid is %d\n", tid2);
  if(error){
    printf("thread2 is not created .. \n");
    return -1;
  }else
    printf("thread2 is created .. \n");
  sleep(20);
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  return 0;
}
