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

extern "C"{
    typedef void * threadFunction(void *);
}

class xthread{
  public:

  static inline void* thread_create(threadFunction * fn, void* arg){
        printf("This is a new thread_create function\n"); 
        pthread_t *pid=(pthread_t*)malloc(sizeof(pthread_t));
        
        int child = syscall(SYS_clone, CLONE_FS | CLONE_FILES | SIGCHLD, (void*) 0);
        printf("child: %d\n",child); 
        if (child){//parent process
            *pid = child;
            return pid;
        }else{//child process
            void* result = fn(arg);
            return result;
        }
    
    }

		static inline void thread_join(void * v, void ** result) {

			if (v == NULL) {
				fprintf(stderr, "%d: join with invalid parameter\n", getpid());
				return;
			}
			int status;
			waitpid(*(pthread_t*)v, &status, 0);
		}

		static inline void thread_cancel(void *v) {
			kill(*(int*)v, SIGKILL);
		}

};
