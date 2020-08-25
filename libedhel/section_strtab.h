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
#ifndef EDHELIND_SECTION_STRTAB_H
#define EDHELIND_SECTION_STRTAB_H

#include "libedhel/section.h"


/*!
 * An SHT_STRTAB section
 */
class Section_STRTAB
: public Section
{
public:
    Section_STRTAB(ElfFile const& elf_file, ElfImageView const& image_view);

    /*! Retrieve the string at @p index */
    std::string
    string(std::uint32_t index) const;

    std::ostream&
    printTo(std::ostream& ostr) const override;

private:
    ElfImageView string_table_;
};

#endif /* EDHELIND_SECTION_STRTAB_H */
