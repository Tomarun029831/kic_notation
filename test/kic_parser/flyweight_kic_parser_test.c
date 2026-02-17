#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>
#include <stdio.h>

static inline void kic_version_test() {
  const char *const kic_correct_version[2] = {
      "KIC:" KIC_VERSION ";/", "KIC:" KIC_VERSION ";01290;03000300;00900/"};
  for (const char *const *head = &kic_correct_version[0];
       head != kic_correct_version + 2; head++) {
    const unsigned char expected_compatible = check_kic_compatibility(*head);
    assert(expected_compatible == KIC_CONPATIBLE);
  }

  const char *const kic_incorrect_version[4] = {"KIC:V0;/", "AIC:V0;/",
                                                "KAC:V0;/", "KIA:V0;/"};
  for (const char *const *head = &kic_incorrect_version[0];
       head != kic_incorrect_version + 4; head++) {
    const unsigned char expected_incompatible = check_kic_compatibility(*head);
    assert(expected_incompatible == KIC_INCONPATIBLE);
  }
}

static inline void kic_syntax_test() {
  const char *kic_correct_syntax[3] = {
      "KIC:" KIC_VERSION ";01200;00010001;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;20700090011001330;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;/"};
  for (const char *const *head = &kic_correct_syntax[0];
       head != kic_correct_syntax + 3; head++) {
    const unsigned char expected_correct = check_kic_syntax(*head);
    assert(expected_correct == KIC_SYNTAX_CORRECT);
  }

  const char *kic_wrong_syntax[6] = {
      "KIC:" KIC_VERSION ";129GNUIS0030NOTUNIX.223;21023/",
      "KIC:" KIC_VERSION ";9000;90009000;11200;/",
      "KIC:" KIC_VERSION ";01200;0090;11200;/",
      "KIC:" KIC_VERSION ";01200;00900090;1120012009;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200;/",
      "KIC:" KIC_VERSION ";01437;01140334;008001200;10100;20700090011001330;"
      "390001200;41200;51200;61200;712001200/"};
  for (const char *const *head = &kic_wrong_syntax[0];
       head != kic_wrong_syntax + 6; head++) {
    const unsigned char expected_wrong = check_kic_syntax(*head);
    assert(expected_wrong == KIC_SYNTAX_ERROR);
  }
}

static inline void kic_get_timestamp_test() {}
static inline void kic_get_boardsize_test() {}
static inline void kic_get_schedules_test() {}
static inline void kic_get_day_of_week_test() {}
static inline void kic_get_time_in_schedule_test() {}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  // below here, these tests do NOT check syntax of array of chars.
  // So you must let the tests check array of chars which has correct kic syntax
  kic_get_timestamp_test();
  kic_get_boardsize_test();
  kic_get_schedules_test();
  kic_get_day_of_week_test();
  kic_get_time_in_schedule_test();

  puts("flyweight_kic_parser_test passed");
  return 0;
}
