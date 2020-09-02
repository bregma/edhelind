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
#ifndef EDHELIND_SEGMENT_H
#define EDHELIND_SEGMENT_H

#include "libedhel/detailable.h"
#include "libedhel/elf.h"
#include "libedhel/elfimage.h"
#include <iosfwd>


class ElfFile;


class Segment
: public Detailable
{
public:
    Segment(ElfFile const& elfFile, ElfImageView const& imageView);

    virtual ~Segment() = default;

    PType
    type() const;

    /**! The type of the segment name as a string */
    std::string
    type_string() const;

    PFlags
    flags() const;

    std::string
    flags_string() const;

    std::uint64_t
    offset() const;

    std::uint64_t
    vaddr() const;

    std::uint64_t
    paddr() const;

    std::uint64_t
    filesz() const;

    std::uint64_t
    memsz() const;

    std::uint64_t
    align() const;

private:
    virtual std::ostream&
    printTo(std::ostream&) const override;

    virtual std::ostream&
    printDetailTo(std::ostream&) const;

private:
    Segment(Segment const&) = delete;
    Segment(Segment const&&) = delete;
    Segment& operator=(Segment const&) = delete;

private:
    ElfFile const* elf_file_;
    ElfImageView   image_view_;
    bool           is_64bit_;
};

#endif /* EDHELIND_SEGMENT_H */
