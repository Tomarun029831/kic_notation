![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)
[![Build and Test](https://github.com/Tomarun029831/kic_notation/actions/workflows/cmake.yml/badge.svg)](https://github.com/Tomarun029831/kic_notation/actions/workflows/cmake.yml)

# kic_notation

A high-performance, **zero-copy** music/schedule notation parser and timestamp management library written in pure C99. Specifically optimized for resource-constrained environments like **Arduino**, **ESP32**, and other MCU platforms.

## 🚀 Key Features

* **Zero-Copy Flyweight Parser**: Does not use `malloc()` or `free()`. It parses the KIC string format directly from the buffer using pointer arithmetic, keeping the memory footprint near zero.
* **Compact 32-bit Timestamps**: Uses C bit-fields to pack Year/Day/Hour/Min/Sec/Ms into a single `uint32_t` (4 bytes).
* **Embedded Friendly**: Native support for Arduino (integrated test suite) and standard PC environments.
* **Robust Validation**: Includes a strict syntax checker to ensure data integrity before processing.
* **Modern CMake**: Fully supports `find_package()` and `FetchContent` with proper export targets.

---

## 🏗 Memory Architecture

### Compact Timestamp Representation

The library utilizes a `union` with bit-fields to minimize RAM usage while providing human-readable segments.

| Component | Bits | Range |
| --- | --- | --- |
| `millisecond` | 10 | 0–999 |
| `second` | 6 | 0–59 |
| `hour_min` | 11 | 0–1159 (HHMM format) |
| `day` | 3 | 0–6 |
| `is_PM` | 1 | 0: AM, 1: PM |
| `is_invalid` | 1 | Status Flag |

---

## 📝 KIC Format Specification

The KIC format is a semicolon-delimited string designed for easy transmission and low-cost parsing:

`KIC:[VER];[TIMESTAMP];[BOARD_SIZE];[SCHEDULES...]/`

* **Example**: `KIC:V3;01200;00400030;110001230;20900/`
* `V1`: Version 3.
* `01200`: Day 0, 12:00 PM.
* `00400030`: Board size Height 40cm, Width 30cm.
* `110001230`: Schedule for Day 1 starting at 10:00 and 12:30.
* `/`: Terminator.



---

## 🛠 Usage

### C API Example

```c
#include <kic_notation/kic_parser/flyweight_kic_parser.h>

const char* data = "KIC:V1;01200;00400030;110001230/";

// 1. Validate Syntax
if (check_kic_syntax(data) == KIC_SYNTAX_CORRECT) {
    // 2. Get Timestamp (Zero-copy)
    KIC_Timestamp ts = get_kic_timestamp(data);
    
    // 3. Find specific schedule for Day 1
    const char* schedule = find_kic_schedule(data, 1);
    if (schedule != KIC_SCHEDULE_NOT_FOUND) {
        KIC_Timestamp event = get_kic_time_in_schedule(schedule, 0);
    }
}

```

### Building with CMake

```cmake
find_package(kic_notation REQUIRED)

target_link_libraries(your_project PRIVATE kic_notation::kic_parser)

```

---

## 🧪 Testing

The library uses `ctest` and is verified on both x86_64 and Arduino environments.

```bash
mkdir build && cd build
cmake .. -DKIC_PARSER_TESTS=ON
make
ctest

```

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

---
