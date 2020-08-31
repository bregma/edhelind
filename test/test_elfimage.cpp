/*
 * Copyright 2020  Stephen M. Webb <stephen.webb@bregmsoft.ca>
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
#include "libedhel/elfimage.h"
#include <sstream>


TEST_CASE("ElfImage functionality") {
    ElfImage::ByteSequence  test_image = {
        std::byte(0x01),
        std::byte(0x02),
        std::byte(0x03),
        std::byte(0x04),
        std::byte(0x05),
        std::byte(0x06),
        std::byte(0x07),
        std::byte(0x08),
        std::byte('t'), std::byte('e'), std::byte('s'), std::byte('t'),
        std::byte(0x00),
    };


    SECTION("Verify extraction in little-endian (default) mode") {
        ElfImage image(test_image);

        CHECK(image.get_uint8(0) == 0x01);
        CHECK(image.get_uint16(0) == 0x0201);
        CHECK(image.get_uint32(0) == 0x04030201);
        CHECK(image.get_uint64(0) == 0x0807060504030201LL);
        CHECK(image.get_string(8, 5) == "test");
    }


    SECTION("Verify extraction in big-endian mode") {
        ElfImage image(test_image);
        image.setBigEndian(true);

        CHECK(image.get_uint8(0) == 0x01);
        CHECK(image.get_uint16(0) == 0x0102);
        CHECK(image.get_uint32(0) == 0x01020304);
        CHECK(image.get_uint64(0) == 0x0102030405060708LL);
        CHECK(image.get_string(8, 4) == "test");
    }


    SECTION("Verify basic ElfImageView functionality") {
        ElfImage image(test_image);

        auto view = image.view(8, 5);
        CHECK(view.get_string(0, 8) == "test");

        auto subview = view.view(2, 3);
        CHECK(subview.get_string(0, 2) == "st");
    }


    SECTION("Verify creating an ElfImage from a non-existent file throws") {
        auto create_empty_image = [](){ ElfImage image{""}; };
        REQUIRE_THROWS_AS(create_empty_image(), std::runtime_error);
    }

}
