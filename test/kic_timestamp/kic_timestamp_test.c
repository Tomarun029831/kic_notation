/*
 * Copyright (c) 2026 Tomarun029831
 * SPDX-License-Identifier: MIT
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "kic_timestamp/kic_timestamp.h"
#ifdef __cplusplus
}
#endif

#include <assert.h>

#ifdef ARDUINO
#include <Arduino.h>
#define TEST_MAIN void setup()
#define TEST_EXIT()                                                            \
  do {                                                                         \
  } while (0)
#else
#include <stdio.h>
#define TEST_MAIN int main(void)
#define TEST_EXIT() return 0
#endif

TEST_MAIN {
  {
    KIC_Timestamp ts = TIMESTAMP(0, 1030);
    ts.segments.second = 59;
    KIC_Timestamp_AddSec(&ts, 1);
    assert(ts.segments.second == 0);
    assert(ts.segments.hour_min == 1031);
  }

  {
    KIC_Timestamp ts = TIMESTAMP(0, 1059);
    KIC_Timestamp_AddMin(&ts, 1);
    assert(ts.segments.hour_min == 1100);
    assert(ts.segments.is_PM == 0);
  }

  {
    KIC_Timestamp ts = TIMESTAMP(0, 1159);
    ts.segments.is_PM = 0;
    KIC_Timestamp_AddMin(&ts, 1);
    assert(ts.segments.hour_min == 0);
    assert(ts.segments.is_PM == 1);
  }

  {
    KIC_Timestamp ts = TIMESTAMP(0, 1159);
    ts.segments.is_PM = 1;
    KIC_Timestamp_AddMin(&ts, 1);
    assert(ts.segments.hour_min == 0);
    assert(ts.segments.is_PM == 0);
    assert(ts.segments.day == 1);
  }

  {
    KIC_Timestamp ts = TIMESTAMP(1, 550);
    KIC_Timestamp_AddMs(&ts, 120000);
    assert(ts.segments.hour_min == 552);
    assert(ts.segments.millisecond == 0);
  }

  {
    KIC_Timestamp ts = TIMESTAMP(6, 1159);
    ts.segments.is_PM = 1;
    KIC_Timestamp_AddHour(&ts, 1);
    assert(ts.segments.day == 0);
  }

#ifndef ARDUINO
  puts("kic_timestamp_test passed");
#endif
  TEST_EXIT();
}

#ifdef ARDUINO
void loop() {}
#endif
