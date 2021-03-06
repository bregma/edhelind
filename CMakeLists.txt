#
# Copyright 2020 Stephen M. Webb <stephen.webb@bregmasoft.ca>
#
# This file is part of Edhelind.
#
# Edhelind is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Edhelind is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Edhelind.  If not, see <https://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 3.5)

# set the project name
project(edhelind VERSION 0.1)

# Project-wide configuration
if (${CMAKE_VERSION} VERSION_LESS "3.10.0")
    if (MSVC)
        add_compile_options("/std:c++17")
    else()
        add_compile_options("-std=c++17")
    endif()
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

configure_file(edhelind_config.h.in edhelind_config.h)

include_directories(edhelind PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# The edhelind library
add_library(libedhel STATIC
    libedhel/elffile.cpp
    libedhel/elfimage.cpp
    libedhel/elfheader.cpp
    libedhel/note.cpp
    libedhel/section.cpp
    libedhel/sectiontable.cpp
    libedhel/section_note.cpp
    libedhel/section_strtab.cpp
    libedhel/section_symtab.cpp
    libedhel/segment.cpp
    libedhel/segmenttable.cpp
    libedhel/segment_interp.cpp
    libedhel/segment_note.cpp
    libedhel/symbol.cpp)

target_compile_options(libedhel PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra>
     $<$<CXX_COMPILER_ID:MSVC>:
        /W4>)

# Main GUI executable
add_executable(edhelind
    edhelind/main.cpp
    edhelind/mainwindow.cpp)

target_link_libraries(edhelind Qt5::Widgets libedhel)


# Unit tests
enable_testing()
add_executable(edhelind_test
    test/test_main.cpp
    test/test_elfimage.cpp
    test/test_elffile.cpp)

target_link_libraries(edhelind_test libedhel)

add_test(NAME edhelind_test COMMAND edhelind_test)
