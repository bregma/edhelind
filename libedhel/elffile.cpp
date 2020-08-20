/*
 * Copyright 2020  Stephen M. Webb <stephen.webb@bregmasoft.ca>
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
#include "libedhel/elffile.h"


ElfFile::
ElfFile(std::string const& file_name)
: file_name_(file_name)
, elf_image_(file_name_)
, elf_header_(elf_image_.view(0, 56))
, set_endianness_(elf_header_, elf_image_)
, section_table_(*this)
{
}


bool ElfFile::
is_64bit() const
{
    return elf_header_.is64();
}


ElfHeader const& ElfFile::
elf_header() const
{
    return elf_header_;
}


Section const& ElfFile::
section(std::uint32_t index) const
{
    return section_table_.section(index);
}


SectionTable const& ElfFile::
section_table() const
{
    return section_table_;
}


ElfImageView ElfFile::
view(std::size_t offset, std::size_t size) const
{
    return elf_image_.view(offset, size);
}
