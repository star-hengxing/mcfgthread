// This file is part of MCF gthread.
// See LICENSE.TXT for licensing information.
// Copyleft 2022, LH_Mouse. All wrongs reserved.

#include "../src/gthr.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>

static __gthread_key_t key;
static __gthread_t thrd;
static int dso_1, dso_2;

static void*
thread_proc(void* param)
  {
    (void) param;
    void* p;
    int r;

    p = __gthread_getspecific(key);
    assert(p == NULL);

    r = __gthread_setspecific(key, &dso_2);
    assert(r == 0);

    p = __gthread_getspecific(key);
    assert(p == &dso_2);

    Sleep(1000);
    printf("thread %d quitting\n", (int) GetCurrentThreadId());
    return NULL;
  }

int
main(void)
  {
    void* p;
    int r;

    r = __gthread_key_create(&key, NULL);
    assert(r == 0);
    assert(key);

    p = __gthread_getspecific(key);
    assert(p == NULL);

    r = __gthread_setspecific(key, &dso_1);
    assert(r == 0);

    p = __gthread_getspecific(key);
    assert(p == &dso_1);

    r = __gthread_create(&thrd, thread_proc, NULL);
    assert(r == 0);
    assert(thrd);

    printf("main waiting\n");
    WaitForSingleObject(thrd->__handle, INFINITE);
    printf("main wait finished\n");

    p = __gthread_getspecific(key);
    assert(p == &dso_1);
  }