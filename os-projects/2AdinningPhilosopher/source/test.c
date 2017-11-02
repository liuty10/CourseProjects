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
int main(){
  srand(time(0));
  while(1){
    long int i=random()%50;
    printf("%ld\n",i);
    sleep(1);
  }
  return 0;
}
