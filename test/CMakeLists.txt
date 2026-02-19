option(KIC_PARSER_TESTS "Build tests for kic_parser" ON)
option(KIC_TIMESTAMP_TESTS "Build tests for timestamp" ON)

if(KIC_PARSER_TESTS)
    add_executable(flyweight_kic_parser_test kic_parser/flyweight_kic_parser_test.c)
    target_link_libraries(flyweight_kic_parser_test PRIVATE kic_parser)
endif()

if(TIMESTAMP_TESTS)
    add_executable(kic_timestamp_test kic_timestamp/kic_timestamp_test.c)
    target_link_libraries(kic_timestamp_test PRIVATE kic_timestamp)
endif()
