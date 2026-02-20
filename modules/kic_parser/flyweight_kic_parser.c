#include "flyweight_kic_parser.h"
#include "kic_parser/kic_parser_specifications.h"
#include <stddef.h>

#define KIC_HEADER "KIC:" KIC_VERSION
#define KIC_SEPARATOR ';'
#define KIC_TERMINATOR '/'

const unsigned char check_kic_compatibility(const char *string) {
  const char *kic_header = KIC_HEADER;
  for (;; string++, kic_header++) {
    if (*kic_header == ((char)'\0') && *string == KIC_SEPARATOR)
      return KIC_COMPATIBLE;
    else if (*string != *kic_header)
      return KIC_INCOMPATIBLE;
  }
}

#define KIC_TIMESTAMP_LEN 5
#define KIC_BOARD_LEN 8
#define KIC_SCHEDULE_PAYLOAD_LEN 4
#define MAX_KIC_SCHEDULE_AMOUNT ((unsigned char)7)
#define IS_NOT_DIGIT_CHAR(char) (!('0' <= char &&char <= '9'))

const unsigned char check_kic_syntax(const char *string) {
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
      ++flags_to_count_separator;
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

const KIC_Timestamp get_kic_timestamp(const char *string) {
#define OFFSET_TO_KIC_TIMESTAMP (sizeof(KIC_HEADER))
  const unsigned int raw_hour =
      (string[OFFSET_TO_KIC_TIMESTAMP + 1] - '0') * 1000 +
      (string[OFFSET_TO_KIC_TIMESTAMP + 2] - '0') * 100;
  const unsigned char is_PM = (raw_hour >= HALF_DAY_OFFSET); // 1bit
  const unsigned int hour = is_PM ? (raw_hour - HALF_DAY_OFFSET) : raw_hour;

  const KIC_Timestamp time = // PERF:
      TIMESTAMP_RAW((string[OFFSET_TO_KIC_TIMESTAMP + 0] - '0'),
                    (hour + (string[OFFSET_TO_KIC_TIMESTAMP + 3] - '0') * 10 +
                     (string[OFFSET_TO_KIC_TIMESTAMP + 4] - '0')),
                    is_PM, 0);
  return time;
}

BoardSize get_kic_boardsize(const char *string) {
#define OFFSET_TO_KIC_BOARDSIZE (sizeof(KIC_HEADER ";DHHMM"))
  const BoardSize size = // PERF:
      BOARDSIZE((string[OFFSET_TO_KIC_BOARDSIZE + 0] - '0') * 1000 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 1] - '0') * 100 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 2] - '0') * 10 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 3] - '0'),
                (string[OFFSET_TO_KIC_BOARDSIZE + 4] - '0') * 1000 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 5] - '0') * 100 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 6] - '0') * 10 +
                    (string[OFFSET_TO_KIC_BOARDSIZE + 7] - '0'));
  return size;
}

const char *find_kic_schedule(const char *string, const char day) {
  const char converted_day =
      (day >= '0') ? day : day + '0'; // HACK: char is char, int to char
#define OFFSET_TO_HEADER_OF_KIC_SCHEDULES (sizeof(KIC_HEADER ";DHHMM;HHHHWWWW"))
  string += OFFSET_TO_HEADER_OF_KIC_SCHEDULES;

  for (;; string += KIC_SCHEDULE_PAYLOAD_LEN) {
    if (*string == KIC_TERMINATOR || *string == '\0')
      break;
    else if (*(string - 1) == KIC_SEPARATOR) {
      if (*string == converted_day)
        return string;
      string += 2;
    }
  }
  return KIC_SCHEDULE_NOT_FOUND;
}

const KIC_Timestamp get_kic_time_in_schedule(const char *ptr_to_schedule,
                                             const size_t idx) {
  const char *ptr_to_first_time = ptr_to_schedule + 1;
  for (size_t tidx = 0; tidx <= idx; tidx++) {
    if (*(ptr_to_first_time + tidx * KIC_SCHEDULE_PAYLOAD_LEN) == KIC_SEPARATOR)
      return KIC_TIME_NOT_FOUND;
  }
#define DAY (*ptr_to_schedule - '0')
#define OFFSET_TO_FOUND_TIME                                                   \
  (ptr_to_first_time + KIC_SCHEDULE_PAYLOAD_LEN * idx)
  return TIMESTAMP(DAY, (*(OFFSET_TO_FOUND_TIME + 0) - '0') * 1000 +
                            (*(OFFSET_TO_FOUND_TIME + 1) - '0') * 100 +
                            (*(OFFSET_TO_FOUND_TIME + 2) - '0') * 10 +
                            (*(OFFSET_TO_FOUND_TIME + 3) - '0'));
}
