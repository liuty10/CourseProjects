#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdarg.h>
#include "xmemory.h"
#include "xthread.h"

extern "C" {
  void initializer (void) __attribute__((constructor));
  void finalizer (void)   __attribute__((destructor));
  bool initialized = false;
  //unsigned long globalStart, globalEnd;
  //unsigned long heapStart, heapEnd;
	enum { InitialMallocSize = 1024 * 1024 * 1024 };

  void initializer (void) {
    // Using globals to provide allocation
    // before initialized.
    init_real_functions();
    xmemory::getInstance().initialize();

    initialized = true;
		fprintf(stderr, "Now we have initialized successfuuly\n"); 
  }

  void finalizer (void) {
    initialized = false;
  }

  // Temporary mallocation before initlization has been finished.
  static void * tempmalloc(int size) {
    static char _buf[InitialMallocSize];
    static int _allocated = 0;
  
    if(_allocated + size > InitialMallocSize) {
      printf("Not enough space for tempmalloc");
      abort();
    } else {
      void* p = (void*)&_buf[_allocated];
      _allocated += size;
      return p;
    }
  }

  /// Functions related to memory management.
  void * malloc (size_t sz) {
    void * ptr;
    if(sz == 0) {
      sz = 1;
    }

    // Align the object size. FIXME for now, just use 16 byte alignment and min size.
    if(!initialized) {
      if (sz < 16) {
        sz = 16;
      }
      sz = (sz + 15) & ~15;
      ptr = tempmalloc(sz);
    }
    else {
      ptr = xmemory::getInstance().malloc(sz);
    }

    if (ptr == NULL) {
      fprintf (stderr, "Out of memory!\n");
      ::abort();
    }
//    fprintf(stderr, "********MALLOC sz %ld ptr %p***********\n", sz, ptr);
    return ptr;
  }
  
  void * calloc (size_t nmemb, size_t sz) {
    void * ptr;
    
    ptr = malloc (nmemb * sz);
	  memset(ptr, 0, sz*nmemb);
    return ptr;
  }

  void free (void * ptr) {
    // We donot free any object if it is before 
    // initializaton has been finished to simplify
    // the logic of tempmalloc
    if(initialized) {
      xmemory::getInstance().free (ptr);
    }
  }

  size_t malloc_usable_size(void * ptr) {
    if(initialized) {
      return xmemory::getInstance().malloc_usable_size(ptr);
    }
    return 0;
  }

  // Implement the initialization
  void * memalign (size_t boundary, size_t size) {
    if(!initialized) {
			initializer();
    }
    return xmemory::getInstance().memalign(boundary, size);
   // return NULL;
  }

  void * realloc (void * ptr, size_t sz) {
    if(initialized) {
      return xmemory::getInstance().realloc(ptr, sz);
    }
    else {
      return tempmalloc(sz);
    }
  }
  //***************************************************************************************
  int pthread_create(pthread_t * tid, const pthread_attr_t * attr, void *(*fn)(void*), void *arg)  {
    if(initialized){
      *tid = *(pthread_t*)xthread::thread_create(fn, arg);
    }
    return 0;
  }

  int pthread_join(pthread_t tid, void ** val) {
	  //assert(initialized);
	  if(initialized) {
		  xthread::thread_join((void*)tid, val);
	  }
	  return 0;
  }
  
  int pthread_cancel(pthread_t thread) {
	  if(initialized) {
		  xthread::thread_cancel((void*) thread);
	  }
	  return 0;
  }

/*
void pthread_exit(void * value_ptr) {
	if(initialized) {
		xthread::threadDeregister();
	}
	_exit(0);
}
*****************************************************************************************
  

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *) {
	if(initialized) {
		return xthread::mutex_init(mutex);
	}
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
	if(initialized) {
		xthread::mutex_lock(mutex);
	}
	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	if(initialized) {
		xthread::mutex_unlock(mutex);
	}
	return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
	DEBUG("pthread_mutex_trylock is not supported");
	return 0;
}

int pthread_mutex_destory(pthread_mutex_t *mutex) {
	if(initialized) {
		return xthread::mutex_destroy(mutex);
	}
	return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t *) {
	return 0;
}

*****************************************************************************************

int pthread_cond_init(pthread_cond_t * cond, const pthread_condattr_t *attr) {
	//assert(initialized);
	if(initialized) {
		xthread::cond_init((void *) cond);
	}
	return 0;
}

int pthread_cond_broadcast(pthread_cond_t * cond) {
	//assert(initialized);
	if(initialized) {
		xthread::cond_broadcast((void *) cond);
	}
	return 0;
}

int pthread_cond_signal(pthread_cond_t * cond) {
	//assert(initialized);
	if(initialized) {
		xthread::cond_signal((void *) cond);
	}
	return 0;
}

int pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex) {
	//assert(initialized);
	if(initialized) {
		xthread::cond_wait((void *) cond, (void*) mutex);
	}
	return 0;
}

int pthread_cond_destroy(pthread_cond_t * cond) {
	//assert(initialized);
	if(initialized) {
		xthread::cond_destroy(cond);
	}
	return 0;
}
******************************************************************************************

// Add support for barrier functions
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t * attr, unsigned int count) {
	//assert(initialized);
	if(initialized) {
		return xthread::barrier_init(barrier, count);
	}
	return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
	//assert(initialized);
	if(initialized) {
		return xthread::barrier_destroy(barrier);
	}
	return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
	//assert(initialized);
	if(initialized) {
		return xthread::barrier_wait(barrier);
	}
	return 0;
}
*/

};

