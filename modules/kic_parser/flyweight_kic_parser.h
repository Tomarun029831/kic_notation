#pragma once
#include <stdint.h>

#define KIC_CONPATIBLE ((unsigned char)0)
#define KIC_INCONPATIBLE ((unsigned char)1)
const unsigned char check_kic_compatibility(const char *string);

#define KIC_SYNTAX_CORRECT ((unsigned char)0)
#define KIC_SYNTAX_ERROR ((unsigned char)1)
// this function will call check_kic_compatibility() with the argument
const unsigned char check_kic_syntax(const char *string);

typedef union {
  struct {
    uint16_t time : 11; // 0~1159
    uint16_t day : 3;   // 0~6
    uint16_t isPM : 1;  // 0 - am or 1 - pm
    uint16_t unused : 1;
  } segments;
  uint16_t raw;
} Timestamp;
#define HALF_DAY_OFFSET (1200)
static inline Timestamp TIMESTAMP(unsigned int d, unsigned int t) {
  const char is_pm = (t >= HALF_DAY_OFFSET);
  return (Timestamp){.segments = {.isPM = is_pm,
                                  .day = d,
                                  .time = is_pm ? (t - HALF_DAY_OFFSET) : t,
                                  .unused = 0}};
}
#define TIMESTAMP_RAW(d, t, pm)                                                \
  ((Timestamp){                                                                \
      .segments = {.time = (t), .day = (d), .isPM = (pm), .unused = 0}})
// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
const Timestamp get_kic_timestamp(const char *string);

typedef struct {
  const uint16_t height_cm;
  const uint16_t width_cm;
} BoardSize;
#define BOARDSIZE(h, w) ((BoardSize){(h), (w)})
// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
BoardSize get_kic_boardsize(const char *string);

// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
const char *find_kic_schedule(const char *string);

// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
const char *find_kic_day_of_week_in_schedule(const char *string);

// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
const char *find_kic_time_in_schedule(const char *string);
