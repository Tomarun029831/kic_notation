add_library(kic_parser STATIC)
target_sources(kic_parser
    PRIVATE
        kic_parser/flyweight_kic_parser.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
            kic_parser/flyweight_kic_parser.h
)

add_library(timestamp STATIC)
target_sources(timestamp
    PRIVATE
        timestamp/timestamp.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
            timestamp/timestamp.h
)
