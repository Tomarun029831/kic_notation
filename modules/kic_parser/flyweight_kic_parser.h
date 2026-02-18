#pragma once
#define KIC_CONPATIBLE ((unsigned char)0)
#define KIC_INCONPATIBLE ((unsigned char)1)
const unsigned char check_kic_compatibility(const char *string);

#define KIC_SYNTAX_CORRECT ((unsigned char)0)
#define KIC_SYNTAX_ERROR ((unsigned char)1)
// this function will call check_kic_compatibility() with the argument
const unsigned char check_kic_syntax(const char *string);

#define TIMESTAMP(day, time) ((unsigned int)(day * 10000 + time))
typedef struct {
  const unsigned int height_cm;
  const unsigned int width_cm;
} BoardSize;
#define BOARDSIZE(h, w) ((BoardSize){(h), (w)})
// this function will NOT check the syntax,
// So you need to call check_kic_syntax(const char *string),
// if you cannot ensure that the argument of this function follows kic-format
const unsigned int get_kic_timestamp(const char *string);

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
