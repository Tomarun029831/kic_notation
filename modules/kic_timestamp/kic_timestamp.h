#pragma once
#include <stdint.h>

typedef union {
  struct {
    uint32_t millisecond : 10; // 0~999
    uint32_t second : 6;       // 0~59
    uint32_t hour_min : 11;    // 0~1159
    uint32_t day : 3;          // 0~6
    uint32_t is_PM : 1;        // 0:AM, 1:PM
    uint32_t is_invalid : 1;   // 0: vailed, 1:invailed
  } segments;
  uint32_t raw;
} KIC_Timestamp;

#define HALF_DAY_OFFSET (1200)
static inline KIC_Timestamp TIMESTAMP(uint32_t d, uint32_t t) {
  const char is_PM = (t >= HALF_DAY_OFFSET);
  return (KIC_Timestamp){
      .segments = {.millisecond = 0,
                   .second = 0,
                   .hour_min = is_PM ? (t - HALF_DAY_OFFSET) : t,
                   .day = d,
                   .is_PM = is_PM,
                   .is_invalid = 0}};
}
#define TIMESTAMP_RAW(d, t, pm, v)                                             \
  ((KIC_Timestamp){.segments = {.millisecond = 0,                              \
                                .second = 0,                                   \
                                .hour_min = (t),                               \
                                .day = (d),                                    \
                                .is_PM = (pm),                                 \
                                .is_invalid = (v)}})
