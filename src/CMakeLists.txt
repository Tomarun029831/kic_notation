# Copyright (c) 2026 Tomarun029831
# SPDX-License-Identifier: MIT

add_library(kic_internal STATIC
    kic_parser/flyweight_kic_parser.c
    kic_timestamp/kic_timestamp.c
)

target_include_directories(kic_internal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(kic_notation INTERFACE)
target_link_libraries(kic_notation INTERFACE kic_internal)

target_include_directories(kic_notation
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(GNUInstallDirs)

install(FILES kic_notation.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY kic_notation/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kic_notation)

install(TARGETS kic_internal kic_notation
    EXPORT kic_notation_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT kic_notation_targets
    FILE kic_notationTargets.cmake
    NAMESPACE kic_notation::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kic_notation
)
