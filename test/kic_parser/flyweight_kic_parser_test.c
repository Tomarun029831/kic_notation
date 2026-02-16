#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>
#include <stdio.h>

inline static void kic_version_test() {
  const char *const kic_correct_version[2] = {
      "KIC:" KIC_VERSION ";/", "KIC:" KIC_VERSION ";1290;03000300;00900/"};
  for (const char *const *head = &kic_correct_version[0];
       head != kic_correct_version + 2; head++) {
    const unsigned char expected_available = check_availability_of_kic(*head);
    assert(expected_available == KIC_AVAILABLE);
  }

  const char *const kic_incorrect_version[4] = {"KIC:V0;/", "AIC:V0;/",
                                                "KAC:V0;/", "KIA:V0;/"};
  for (const char *const *head = &kic_incorrect_version[0];
       head != kic_incorrect_version + 4; head++) {
    const unsigned char expected_unavailable = check_availability_of_kic(*head);
    assert(expected_unavailable == KIC_UNAVAILABLE);
  }
}

inline static void kic_syntax_test() {}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  puts("flyweight_kic_parser_test passed");
  return 0;
}
