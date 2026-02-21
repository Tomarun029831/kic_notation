// Copyright (c) 2026 Tomarun029831
// SPDX-License-Identifier: MIT
#pragma once

#ifdef KIC_USE_INTERNAL_PATH
#include "internal/flyweight_kic_parser.h"      // IWYU pragma: export
#include "internal/kic_parser_specifications.h" // IWYU pragma: export
#include "internal/kic_timestamp.h"             // IWYU pragma: export
#else
#include "kic_parser/flyweight_kic_parser.h"      // IWYU pragma: export
#include "kic_parser/kic_parser_specifications.h" // IWYU pragma: export
#include "kic_timestamp/kic_timestamp.h"          // IWYU pragma: export
#endif
