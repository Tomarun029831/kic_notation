/*
 * Copyright (c) 2026 Tomarun029831
 * SPDX-License-Identifier: MIT
 */
#include "kic_notation/internal/kic_timestamp.h"

#define GET_MIN(hm) ((hm) % 100)
#define GET_HOUR(hm) ((hm) / 100)

void KIC_Timestamp_AddDay(KIC_Timestamp *ts, uint32_t day) {
  ts->segments.day = (ts->segments.day + day) % 7;
}

void KIC_Timestamp_AddHour(KIC_Timestamp *ts, uint32_t hour) {
  uint32_t h = GET_HOUR(ts->segments.hour_min) + hour;
  uint32_t m = GET_MIN(ts->segments.hour_min);

  uint32_t total_12h_blocks = h / 12;
  ts->segments.hour_min = ((h % 12) * 100) + m;

  if (total_12h_blocks > 0) {
    if (total_12h_blocks % 2 != 0) {
      if (ts->segments.is_PM) {
        ts->segments.is_PM = 0;
        KIC_Timestamp_AddDay(ts, 1);
      } else {
        ts->segments.is_PM = 1;
      }
    }
    if (total_12h_blocks / 2 > 0) {
      KIC_Timestamp_AddDay(ts, total_12h_blocks / 2);
    }
  }
}

void KIC_Timestamp_AddMin(KIC_Timestamp *ts, uint32_t min) {
  uint32_t h = GET_HOUR(ts->segments.hour_min);
  uint32_t m = GET_MIN(ts->segments.hour_min) + min;

  ts->segments.hour_min = (h * 100) + (m % 60);
  if (m / 60 > 0) {
    KIC_Timestamp_AddHour(ts, m / 60);
  }
}

void KIC_Timestamp_AddSec(KIC_Timestamp *ts, uint32_t sec) {
  uint32_t s = ts->segments.second + sec;
  ts->segments.second = s % 60;
  if (s / 60 > 0) {
    KIC_Timestamp_AddMin(ts, s / 60);
  }
}

void KIC_Timestamp_AddMs(KIC_Timestamp *ts, uint32_t ms) {
  uint32_t msec = ts->segments.millisecond + ms;
  ts->segments.millisecond = msec % 1000;
  if (msec / 1000 > 0) {
    KIC_Timestamp_AddSec(ts, msec / 1000);
  }
}
