// This file is part of MCF gthread.
// See LICENSE.TXT for licensing information.
// Copyleft 2022, LH_Mouse. All wrongs reserved.

#include "../src/cond.h"
#include "../src/clock.h"
#include <assert.h>
#include <stdio.h>
#include <windows.h>

static _MCF_cond cond;

int
main(void)
  {
    double now, delta;
    int64_t timeout;
    int r;

    now = _MCF_perf_counter();
    timeout = _MCF_utc_now() + 1100;  // absolute
    r = _MCF_cond_wait(&cond, NULL, NULL, 0, &timeout);
    assert(r == -1);
    delta = _MCF_perf_counter() - now;
    printf("delta = %.6f\n", delta);
    assert(delta >= 1000);
    assert(delta <= 1200);

    now = _MCF_perf_counter();
    timeout = -1100;  // relative
    r = _MCF_cond_wait(&cond, NULL, NULL, 0, &timeout);
    assert(r == -1);
    delta = _MCF_perf_counter() - now;
    printf("delta = %.6f\n", delta);
    assert(delta >= 1000);
    assert(delta <= 1200);
  }