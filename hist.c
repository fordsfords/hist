/* hist.c - fast histogrammer. */
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
#include "hist.h"


hist_t *hist_create(int size)
{
  hist_t *hist = NULL;
  CPRT_ENULL(hist = (hist_t *)malloc(size * sizeof(hist_t)));

  CPRT_ENULL(hist->buckets = (uint64_t *)malloc(size * sizeof(uint64_t)));
  hist->size = size;

  hist_init(hist);

  return hist;
}

void hist_init(hist_t *hist)
{
  /* Re-initialize the data. */
  hist->min_sample = 999999999;
  hist->max_sample = 0;
  hist->sample_sum = 0;
  hist->overflows = 0;  /* Number of values above the last bucket. */
  hist->num_samples = 0;

  /* Init histogram (also makes sure it is mapped to physical memory. */
  int i;
  for (i = 0; i < hist->size; i++) {
    hist->buckets[i] = 0;
  }
}  /* hist_init */


void hist_input(hist_t *hist, uint64_t in_sample)
{
  hist->num_samples++;
  hist->sample_sum += in_sample;

  if (in_sample > hist->max_sample) {
    hist->max_sample = in_sample;
  }
  if (in_sample < hist->min_sample) {
    hist->min_sample = in_sample;
  }

  if (in_sample >= hist->size) {
    hist->overflows++;
  }
  else {
    hist->buckets[in_sample]++;
  }
}  /* hist_input */


void hist_print(hist_t *hist)
{
  int i;
  for (i = 0; i < hist->size; i++) {
    printf("%"PRIu64"\n", hist->buckets[i]);
  }
}  /* hist_print */


void hist_delete(hist_t *hist)
{
  free(hist->buckets);
  free(hist);
}  /* hist_delete */
