/* This file is part of MCF Gthread.
 * See LICENSE.TXT for licensing information.
 * Copyleft 2022, LH_Mouse. All wrongs reserved.  */

#include "../mcfgthread/cxx11.hpp"
#include "../mcfgthread/sem.h"
#include <assert.h>
#include <stdio.h>
#include <vector>

#ifdef TEST_STD
#  include <mutex>
#  include <thread>
#  include <chrono>
namespace NS = std;
#else
namespace NS = ::_MCF;
#endif

constexpr std::size_t NTHREADS = 64U;
static std::vector<NS::thread> threads(NTHREADS);
static NS::mutex mutex;
static ::_MCF_sem start = __MCF_SEM_INIT(NTHREADS);
static int resource = 0;

static
void
thread_proc()
  {
    ::_MCF_sem_wait(&start, nullptr);

    for(;;) {
      NS::unique_lock<NS::mutex> lock(mutex, NS::try_to_lock);
      if(lock) {
        // Add a resource.
        int old = resource;
        NS::this_thread::sleep_for(NS::chrono::milliseconds(10));
        resource = old + 1;
        break;
      }
      else
        NS::this_thread::sleep_for(NS::chrono::milliseconds(10));
    }

    ::printf("thread %d quitting\n", (int) ::_MCF_thread_self_tid());
  }

int
main(void)
  {
    for(auto& thr : threads)
      thr = NS::thread(thread_proc);

    ::printf("main waiting\n");
    ::_MCF_sem_signal_some(&start, NTHREADS);

    for(auto& thr : threads)
      thr.join();

    assert(resource == NTHREADS);
  }
