/*
 * Copyright (c) 2026 Tomarun029831
 * SPDX-License-Identifier: MIT
 */
// IWYU pragma: private, include <kic_notation.h>
#pragma once
#include <stdint.h>

/**
 * @brief A packed 32-bit union representing a KIC timestamp.
 * * Manages high-precision time data (down to 1ms) using memory-efficient
 * bit-fields.
 */
typedef union {
  struct {
    uint32_t millisecond : 10; ///< 0-999 milliseconds
    uint32_t second : 6;       ///< 0-59 seconds
    uint32_t hour_min : 11;    ///< 0-1159 (HHMM format in 12-hour clock)
    uint32_t day : 3;          ///< 0-6 (Day of week)
    uint32_t is_PM : 1;        ///< 0: AM, 1: PM
    uint32_t is_invalid : 1;   ///< 0: Valid, 1: Invalid/Error flag
  } segments;
  uint32_t raw; ///< Raw 32-bit access for atomic comparison
} KIC_Timestamp;

#define HALF_DAY_OFFSET (1200)
/**
 * @brief Creates a KIC_Timestamp from day and time (HHMM).
 * * Automatically converts 24-hour format to 12-hour + AM/PM representation.
 * @param d Day of the week (0-6).
 * @param t Time in HHMM format (0-2359).
 * @return KIC_Timestamp The generated timestamp structure.
 */
static inline KIC_Timestamp TIMESTAMP(uint32_t d, uint32_t t) {
  const uint32_t is_PM = (t >= HALF_DAY_OFFSET);
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

/**
 * @brief Compares two timestamps for equality.
 * @return Non-zero (true) if identical, 0 (false) otherwise.
 */
static inline int32_t KIC_Timestamp_Compare(KIC_Timestamp ts1,
                                            KIC_Timestamp ts2) {
  return ts1.raw == ts2.raw; // HACK:
}

/** @name Timestamp Manipulation Functions */
void KIC_Timestamp_AddMs(KIC_Timestamp *ts, uint32_t ms);
/** @name Timestamp Manipulation Functions */
void KIC_Timestamp_AddSec(KIC_Timestamp *ts, uint32_t sec);
/** @name Timestamp Manipulation Functions */
void KIC_Timestamp_AddMin(KIC_Timestamp *ts, uint32_t min);
/** @name Timestamp Manipulation Functions */
void KIC_Timestamp_AddHour(KIC_Timestamp *ts, uint32_t hour);
/** @name Timestamp Manipulation Functions */
void KIC_Timestamp_AddDay(KIC_Timestamp *ts, uint32_t day);
