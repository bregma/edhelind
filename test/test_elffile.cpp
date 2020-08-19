/*
 * Copyright 2020 Stephen M. Webb <stephen.webb@bregmasoft.ca>
 * 
 * This file is part of Edhelind.
 * 
 * Edhelind is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "test/catch.hpp"

#include "libedhel/elffile.h"


TEST_CASE("ElfFile functionality") {
    SECTION("basics") {
#ifdef DUDE_LATER_DUDE
        ElfFile elfFile("../test_files/64_intel_ls");
        EXPECT_TRUE(elfFile.isLE());
        EXPECT_TRUE(elfFile.is64());
        EXPECT_EQ(elfFile.elfHeader().version(), 1U);
        EXPECT_EQ(elfFile.elfHeader().machine(), 62U);
        EXPECT_EQ(elfFile.elfHeader().entry(), 0x404888UL);
        EXPECT_EQ(elfFile.elfHeader().phoff(), 64UL);
        EXPECT_EQ(elfFile.elfHeader().shoff(), 108296UL);
        EXPECT_EQ(elfFile.elfHeader().phentsize(), 56U);
        EXPECT_EQ(elfFile.elfHeader().phnum(), 9U);
        EXPECT_EQ(elfFile.elfHeader().shentsize(), 64U);
        EXPECT_EQ(elfFile.elfHeader().shnum(), 28U);
        EXPECT_EQ(elfFile.elfHeader().shstrndx(), 27U);
#endif
    }
}
