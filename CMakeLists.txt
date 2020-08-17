#
# Copyright 2020 Stephen M. Webb <stephen.webb@bregmasoft.ca>
#
# This file is part of Foobar.
#
# Foobar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 3.5)

# set the project name
project(edhelind VERSION 0.1)

# compiler flags
if (MSVC)
    add_compile_options("/std:c++17")
else()
    add_compile_options("-std=c++17" "-Wall" "-Wextra")
endif()


# add the executable
add_executable(edhelind
    edhelind/main.cpp)
