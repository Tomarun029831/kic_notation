// Copyright (c) 2026 Tomarun029831
// SPDX-License-Identifier: MIT
#include <kic_notation.h>
#include <HardwareSerial.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  const char* data = "KIC:V3;01200;00400030;110001230/";
  Serial.println("--- KIC Notation Basic Usage ---");

  // 1. Syntax Validation
  if (check_kic_syntax(data) == KIC_SYNTAX_CORRECT) {
    Serial.println("Syntax: OK");

    // 2. Zero-copy parse
    KIC_Timestamp ts = get_kic_timestamp(data);
    Serial.print("Base Timestamp Day: ");
    Serial.println(ts.segments.day);

    BoardSize size = get_kic_boardsize(data);
    Serial.print("Board Size: ");
    Serial.print(size.width_cm);
    Serial.print("x");
    Serial.println(size.height_cm);

    KIC_SchedulePtr schedule = find_kic_schedule(data, 1);
    if (schedule != KIC_SCHEDULE_NOT_FOUND) {
      // Get first time of events
      KIC_Timestamp event = find_kic_time_in_schedule(schedule, 0);
      if (event != KIC_TIME_NOT_FOUND) {
        Serial.print("First event hour_min: ");
        Serial.println(event.segments.hour_min);
      }
    }
  } else {
    Serial.println("Syntax: Error!");
  }
}

void loop() {}
