cmake_minimum_required(VERSION 3.10)
project(kic_notation LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for lsp

include(CTest)
enable_testing()

add_subdirectory(modules)
add_subdirectory(test)
