/*
 * =====================================================================================
 *
 *       Filename:  Philosopher.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/01/2017 09:00:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tianyi Liu 
 *   Organization:  UTSA
 *
 * =====================================================================================
 */
#ifndef _PHILOSOPHER_H
#define _PHILOSOPHER_H

#include "ThreadClass.h"

#define PHILOSOPHERS 5

class Philosopher: public Thread
{
    public:
       Philosopher(int Number, int iter);
    private:
       int No;
       int Iterations;
       void ThreadFunc();
};

#endif




