/*
 * =====================================================================================
 *
 *       Filename:  Philosopher.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/01/2017 09:11:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include "Philosopher.h"

extern Mutex *Chopstick[PHILOSOPHERS]; // locks for chopsticks
//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------
static stream *Filler(int n){
  int i;
  strstream *Space;

  Space = new strstream;
  for(i = 0; i < n; i++)
    (*Space) << ' ';
  (*Space) << '\0';
  return Space;
}

//-------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------
Philosopher::Philosopher(int Number, int iter): No(Number),Iterations(iter)
{
  ThreadName.seekp(0, ios::beg);
  ThreadName << "Philosopher" << Number << '\0';
}

//
//
//
void Philosopher::ThreadFunc(){
  Thread::ThreadFunc();
  strstream *Space;
  int i;

  Space = Filler(No*2);
  for(i = 0; i<Iterations; i++){
    Delay();                                    //think for a while
    Chopstick[NO]->Lock();                      //get left chopstick
    Chopstick[(NO+1)%PHILOSOPHERS]->Lock();

    cout << Space->str() << ThreadName.str()
          << "begin eating." << endl;
    Delay();                                    //eat for a while
    cout << Space->str() << ThreadName.str()
        << " finish eating." << endl;
    Chopstick[NO]->Unlock();                    // release left chopstick
    Chopstick[(NO+1)%PHILOSOPHERS]->Unlock();   // release right chopstick
  }
  Exit();
}






