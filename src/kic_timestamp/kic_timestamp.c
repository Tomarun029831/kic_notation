/*
 * Copyright (c) 2026 Tomarun029831
 * SPDX-License-Identifier: MIT
 */
#include "kic_notation/internal/kic_timestamp.h"

#define GET_MIN(hm) ((hm) % 100)
#define GET_HOUR(hm) ((hm) / 100)

void KIC_Timestamp_AddDay(KIC_Timestamp *ts, uint32_t day) {
  if (!day)
    return;
  ts->segments.day = (ts->segments.day + (day % 7)) % 7;
}

void KIC_Timestamp_AddHour(KIC_Timestamp *ts, uint32_t hour) {
  if (!hour)
    return;
  uint32_t hm = ts->segments.hour_min;
  uint64_t h = (uint64_t)(hm / 100) + hour;

  ts->segments.hour_min = ((uint32_t)(h % 12) * 100) + (hm % 100);

  uint32_t blocks = (uint32_t)(h / 12);
  if (blocks > 0) {
    uint32_t flips = blocks & 1;
    uint32_t days_to_add = (blocks >> 1) + ((ts->segments.is_PM + flips) >> 1);
    ts->segments.is_PM ^= flips;

    if (days_to_add > 0) {
      ts->segments.day = (ts->segments.day + (days_to_add % 7)) % 7;
    }
  }
}

void KIC_Timestamp_AddMin(KIC_Timestamp *ts, uint32_t min) {
  if (!min)
    return;

  uint32_t hm = ts->segments.hour_min;
  uint64_t m = (uint64_t)(hm % 100) + min;

  ts->segments.hour_min = ((hm / 100) * 100) + (uint32_t)(m % 60);

  uint32_t add_h = (uint32_t)(m / 60);
  if (add_h > 0) {
    KIC_Timestamp_AddHour(ts, add_h);
  }
}

void KIC_Timestamp_AddSec(KIC_Timestamp *ts, uint32_t sec) {
  if (!sec)
    return;

  uint64_t s = (uint64_t)ts->segments.second + sec;
  ts->segments.second = (uint32_t)(s % 60);

  uint32_t add_m = (uint32_t)(s / 60);
  if (add_m > 0) {
    KIC_Timestamp_AddMin(ts, add_m);
  }
}

void KIC_Timestamp_AddMs(KIC_Timestamp *ts, uint32_t ms) {
  if (!ms)
    return;

  uint64_t msec = (uint64_t)ts->segments.millisecond + ms;
  ts->segments.millisecond = (uint32_t)(msec % 1000);

  uint32_t add_s = (uint32_t)(msec / 1000);
  if (add_s > 0) {
    KIC_Timestamp_AddSec(ts, add_s);
  }
}
