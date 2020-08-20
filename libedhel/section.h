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
#ifndef EDHELIND_SECTION_H
#define EDHELIND_SECTION_H

#include "libedhel/elf.h"
#include "libedhel/elfimage.h"
#include <iosfwd>


class ElfFile;


class Section
{
public:
    Section(ElfFile const& elfFile, ElfImageView const& imageView);

    virtual ~Section() = default;

    /**! The index of the section name in the shstrndx section */
    std::uint32_t
    name() const;

    /**! The name of the section name as a string */
    std::string
    name_string() const;

    s_type
    type() const;

    /**! The type of the section name as a string */
    std::string
    type_string() const;

    std::uint64_t
    flags() const;

    std::uint64_t
    addr() const;

    std::uint64_t
    offset() const;

    std::uint64_t
    size() const;

    std::uint32_t
    link() const;

    std::uint32_t
    info() const;

    std::uint64_t
    addralign() const;

    std::uint64_t
    entsize() const;

    virtual std::ostream&
    printTo(std::ostream&) const;

private:
    Section(Section const&) = delete;
    Section(Section const&&) = delete;
    Section& operator=(Section const&) = delete;

private:
    ElfFile const* m_elfFile;
    ElfImageView   m_imageView;
    bool           m_is64bit;
};


inline std::ostream&
operator<<(std::ostream& ostr, Section const& section)
{
    return section.printTo(ostr);
}

#endif /* EDHELIND_SECTION_H */