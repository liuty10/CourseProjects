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

#include <iostream>
#include <stdlib.h>

#include "Philosopher.h"

Mutex *Chopstick[PHILOSOPHERS]; //locks for chopsticks

int main(int argc, char *argv[]){
  Philosopher *Philosophers[PHILOSOPHERS];
  int i, iter;
  strstream name;
  
  if(argc!=2){
    cout << "Use " << argv[0] <<  " #-of-iterations." << endl;
    exit(0);
  }else
    iter = abs(atoi(argv[1]));

  for(i=0; i< PHILOSOPHERS; i++){
    name.seekp(0, ios::beg);
    name << "ChopStick" << i << '\0';
    Chopstick[i] = new Mutex(name.str()); 
  }

  for(i=0; i< PHILOSOPHERS; i++){
    Philosophere[i] = new Philosopher(i, iter);
    Philosophere[i]->Begin(); 
  }

  for(i=0; i< PHILOSOPHERS; i++){
    Philosophers[i]->Join();
  }
  Exit();
  return 0;
}















