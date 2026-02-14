option(KIC_PARSER_TESTS "Build tests for kic_parser" ON)

if(KIC_PARSER_TESTS)
    add_executable(flyweight_kic_parser_test kic_parser/flyweight_kic_parser_test.c)
    target_link_libraries(flyweight_kic_parser_test PRIVATE kic_parser)
endif()
