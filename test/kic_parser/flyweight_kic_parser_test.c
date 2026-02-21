/*
 * Copyright (c) 2026 Tomarun029831
 * SPDX-License-Identifier: MIT
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "kic_notation.h"
#ifdef __cplusplus
}
#endif

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
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

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static void
kic_boolean_test_iterator_impl(const char *const *array, const size_t len,
                               const unsigned char (*func)(const char *),
                               const unsigned char expected, const char *file,
                               const int line) {
  for (size_t i = 0; i < len; i++) {
    const unsigned char result = func(array[i]);
    if (result == expected)
      continue;
  }
}
#define kic_boolean_test_iterator(arr, len, func, exp)                         \
  kic_boolean_test_iterator_impl((arr), (len), (func), (exp), __FILE__,        \
                                 __LINE__)

static inline void kic_version_test() {
  const char *const kic_correct_version[] = {
      "KIC:" KIC_VERSION ";/", "KIC:" KIC_VERSION ";01290;03000300;00900/"};
  kic_boolean_test_iterator(kic_correct_version,
                            ARRAY_SIZE(kic_correct_version),
                            check_kic_compatibility, KIC_COMPATIBLE);
  const char *const kic_incorrect_version[] = {"KIC:V0;/", "AIC:V0;/",
                                               "KAC:V0;/", "KIA:V0;/"};
  kic_boolean_test_iterator(kic_incorrect_version,
                            ARRAY_SIZE(kic_incorrect_version),
                            check_kic_compatibility, KIC_INCOMPATIBLE);
}

static inline void kic_syntax_test() {
  const char *kic_correct_syntax[] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  kic_boolean_test_iterator(kic_correct_syntax, ARRAY_SIZE(kic_correct_syntax),
                            check_kic_syntax, KIC_SYNTAX_CORRECT);
  const char *kic_wrong_syntax[] = {
      "KIC:" KIC_VERSION ";129GNUIS0030NOTUNIX.223;21023/",
      "KIC:" KIC_VERSION ";9000;90009000;11200;/",
      "KIC:" KIC_VERSION ";01200;0090;11200;/",
      "KIC:" KIC_VERSION ";01200;00900090;1120012009;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200/"};
  kic_boolean_test_iterator(kic_wrong_syntax, ARRAY_SIZE(kic_wrong_syntax),
                            check_kic_syntax, KIC_SYNTAX_ERROR);
}

static inline void kic_get_timestamp_test() {
  const char *kic_correct_syntax[] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const KIC_Timestamp expected_timestamps[] = {
      TIMESTAMP(0, 1200), TIMESTAMP(1, 1437), TIMESTAMP(2, 1437)};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + ARRAY_SIZE(kic_correct_syntax); head++) {
    KIC_Timestamp result = get_kic_timestamp(*head);
    ptrdiff_t idx = head - kic_correct_syntax;
    assert(result.raw == expected_timestamps[idx].raw);
  }
}

static inline void kic_get_boardsize_test() {
  const char *kic_correct_syntax[] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const BoardSize expected_boardsizes[] = {BOARDSIZE(1, 1), BOARDSIZE(114, 334),
                                           BOARDSIZE(114, 334)};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + ARRAY_SIZE(kic_correct_syntax); head++) {
    const BoardSize result = get_kic_boardsize(*head);
    ptrdiff_t idx = head - kic_correct_syntax;
    const BoardSize expected_boardsize = expected_boardsizes[(idx)];
    assert(result.height_cm == expected_boardsize.height_cm &&
           result.width_cm == expected_boardsize.width_cm);
  }
}

static inline void kic_find_schedules_test() {
  const char *kic_correct_syntax[] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const char *expected_ptr[] = {KIC_SCHEDULE_NOT_FOUND,
                                &kic_correct_syntax[1][22],
                                &kic_correct_syntax[2][38]};
  const uint32_t args[] = {0, 0, 2};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + ARRAY_SIZE(kic_correct_syntax); head++) {
    ptrdiff_t idx = head - kic_correct_syntax;
    const char *result = find_kic_schedule(*head, args[idx]);
    assert(expected_ptr[idx] == result);
  }
}

static inline void kic_find_time_in_schedule_test() {
  const char *kic_correct_syntax[] = {
      "KIC:" KIC_VERSION ";01200;00010001;11154;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const KIC_Timestamp expected_timestamps[] = {
      TIMESTAMP(1, 1154), KIC_TIME_NOT_FOUND, TIMESTAMP(2, 1100)};
  const uint32_t args_to_find_schedule[] = {1, 0, 2};
  const size_t args_to_get_time[] = {0, 3, 2};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + ARRAY_SIZE(kic_correct_syntax); head++) {
    ptrdiff_t idx = head - kic_correct_syntax;
    const char *schedule = find_kic_schedule(*head, args_to_find_schedule[idx]);
    const KIC_Timestamp result =
        find_kic_time_in_schedule(schedule, args_to_get_time[idx]);
    assert(expected_timestamps[idx].raw == result.raw);
  }
}

TEST_MAIN {
  kic_version_test();
  kic_syntax_test();
  kic_get_timestamp_test();
  kic_get_boardsize_test();
  kic_find_schedules_test();
  kic_find_time_in_schedule_test();

#ifndef ARDUINO
  puts("flyweight_kic_parser_test passed");
#endif

  TEST_EXIT();
}

#ifdef ARDUINO
void loop() {}
#endif
