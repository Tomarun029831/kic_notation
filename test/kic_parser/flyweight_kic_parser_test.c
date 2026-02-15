#include "kic_parser/flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <assert.h>

inline static void kic_version_test() {
  const char *const kic_correct_version = "KIC:" KIC_VERSION ";/";
  const char *const kic_incorrect_version = "KIC:V0;/";
  const unsigned char expected_available =
      check_availability_of_kic(kic_correct_version);
  assert(expected_available == KIC_AVAILABLE);
  const unsigned char expected_unavailable =
      check_availability_of_kic(kic_incorrect_version);
  assert(expected_unavailable == KIC_UNAVAILABLE);
}

inline static void kic_syntax_test() {}

int main(void) {
  kic_version_test();
  kic_syntax_test();
  return 0;
}
