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
#include "libedhel/segment_interp.h"

#include "libedhel/elffile.h"
#include "libedhel/elfimage.h"
#include <iostream>


Segment_INTERP::
Segment_INTERP(ElfFile const& elf_file, ElfImageView const& image_view)
: Segment(elf_file, image_view)
, interp_(elf_file.view(this->offset(), this->filesz()).get_string(0, std::string::npos))
{
}


std::string Segment_INTERP::
interp() const
{
    return interp_;
}


std::ostream& Segment_INTERP::
printDetailTo(std::ostream& ostr) const
{
    ostr << "Interpreter: " << this->interp() << "\n";
    return ostr;
}
