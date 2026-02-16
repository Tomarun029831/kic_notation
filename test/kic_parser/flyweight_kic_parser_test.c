#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>
#include <stdio.h>

inline static void kic_version_test() {
  const char *const kic_correct_version[2] = {
      "KIC:" KIC_VERSION ";/", "KIC:" KIC_VERSION ";1290;03000300;00900/"};
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

inline static void kic_syntax_test() {
  const char *kic_correct_syntax =
      "KIC:" KIC_VERSION ";1290;03000300;009001223;21023/";
  const unsigned char expected_correct = check_kic_syntax(kic_correct_syntax);
  assert(expected_correct == KIC_SYNTAX_CORRECT);

  const char *kic_wrong_syntax =
      "KIC:" KIC_VERSION ";129GNUIS0030NOTUNIX.223;21023/";
  const unsigned char expected_wrong = check_kic_syntax(kic_wrong_syntax);
  assert(expected_wrong == KIC_SYNTAX_ERROR);
}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  puts("flyweight_kic_parser_test passed");
  return 0;
}
