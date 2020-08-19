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
#ifndef EDHELIND_ELFHEADER_H
#define EDHELIND_ELFHEADER_H

#include "libedhel/elf.h"
#include "libedhel/elfimage.h"


/**
 * Interpret the Ehdr structure at the start of every ELF file.
 */
class ElfHeader
{
public:
    ElfHeader(ElfImageView const& imageView);

    bool
    is64() const;

    bool
    isLE() const;

    /** Operating system-specific ABI */
    ehi_osabi
    osabi() const;

    /** ELF file type */
    eh_type
    type() const;

    /** Processor architecture */
    eh_machine
    machine() const;

    /** ELF version */
    std::uint32_t
    version() const;

    /** Entry point virtual address */
    std::uint64_t
    entry() const;

    /** Program header table file offset */
    std::uint64_t
    phoff() const;

    /** Section header table file offset */
    std::uint64_t
    shoff() const;

    /** Processor-specific flags */
    std::uint32_t
    flags() const;

    /** Size of ELF header in bytes */
    std::uint16_t
    ehsize() const;

    /** Size of program header table entry in bytes */
    std::uint16_t
    phentsize() const;

    /** Size of program header table in program headers */
    std::uint16_t
    phnum() const;

    /** Size of section header table entry in bytes */
    std::uint16_t
    shentsize() const;

    /** Size of section header table in section headers */
    std::uint16_t
    shnum() const;

    /** Index of string table section header in section header table */
    std::uint16_t
    shstrndx() const;

private:
    ElfImageView m_imageView;
};

#endif /* EDHELIND_ELFHEADER_H */
