#include "flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"

#define KIC_HEADER "KIC:" KIC_VERSION
#define KIC_SEPARATOR ';'
#define KIC_TERMINATOR '/'

unsigned char check_kic_compatibility(const char *string) {
  const char *kic_header = KIC_HEADER;
  for (;; string++) {
    if (*kic_header == ((char)'\0') && *string == KIC_SEPARATOR)
      break; // pass first test
    if (*string != *kic_header++)
      return KIC_INCONPATIBLE;
  }

  for (;; string++) {
    if (*string == KIC_TERMINATOR)
      break; // pass second test
    if (*string == ((char)'\0'))
      return KIC_INCONPATIBLE;
  }

  return KIC_CONPATIBLE;
}
