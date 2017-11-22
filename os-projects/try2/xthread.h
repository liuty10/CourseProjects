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
              printf("region[%d].start: %p, end: %p\n", i, regions[i].start, regions[i].end);
            }
            for(i=0; i<regionNumb; i++){
              printf("regionNumb: %d, i = %d\n", regionNumb, i); 
              size_t len = (char*)regions[i].end - (char*)regions[i].start;
              printf("regionNumb: before mmap: %zu\n",len); 
              void * backup = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
              printf("regionNumb: after mmap"); 
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
              printf("This is a new thread_create function in for loop\n"); 
            }
        }
        printf("This is a new thread_create function\n"); 
        pthread_t *pid=(pthread_t*)malloc(sizeof(pthread_t));
        void* childstack = mmap(NULL, 1024*1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        if(childstack == MAP_FAILED){
           printf("stack map faild\n");
           exit(0);
        }
        void* childstackend = (void*)((char*)childstack + 1024*1024);
        printf("childstack: %p, end: %p\n", childstack, childstackend); 
        //int child = syscall(SYS_clone, CLONE_FS | CLONE_FILES | SIGCHLD, (void*) 0);
        int child = clone((int(*)(void*))fn, childstackend , CLONE_IO | CLONE_FS | CLONE_FILES | SIGCHLD, arg);

        if (child){//parent process
            *pid = child;
            return pid;
        } 
    }

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

};
