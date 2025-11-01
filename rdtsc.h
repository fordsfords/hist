/* rdtsc.h - test program for rdtsc
# For documentation, see https://github.com/fordsfords/rdtsc */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/
 * 
 * To the extent possible under law, Steven Ford has waived all copyright
 * and related or neighboring rights to this work. In other words, you can 
 * use this code for any purpose without any restrictions.
 * This work is published from: United States.
 * Project home: https://github.com/fordsfords/rdtsc
 */

#ifndef NSTM_H
#define NSTM_H

#ifdef __cplusplus
extern "C" {
#endif


#define RDTSC(rdtsc_val_) do { \
  uint32_t rdtsc_hi_, rdtsc_lo_; \
  __asm__ volatile ("rdtsc" : "=a" (rdtsc_lo_), "=d" (rdtsc_hi_)); \
  rdtsc_val_ = (uint64_t)rdtsc_hi_ << 32 | rdtsc_lo_; \
} while (0)


/* Global (set by rdtsc_calibrate()). */
extern uint64_t rdtsc_ticks_per_sec;

void rdtsc_calibrate();

#ifdef __cplusplus
}
#endif

#endif  /* NSTM_H */
