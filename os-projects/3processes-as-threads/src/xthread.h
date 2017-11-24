/*
 * =====================================================================================
 *
 *       Filename:  xthread.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2017 04:30:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <sys/types.h>
#include <sys/wait.h>
#include "selfmap.hh"
#include "xdefines.h"
#include <sched.h>
#include "libfuncs.h"

bool shared_region_flag = false;

class xthread{
  public:

  static inline void* thread_create(void* (*fn)(void*), void* arg){
        if(shared_region_flag == false){
            shared_region_flag = true;
            selfmap _selfmap = selfmap::getInstance();
            regioninfo regions[64];
            int regionNumb;
            selfmap::getInstance().getGlobalRegions(regions, &regionNumb);

            int i=0;
            for(i=0; i<regionNumb; i++){
              size_t len = (char*)regions[i].end - (char*)regions[i].start;
              void * backup = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
              memcpy(backup, regions[i].start, len);
              if(munmap(regions[i].start, len) == -1){
                printf("unmap region[%d] fail\n",i);
                exit(0);
              }
              void * current = mmap(regions[i].start, len, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
              if(current==MAP_FAILED){
                printf("map region[%d] fail\n",i);
                exit(0);
              }
              memcpy(regions[i].start, backup, len);
              munmap(backup, len);
            }
        }
        pthread_t *pid=(pthread_t*)malloc(sizeof(pthread_t));
        void* childstack = mmap(NULL, 1024*1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        if(childstack == MAP_FAILED){
           printf("stack map faild\n");
           exit(0);
        }
        void* childstackend = (void*)((char*)childstack + 1024*1024);
        //int child = syscall(SYS_clone, CLONE_FS | CLONE_FILES | SIGCHLD, (void*) 0);
        int child = clone((int(*)(void*))fn, childstackend , CLONE_IO | CLONE_FS | CLONE_FILES | SIGCHLD, arg);

        if (child){//parent process
            *pid = child;
            return pid;
        } 
    }

//*****************************************************************************************
		static inline void thread_join(pthread_t v, void ** result) {
			if (v == 0) {
				fprintf(stderr, "%d: join with invalid parameter\n", getpid());
				return;
			}
			int status;
			waitpid((pid_t)v, &status, 0);
		}

		static inline void thread_cancel(void *v) {
			kill(*(int*)v, SIGKILL);
		}

//*****************************************************************************************
    static inline int mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr){
      WRAP(pthread_mutex_init)(mutex, attr);
	    return 0;
    }

    static inline int mutex_lock(pthread_mutex_t *mutex) {
      //printf("in mutext lock() function\n");
		  WRAP(pthread_mutex_lock)(mutex);
	    return 0;
    }

   static inline int mutex_trylock(pthread_mutex_t *mutex) {
		  WRAP(pthread_mutex_trylock)(mutex);
	    return 0;
    }

   static inline int mutex_unlock(pthread_mutex_t *mutex) {
		  WRAP(pthread_mutex_unlock)(mutex);
	    return 0;
    }

   static inline int mutex_destroy(pthread_mutex_t *mutex) {
		  WRAP(pthread_mutex_destroy)(mutex);
	    return 0;
    }
//*****************************************************************************************
  static inline int cond_init(pthread_cond_t * cond, pthread_condattr_t *attr) {
		  WRAP(pthread_cond_init)(cond, attr);
	    return 0;
   }

  static inline int cond_broadcast(pthread_cond_t * cond) {
		  WRAP(pthread_cond_broadcast)(cond);
	    return 0;
   }

 static inline int cond_signal(pthread_cond_t * cond) {
	    WRAP(pthread_cond_signal)(cond);
	    return 0;
  }

 static inline int cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex) {
		  WRAP(pthread_cond_wait)(cond, mutex);
    	return 0;
  }

 static inline int cond_destroy(pthread_cond_t * cond) {
		  WRAP(pthread_cond_destroy)(cond);
	    return 0;
  }
//******************************************************************************************
 static inline int barrier_init(pthread_barrier_t *barrier, pthread_barrierattr_t * attr, unsigned int count) {
		  WRAP(pthread_barrier_init)(barrier, attr, count);
	    return 0;
  }

 static inline int barrier_destroy(pthread_barrier_t *barrier) {
		 WRAP(pthread_barrier_destroy)(barrier);
	   return 0;
  }

 static inline int barrier_wait(pthread_barrier_t *barrier) {
		WRAP(pthread_barrier_wait)(barrier);
	  return 0;
  }
};
