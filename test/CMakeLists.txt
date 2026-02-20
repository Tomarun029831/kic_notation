# Copyright (c) 2026 Tomarun029831
# SPDX-License-Identifier: MIT
option(KIC_PARSER_TESTS "Build tests for kic_parser" ON)
option(KIC_TIMESTAMP_TESTS "Build tests for timestamp" ON)

if(KIC_PARSER_TESTS)
    add_executable(flyweight_kic_parser_test kic_parser/flyweight_kic_parser_test.c)
    target_link_libraries(flyweight_kic_parser_test PRIVATE kic_parser)
    add_test(NAME parser_test COMMAND flyweight_kic_parser_test)
endif()

if(KIC_TIMESTAMP_TESTS)
    add_executable(kic_timestamp_test kic_timestamp/kic_timestamp_test.c)
    target_link_libraries(kic_timestamp_test PRIVATE kic_timestamp)
    add_test(NAME timestamp_test COMMAND kic_timestamp_test)
endif()
