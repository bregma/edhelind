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
#include "libedhel/section_strtab.h"

#include "libedhel/elffile.h"
#include <iomanip>
#include <iostream>


Section_STRTAB::
Section_STRTAB(ElfFile const& elf_file, ElfImageView const& image_view)
: Section(elf_file, image_view)
, string_table_(elf_file.view(this->offset(), this->size()))
{
}


std::string Section_STRTAB::
string(std::uint32_t index) const
{
    return string_table_.get_string(index, std::string::npos);
}


void Section_STRTAB::
iterate_strings(std::function<void(std::uint32_t, std::string)> visit) const
{
    std::uint32_t offset{0};
    while (offset < string_table_.size())
    {
        std::string value{string_table_.get_string(offset, std::string::npos)};
        visit(offset, value);
        offset += value.size() + 1;
    }
}


std::ostream& Section_STRTAB::
printDetailTo(std::ostream& ostr) const
{
    this->iterate_strings([&](std::uint32_t offset, std::string value){
        ostr << "0x" << std::setw(8) << std::setfill('0') << std::hex << offset
             << ": " << value << "\n";
    });
    return ostr;
}
