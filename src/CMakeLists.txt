# Copyright (c) 2026 Tomarun029831
# SPDX-License-Identifier: MIT
add_library(kic_timestamp STATIC)
target_sources(kic_timestamp
    PRIVATE
        kic_timestamp/kic_timestamp.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
            kic_timestamp/kic_timestamp.h
)
target_include_directories(kic_timestamp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kic_notation>
)

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
target_link_libraries(kic_parser PUBLIC kic_timestamp)
target_sources(kic_parser
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
            kic_notation.h
)
target_include_directories(kic_parser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/kic_notation>
)

add_library(kic_notation INTERFACE)
target_link_libraries(kic_notation INTERFACE kic_parser)
add_library(kic_notation::kic_notation ALIAS kic_notation)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS kic_parser kic_timestamp kic_notation
    EXPORT kic_notationTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kic_notation
)

install(EXPORT kic_notationTargets
    FILE kic_notationTargets.cmake
    NAMESPACE kic_notation::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kic_notation
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/kic_notationConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/kic_notationConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kic_notation
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kic_notationConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kic_notation
)

install(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
        DESTINATION "."
        COMPONENT Runtime)
