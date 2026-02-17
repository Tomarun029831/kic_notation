#include "flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"

#define KIC_HEADER "KIC:" KIC_VERSION
#define KIC_SEPARATOR ';'
#define KIC_TERMINATOR '/'

unsigned char check_kic_compatibility(const char *string) {
  const char *kic_header = KIC_HEADER;
  for (;; string++, kic_header++) {
    if (*kic_header == ((char)'\0') && *string == KIC_SEPARATOR)
      return KIC_CONPATIBLE;
    else if (*string != *kic_header)
      return KIC_INCONPATIBLE;
  }
}

#define KIC_TIMESTAMP_LEN 5
#define KIC_BOARD_LEN 8
#define KIC_SCHEDULE_PAYLOAD_LEN 4
#define MAX_KIC_SCHEDULE_AMOUNT ((unsigned char)0x7F)
#define IS_NOT_DIGIT_CHAR(char) (!('0' <= char &&char <= '9'))

unsigned char check_kic_syntax(const char *string) {
  // check version
  const char *kic_header = KIC_HEADER;
  for (;; string++, kic_header++) {
    if (*kic_header == ((char)'\0') && *string++ == KIC_SEPARATOR)
      break;
    else if (*string != *kic_header)
      return KIC_SYNTAX_ERROR;
  }

  // check timestamp
  for (const char *expected_end_of_timestamp = string + KIC_TIMESTAMP_LEN;;
       string++) {
    if (string == expected_end_of_timestamp && *string++ == KIC_SEPARATOR)
      break;
    else if (*string == '\0' || IS_NOT_DIGIT_CHAR(*string))
      return KIC_SYNTAX_ERROR;
  }

  // check board
  for (const char *expected_end_of_boardsize = string + KIC_BOARD_LEN;;
       string++) {
    if (string == expected_end_of_boardsize && *string++ == KIC_SEPARATOR)
      break;
    else if (*string == '\0' || IS_NOT_DIGIT_CHAR(*string))
      return KIC_SYNTAX_ERROR;
  }

  // check schedules
  for (unsigned char flags_to_count_separator = ((unsigned char)0),
                     len_of_schedule = ((unsigned char)0);
       ; string++) {
    if (*string == KIC_TERMINATOR && len_of_schedule == 0)
      break;
    else if (*string == KIC_SEPARATOR) {
      flags_to_count_separator = (flags_to_count_separator << 1) + 1;
      if ((len_of_schedule % KIC_SCHEDULE_PAYLOAD_LEN) != 1 ||
          flags_to_count_separator > MAX_KIC_SCHEDULE_AMOUNT)
        return KIC_SYNTAX_ERROR; // HACK:
      len_of_schedule = 0;
      continue;
    } else if (*string == '\0' || IS_NOT_DIGIT_CHAR(*string))
      return KIC_SYNTAX_ERROR;
    ++len_of_schedule;
  }
  return KIC_SYNTAX_CORRECT;
}
