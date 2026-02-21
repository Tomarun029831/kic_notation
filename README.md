<!-- Copyright (c) 2026 Tomarun029831 -->
<!-- SPDX-License-Identifier: MIT -->
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)
[![Build and Test](https://github.com/Tomarun029831/kic_notation/actions/workflows/cmake.yml/badge.svg)](https://github.com/Tomarun029831/kic_notation/actions/workflows/cmake.yml)

# background
Note: This library was born to parse a "ridiculous" format created with a friend. While the goal was to minimize communication bandwidth, we ended up achieving "insane" speeds and "joke-like" memory consumption for embedded systems.

# kic_notation

A high-performance, **zero-copy** music/schedule notation parser and timestamp management library written in pure C99 and some standard libraries. Specifically optimized for resource-constrained environments like **Arduino**, **ESP32**, and other MCU platforms.

## 🚀 Key Features

* **Zero-Copy Flyweight Parser**: Does not use `malloc()` or `free()`. It parses the KIC string format directly from the buffer using pointer arithmetic, keeping the memory footprint near zero.
* **Compact 32-bit Timestamps**: Uses C bit-fields to pack Year/Day/Hour/Min/Sec/Ms into a single `uint32_t` (4 bytes).
* **Embedded Friendly**: Native support for Arduino (integrated test suite) and standard PC environments.
* **Robust Validation**: Includes a strict syntax checker to ensure data integrity before processing.
* **Modern CMake**: Fully supports `find_package()` and `FetchContent` with proper export targets.

---

## 📋 Requirements & Dependencies

This library is designed for maximum portability and has **zero external dependencies**. 

- **Language**: Standard C99 or later.
- **Standard Headers**: Requires only `stdint.h` and `stddef.h` (provided by any standard-compliant C compiler/libc).

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

## ⚡ Performance & Memory Footprint

### 📊 Benchmark Results (Verified Reality)
To verify the "insane" performance, we measured a **Full-API call sequence** (Validation + Extraction + Schedule Search) using randomized inputs.

| Metric | Measured Value |
| :--- | :--- |
| **Average Speed** | **4.30 nanoseconds** per full-parse |
| **Throughput** | **~230,000,000** parses per second |
| **Test Data** | `KIC:V1;01200;00400030;01454;110001230;209001800;30830;40901;512341034;610304050/` |

### 💾 Static Analysis (Binary Size)
Verified with `size.exe` (GCC -Os). The library achieves a pure **Zero-static-RAM** architecture.

| Component | Text (Code) | Data (RAM) | BSS (RAM) |
| :--- | :---: | :---: | :---: |
| `flyweight_kic_parser.o` | 832 B | **0 B** | **0 B** |
| `kic_timestamp.o` | 1,172 B | **0 B** | **0 B** |
| **Total Footprint** | **~2.0 KB** | **0 B** | **0 B** |

* **Heap Usage**: **0 bytes** (No `malloc` / `free`).
* **Static RAM Usage**: **0 bytes** (No global variables).

### ⚙️ Algorithm Complexity
| Function | Algorithm | Complexity | Performance Characteristic |
| --- | --- | --- | --- |
|check_kic_compatibility|Fixed-string Comparison|O(1)|Anchored header check. Constant time based on version string length.|
|check_kic_syntax|Bounded Linear Scan|O(Nmax​)|Strict validation of all segments. Time is bounded by protocol max length.|
|get_kic_timestamp|Direct Mapping|O(1)|Zero-loop extraction. Uses compile-time fixed offsets.|
|get_kic_boardsize|Direct Mapping|O(1)|Immediate memory access. No string searching or scanning.|
|find_kic_schedule|Stride Linear Search|O(M)|Jumps in 4-byte strides to find matching day. M≤ max schedules.|
|get_kic_time_in_schedule|Indexed Validation|O(I)|Validated offset jump to index I within a specific schedule block.|
|TIMESTAMP|Bit-packing|O(1)|Inline construction of bit-fields. Minimal CPU cycles.|
|KIC_Timestamp_Compare|Atomic Raw Comparison|O(1)|Single-instruction 32-bit integer comparison.|
|KIC_Timestamp_AddMs|Cascading Arithmetic|O(1)|Sequential addition with carry-over logic. No recursion.|
|KIC_Timestamp_AddSec|Cascading Arithmetic|O(1)|Sequential addition with carry-over logic.|
|KIC_Timestamp_AddMin|Cascading Arithmetic|O(1)|Sequential addition with carry-over logic.|
|KIC_Timestamp_AddHour|Cycle-aware Arithmetic|O(1)|Modular arithmetic to handle 12h roll-over and AM/PM toggle.|
|KIC_Timestamp_AddDay|Modular Arithmetic|O(1)|Single modulo operation (% 7) to handle day-of-week roll-over.|

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
#include <kic_notation.h>

const char* data = "KIC:V3;01200;00400030;110001230/";

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
