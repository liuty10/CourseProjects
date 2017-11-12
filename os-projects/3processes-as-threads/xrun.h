
  

class xrun {

private:
  static volatile bool _initialized;
  static volatile bool _protection_enabled;
  static size_t _master_thread_id;
  static size_t _thread_index;
  static bool _fence_enabled;
  static size_t _children_threads_count;
  static size_t _lock_count;
  static bool _token_holding;
#ifdef TIME_CHECKING
  static struct timeinfo tstart;
#endif

public:

  /// @brief Initialize the system.
  static void initialize(void) {
    DEBUG("initializing xrun");

    _initialized = false;
    _protection_enabled = false;
    _children_threads_count = 0;
    _lock_count = 0;
    _token_holding = false;

    pid_t pid = syscall(SYS_getpid);

    if (!_initialized) {
      _initialized = true;

      // xmemory::initialize should happen before others
      xmemory::initialize();

      xthread::setId(pid);
      _master_thread_id = pid;
      xmemory::setThreadIndex(0);

      determ::getInstance().initialize();
      xbitmap::getInstance().initialize();

      _thread_index = 0;

      // Add myself to the token queue.
      determ::getInstance().registerMaster(_thread_index, pid);
      _fence_enabled = false;
    } else {
      fprintf(stderr, "xrun reinitialized");
      ::abort();
    }
  }


  /// @brief Spawn a thread.
  static inline void * spawn(threadFunction * fn, void * arg) {
    // If system is not protected, we should open protection.
    if(!_protection_enabled) {
      openMemoryProtection();
      atomicBegin(true);
    }
      
    atomicEnd(false);

#ifdef LAZY_COMMIT 
    xmemory::finalcommit(true);
#endif
      
    // If fence is already enabled, then we should wait for token to proceed.
    if(_fence_enabled) {  
      waitToken();

      // In order to speedup the performance, we try to create as many children
      // as possible once. So we set the _fence_enabled to false now, then current
      // thread don't need to wait on token anymore.
      // Since other threads are either waiting on internal fence or waiting on the parent notification, 
      // it will be fine to do so.
      // When current thread are trying to wakeup the children threads, it will set 
      // _fence_enabled to true again.
      _fence_enabled = false;
      _children_threads_count = 0;
    }

    _children_threads_count++;
       
    void * ptr = xthread::spawn(fn, arg, _thread_index);

    // Start a new transaction
    atomicBegin(true);

    return ptr;
  }






}//end xrun
