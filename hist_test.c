/* hist_test.c - fast histogrammer. */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/
 * 
 * To the extent possible under law, Steven Ford has waived all copyright
 * and related or neighboring rights to this work. In other words, you can 
 * use this code for any purpose without any restrictions.
 * This work is published from: United States.
 * Project home: https://github.com/fordsfords/hist
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if ! defined(_WIN32)
  #include <stdlib.h>
  #include <unistd.h>
#endif

#include "rdtsc.h"
#include "hist.h"

double ticks2ns(double ticks)
{
  double res = (double)ticks;
  res /= (double)rdtsc_ticks_per_sec;
  res *= 1000000000.0;
  return res;
}  /* ticks2ns */


int main(int argc, char **argv)
{
  hist_t *hist = hist_create(512);
  if (hist == NULL) {
    fprintf(stderr, "hist_create returned null (malloc failure)\n");
    exit(1);
  }

  rdtsc_calibrate();
  uint64_t start_ticks, end_ticks;

  /* Check the average execution time for hist_input(). */
  int i;
  RDTSC(start_ticks);
  for (i = 0; i < (1024 * 1024); i++) {
    hist_input(hist, i, i / 2048);  /* Visits every bucket 1..511. */
  }
  RDTSC(end_ticks);
  double avg_hist_in_ns = ticks2ns(end_ticks - start_ticks) / (1024.0 * 1024.0);

  /* Clear the histogram. */
  hist_init(hist);

  /* Check the average execution time for two timestamps and a hist_input()
   * and histogram the times between two "rdtsc" calls. Note that the
   * bucket number is ticks divided by 4. */
  RDTSC(start_ticks);
  for (i = 0; i < 1000000; i++) {
    uint64_t rdtsc1, rdtsc2;
    RDTSC(rdtsc1);
    /* You can put code in here that you want to measure. */
    RDTSC(rdtsc2);

    hist_input(hist, (rdtsc2 - rdtsc1), (rdtsc2 - rdtsc1) / 4);
  }
  RDTSC(end_ticks);
  double avg_2ticks_hist_in_ns = ticks2ns(end_ticks - start_ticks) / 1000000.0;

  printf("avg hist=%f ns, avg 2 ticks+hist=%f, ticks per sec=%"PRIu64"\n",
      avg_hist_in_ns, avg_2ticks_hist_in_ns, rdtsc_ticks_per_sec);

  printf("histogram of 2 rdtsc: min=%f ns, max=%f ns, avg=%f ns, ovf=%d, ns per bucket=%f\n",
      ticks2ns(hist->min_raw), ticks2ns(hist->max_raw),
      ticks2ns((double)hist->raw_sum / (double)hist->num_samples), hist->overflows, ticks2ns(4));
  hist_print(hist);

  hist_delete(hist);

  return 0;
}  /* main */
