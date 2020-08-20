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
#ifndef EDHELIND_ELFFILE_H
#define EDHELIND_ELFFILE_H

#include "libedhel/elfheader.h"
#include "libedhel/elfimage.h"
#include "libedhel/sectiontable.h"
#include <string>


/*!
 * Wrap an ELF file and present its innards.
 */
class ElfFile
{
public:

    /** Construct an ElfFile from a named file. */
    ElfFile(std::string const& file_name);

    ElfFile(ElfFile const&) = delete;

    ~ElfFile() = default;

    ElfFile& operator=(ElfFile const&) = delete;

    bool
    is_64bit() const;

    /** Get the ELF header */
    ElfHeader const&
    elf_header() const;

    Section const&
    section(std::uint32_t index) const;

    SectionTable const&
    section_table() const;

    /*! Get a view into the file image */
    ElfImageView
    view(std::size_t offset, std::size_t size) const;

private:
    /*! Helper class for setting endianness */
    struct EndianSetter
    {
        EndianSetter(ElfHeader const& elf_header, ElfImage& elf_image)
        {
            elf_image.setBigEndian(!elf_header.isLE());
        }
    };

    std::string   file_name_;
    ElfImage      elf_image_;
    ElfHeader     elf_header_;
    EndianSetter  set_endianness_;
    SectionTable  section_table_;
};

#endif /* EDHELIND_ELFFILE_H */
