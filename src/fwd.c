/* This file is part of MCF Gthread.
 * See LICENSE.TXT for licensing information.
 * Copyleft 2022, LH_Mouse. All wrongs reserved.  */

#include "precompiled.i"
#define __MCF_FWD_IMPORT  __MCF_DLLEXPORT
#define __MCF_FWD_INLINE  __MCF_DLLEXPORT
#include "fwd.h"
#include "xglobals.i"

__MCF_DLLEXPORT
void
__MCF_runtime_failure(const char* where)
  {
    /* `where` can be examined with a debugger.  */
    __asm__ volatile ("" : : "m"(where) : "memory");
    __builtin_trap();
  }

__MCF_DLLEXPORT
uint32_t
_MCF_get_win32_error(void)
  {
    return GetLastError();
  }
