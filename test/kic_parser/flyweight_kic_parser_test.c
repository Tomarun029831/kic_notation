#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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
    fprintf(stderr, "\n[ASSERTION FAILED]\n");
    fprintf(stderr, "  File    : %s\n", file);
    fprintf(stderr, "  Line    : %d\n", line);
    fprintf(stderr, "  Index   : %zu\n", i);
    fprintf(stderr, "  Input   : \"%s\"\n", array[i]);
    fprintf(stderr, "  Result  : %u (expected %u)\n\n", result, expected);
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
                            check_kic_compatibility, KIC_CONPATIBLE);
  const char *const kic_incorrect_version[] = {"KIC:V0;/", "AIC:V0;/",
                                               "KAC:V0;/", "KIA:V0;/"};
  kic_boolean_test_iterator(kic_incorrect_version,
                            ARRAY_SIZE(kic_incorrect_version),
                            check_kic_compatibility, KIC_INCONPATIBLE);
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
  const Timestamp expected_timestamps[] = {
      TIMESTAMP(0, 1200), TIMESTAMP(1, 1437), TIMESTAMP(2, 1437)};

  for (const char *const *head = kic_correct_syntax;
       head != kic_correct_syntax + ARRAY_SIZE(kic_correct_syntax); head++) {
    Timestamp result = get_kic_timestamp(*head);
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

static inline void kic_find_schedules_test() {}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  // The following tests do not validate the syntax of character arrays.
  // Please ensure you include tests that verify character arrays using correct
  // KIC syntax.
  kic_get_timestamp_test();
  kic_get_boardsize_test();
  kic_find_schedules_test();

  puts("flyweight_kic_parser_test passed");
  return 0;
}
