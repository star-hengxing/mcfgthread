/* This file is part of MCF Gthread.
 * See LICENSE.TXT for licensing information.
 * Copyleft 2022, LH_Mouse. All wrongs reserved.  */

#include "precompiled.i"
#define __MCF_DECLSPEC_GTHR_AUX(...)  __MCF_DLLEXPORT
#include "gthr_aux.h"

__MCF_DLLEXPORT
int64_t
__MCF_gthr_timeout_from_timespec(const struct timespec* abs_time)
  {
    double value = 0.0009999;
    value += (double) abs_time->tv_nsec * 0.000001;
    value += (double) abs_time->tv_sec * 1000;

    /* Clamp the timestamp.  */
    value = __builtin_fmax(value, 0);
    value = __builtin_fmin(value, 0x1p63 - 0x1p10);
    return (int64_t) value;
  }

__MCF_DLLEXPORT
intptr_t
__MCF_gthr_mutex_unlock_callback(intptr_t arg)
  {
    _MCF_mutex* mtx = (_MCF_mutex*) arg;

    /* Unlock the mutex. The return value is unused.  */
    _MCF_mutex_unlock(mtx);
    return 0;
  }

__MCF_DLLEXPORT
void
__MCF_gthr_mutex_relock_callback(intptr_t arg, intptr_t unlocked)
  {
    _MCF_mutex* mtx = (_MCF_mutex*) arg;

    /* Relock the mutex. The `unlocked` value is unused.  */
    (void) unlocked;
    _MCF_mutex_lock(mtx, NULL);
  }

__MCF_DLLEXPORT
intptr_t
__MCF_gthr_recursive_mutex_unlock_callback(intptr_t arg)
  {
    __MCF_gthr_recursive_mutex* rmtx = (__MCF_gthr_recursive_mutex*) arg;

    /* Clear the depth counter and return it.  */
    intptr_t unlocked = rmtx->__depth;
    rmtx->__depth = 0;
    _MCF_atomic_store_32_rlx(&(rmtx->__owner), 0);
    _MCF_mutex_unlock(&(rmtx->__mutex));

    __MCF_ASSERT(unlocked > 0);
    return unlocked;
  }

__MCF_DLLEXPORT
void
__MCF_gthr_recursive_mutex_relock_callback(intptr_t arg, intptr_t unlocked)
  {
    __MCF_ASSERT(unlocked > 0);
    __MCF_gthr_recursive_mutex* rmtx = (__MCF_gthr_recursive_mutex*) arg;

    /* Relock the mutex and restore the depth counter.  */
    _MCF_mutex_lock(&(rmtx->__mutex), NULL);
    __MCF_ASSERT(rmtx->__owner == 0);
    _MCF_atomic_store_32_rlx(&(rmtx->__owner), (int32_t) _MCF_thread_self_tid());
    rmtx->__depth = (int32_t) unlocked;
  }

__MCF_DLLEXPORT
void
__MCF_gthr_thread_thunk_v2(_MCF_thread* thrd)
  {
    __MCF_gthr_thread_record* rec = _MCF_thread_get_data(thrd);

    /* Invoke the user-defined procedure and save its result in the record.  */
    rec->__result = rec->__proc(rec->__arg);
  }
