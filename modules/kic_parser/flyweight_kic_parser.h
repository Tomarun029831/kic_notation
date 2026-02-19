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
    uint32_t millisecond : 10; // 0~999
    uint32_t second : 6;       // 0~59
    uint32_t hour_min : 11;    // 0~1159
    uint32_t day : 3;          // 0~6
    uint32_t is_PM : 1;        // 0:AM, 1:PM
    uint32_t unused : 1;
  } segments;
  uint32_t raw;
} Timestamp;
#define HALF_DAY_OFFSET (1200)
static inline Timestamp TIMESTAMP(unsigned int d, unsigned int t) {
  const char is_PM = (t >= HALF_DAY_OFFSET);
  return (Timestamp){.segments = {.millisecond = 0,
                                  .second = 0,
                                  .hour_min = is_PM ? (t - HALF_DAY_OFFSET) : t,
                                  .day = d,
                                  .is_PM = is_PM,
                                  .unused = 0}};
}
#define TIMESTAMP_RAW(d, t, pm)                                                \
  ((Timestamp){.segments = {.millisecond = 0,                                  \
                            .second = 0,                                       \
                            .hour_min = (t),                                   \
                            .day = (d),                                        \
                            .is_PM = (pm),                                     \
                            .unused = 0}})
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
