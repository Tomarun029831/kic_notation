#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static void kic_boolean_test_iterator(const char *const *array_to_pass_test,
                                      const size_t len_of_array,
                                      const unsigned char (*func)(const char *),
                                      const unsigned char expected_result) {
  for (const char *const *head = &array_to_pass_test[0];
       head != array_to_pass_test + len_of_array; head++) {
    const unsigned char result = func(*head);
    assert(result == expected_result);
  }
}

static inline void kic_version_test() {
  const char *const kic_correct_version[2] = {
      "KIC:" KIC_VERSION ";/", "KIC:" KIC_VERSION ";01290;03000300;00900/"};
  kic_boolean_test_iterator(kic_correct_version, 2, check_kic_compatibility,
                            KIC_CONPATIBLE);
  const char *const kic_incorrect_version[4] = {"KIC:V0;/", "AIC:V0;/",
                                                "KAC:V0;/", "KIA:V0;/"};
  kic_boolean_test_iterator(kic_incorrect_version, 4, check_kic_compatibility,
                            KIC_INCONPATIBLE);
}

static inline void kic_syntax_test() {
  const char *kic_correct_syntax[3] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  kic_boolean_test_iterator(kic_correct_syntax, 3, check_kic_syntax,
                            KIC_SYNTAX_CORRECT);
  const char *kic_wrong_syntax[6] = {
      "KIC:" KIC_VERSION ";129GNUIS0030NOTUNIX.223;21023/",
      "KIC:" KIC_VERSION ";9000;90009000;11200;/",
      "KIC:" KIC_VERSION ";01200;0090;11200;/",
      "KIC:" KIC_VERSION ";01200;00900090;1120012009;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200/"};
  kic_boolean_test_iterator(kic_wrong_syntax, 6, check_kic_syntax,
                            KIC_SYNTAX_ERROR);
}

static inline void kic_get_timestamp_test() {
  const char *kic_correct_syntax[3] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const unsigned int expected_timestamps[3] = {
      TIMESTAMP(0, 1200), TIMESTAMP(1, 1437), TIMESTAMP(2, 1437)};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + 3; head++) {
    unsigned int result = get_kic_timestamp(*head);
    assert(result == expected_timestamps[(head - kic_correct_syntax)]);
  }
}

static inline void kic_get_boardsize_test() {
  const char *kic_correct_syntax[3] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";11437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";21437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  const BoardSize expected_timestamps[3] = {
      BOARDSIZE(1, 1), BOARDSIZE(114, 334), BOARDSIZE(114, 334)};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + 3; head++) {
    const BoardSize result = get_kic_boardsize(*head);
    assert(result.height_cm ==
               expected_timestamps[(head - kic_correct_syntax)].height_cm &&
           result.width_cm ==
               expected_timestamps[(head - kic_correct_syntax)].width_cm);
  }
}

static inline void kic_get_schedules_test() {}
static inline void kic_get_day_of_week_in_schedule_test() {}
static inline void kic_get_time_in_schedule_test() {}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  // below here, these tests do NOT check syntax of array of chars.
  // So you must let the tests check array of chars which has correct kic syntax
  kic_get_timestamp_test();
  kic_get_boardsize_test();
  kic_get_schedules_test();
  kic_get_day_of_week_in_schedule_test();
  kic_get_time_in_schedule_test();

  puts("flyweight_kic_parser_test passed");
  return 0;
}
