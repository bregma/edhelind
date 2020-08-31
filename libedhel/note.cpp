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
#include "libedhel/note.h"

#include <iomanip>
#include <iostream>
#include "libedhel/elf.h"
#include "libedhel/elfimage.h"


namespace
{
    constexpr std::size_t note_namesz_offset = offsetof(ElfNote, namesz);
    constexpr std::size_t note_descsz_offset = offsetof(ElfNote, descsz);
    constexpr std::size_t note_type_offset = offsetof(ElfNote, type);
    constexpr std::size_t note_name_offset = offsetof(ElfNote, name);

    /** Move a value up to the nearest multiple of 4 */
    inline std::size_t align4(std::size_t val)
    {
        return (val + 3) &~ (3);
    }
} // anonymous


Note::
Note(std::string const& name, std::uint32_t type, ElfImageView const& descriptor)
: name_{name}, type_{type}, descriptor_{descriptor}
{ }


std::ostream& Note::
printTo(std::ostream& ostr) const
{
    ostr << "name: " << name_ << "\n"
         << "type: " << type_ << "\n"
         << "data: ";
    for (std::size_t i = 0; i < descriptor_.size(); ++i)
    {
        ostr << std::setw(2) << std::setfill('0') << std::hex
             << std::to_integer<int>(*descriptor_.get_bytes(i))
             << " ";
    }
    ostr << "\n";
    return ostr;
}


NoteTable::
NoteTable(ElfImageView const& image_view)
{
    std::size_t parsed_size = 0;
    while (parsed_size < image_view.size())
    {
        auto namesz = image_view.get_uint32(parsed_size + note_namesz_offset);
        auto descsz = image_view.get_uint32(parsed_size + note_descsz_offset);
        auto type = image_view.get_uint32(parsed_size + note_type_offset);
        std::size_t desc_offset = note_name_offset + align4(namesz);

        notes_.emplace_back(image_view.get_string(parsed_size + note_name_offset, std::string::npos),
                            type,
                            image_view.view(desc_offset, descsz));

        std::size_t note_size = align4(namesz) + align4(descsz) + 3*sizeof(std::uint32_t);
        parsed_size += note_size;
    }
}


void NoteTable::
iterate_notes(std::function<void(Note const&)> visit) const
{
    for (auto const& note: notes_)
    {
        visit(note);
    }
}



