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
#include "libedhel/segment_note.h"

#include <iostream>
#include "libedhel/elffile.h"


Segment_NOTE::
Segment_NOTE(ElfFile const& elf_file, ElfImageView const& image_view)
: Segment{elf_file, image_view}
, note_table_{elf_file.view(this->offset(), this->filesz())}
{
}


void Segment_NOTE::
iterate_notes(std::function<void(Note const&)> visit) const
{
    note_table_.iterate_notes(visit);
}


std::ostream& Segment_NOTE::
printDetailTo(std::ostream& ostr) const
{
    ostr << "NOTE details";
    return ostr;
}

