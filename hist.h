/* hist.h - fast histogrammer. */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/
 * 
 * To the extent possible under law, Steven Ford has waived all copyright
 * and related or neighboring rights to this work. In other words, you can 
 * use this code for any purpose without any restrictions.
 * This work is published from: United States.
 * Project home: https://github.com/fordsfords/hist
 */

#include <inttypes.h>

#ifndef HIST_H
#define HIST_H

/* Histogram. */
struct hist_s {
  uint64_t *buckets;
  uint64_t min_raw;
  uint64_t max_raw;
  uint64_t raw_sum;
  int size;  /* Number of buckets. */
  int overflows;  /* Number of values above the last bucket. */
  int num_samples;
};
typedef struct hist_s hist_t;


hist_t *hist_create(int size);
void hist_init(hist_t *hist);  /* Clear a histogram. */
void hist_input(hist_t *hist, uint64_t raw_sample, int bucket);
void hist_print(hist_t *hist);  /* Print to stdout the buckets. */
void hist_delete(hist_t *hist);

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* HIST_H */
