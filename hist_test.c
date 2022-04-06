/* hist_test.c - fast histogrammer. */
/*
I want there to be NO barriers to using this code, so I am releasing it to the public domain.
But "public domain" does not have an internationally agreed-upon definition, so I use CC0:

Copyright 2022 Steven Ford http://geeky-boy.com and licensed
"public domain" style under
[CC0](http://creativecommons.org/publicdomain/zero/1.0/):
![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png "CC0")

To the extent possible under law, the contributors to this project have
waived all copyright and related or neighboring rights to this work.
In other words, you can use this code for any purpose without any
restrictions.  This work is published from: United States.  The project home
is https://github.com/fordsfords/hist

To contact me, Steve Ford, project owner, you can find my email address
at http://geeky-boy.com.  Can't see it?  Keep looking.
*/

#include "cprt.h"
#include <stdio.h>
#include <string.h>
#if ! defined(_WIN32)
  #include <stdlib.h>
  #include <unistd.h>
#endif

#include "cprt.h"
#include "rdtsc.h"
#include "hist.h"

double ts_2_ns(uint64_t ticks)
{
  double res = (double)ticks;
  res /= (double)rdtsc_ticks_per_sec;
  res *= 1000000000.0;
  return res;
}  /* ts_2_ns */


int main(int argc, char **argv)
{
  hist_t *hist = hist_create(500);
  hist_t *test_hist = hist_create(200);

  rdtsc_calibrate();
  uint64_t start_ts, end_ts;

  hist_init(hist);

  int i;
  RDTSC(start_ts);
  for (i = 0; i < 1000000; i++) {
    hist_input(test_hist, i % 128);
  }
  RDTSC(end_ts);
  double avg_hist_input_ns = ts_2_ns(end_ts - start_ts) / 1000000;

  RDTSC(start_ts);
  for (i = 0; i < 1000000; i++) {
    uint64_t dummy1, dummy2;
    RDTSC(dummy1);
    RDTSC(dummy2);

    hist_input(hist, dummy2 - dummy1);
  }
  RDTSC(end_ts);
  double avg_2ts_hist_input_ns = ts_2_ns(end_ts - start_ts) / 1000000;

  printf("avg hist_input() time=%f ns, avg 2ts+hist=%f\n", avg_hist_input_ns, avg_2ts_hist_input_ns);
  printf("min=%f ns, max=%f ns, avg=%f ns, ovf=%"PRIu32", rdtsc_ticks_per_sec=%"PRIu64"\n",
      ts_2_ns(hist->min_sample), ts_2_ns(hist->max_sample),
      ts_2_ns(hist->sample_sum / hist->num_samples), hist->overflows, rdtsc_ticks_per_sec);

  hist_print(hist);

  hist_delete(hist);
}  /* main */
