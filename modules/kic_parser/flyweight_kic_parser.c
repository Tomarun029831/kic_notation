#include "flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"

#define KIC_HEADER "KIC:" KIC_VERSION
#define KIC_SEPARATOR ';'
#define KIC_TERMINATOR '/'

unsigned char check_kic_compatibility(const char *string) {
  const char *kic_header = KIC_HEADER;
  for (;; string++) {
    if (*kic_header == ((char)'\0') && *string == KIC_SEPARATOR)
      return KIC_CONPATIBLE;
    if (*string != *kic_header++)
      return KIC_INCONPATIBLE;
  }
}

unsigned char check_kic_syntax(const char *string) {}
