/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/01/2017 05:16:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int main(){
  pthread_mutex_t mutex1;
  pthread_mutex_init(&mutex1,NULL);

  srand(time(0));
  while(1){
    long int i=random()%50;
    printf("%ld\n",i);
    sleep(1);
    pthread_mutex_lock(&mutex1);
    printf("lock mutex1, and try to lock mutex1 again\n");
    pthread_mutex_lock(&mutex1);
    printf("lock mutex1 again\n");

  }
  return 0;
}
