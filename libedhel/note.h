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
#ifndef EDHELIND_NOTE_H
#define EDHELIND_NOTE_H

#include <functional>
#include "libedhel/detailable.h"
#include "libedhel/elfimage.h"
#include <string>
#include <vector>


/**
 * A single Note
 *
 * A Note is keyed by (name, type).
 */
struct Note
: public Detailable
{
    using Bytes = std::vector<std::byte>;

    Note(std::string const& name, std::uint32_t type, ElfImageView const& descriptor);

    std::ostream&
    printTo(std::ostream& ostr) const override;

    std::string   name_;
    std::uint32_t type_;
    ElfImageView  descriptor_;
};


/**
 * A SHT_NOTE or PT_NOTE is a collection of one or more Notes.
 *
 * A NoteTable may be a Section or it may be a Segment. They work the same way.
 */
class NoteTable
{
public:
    NoteTable(ElfImageView const& image_view);

    void
    iterate_notes(std::function<void(Note const&)>) const;

private:
    std::vector<Note> notes_;
};

#endif /* EDHELIND_NOTE_H */

