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


std::ostream& Section_STRTAB::
printTo(std::ostream& ostr) const
{
    Section::printTo(ostr) << "some string table" << '\n';
    return ostr;
}
