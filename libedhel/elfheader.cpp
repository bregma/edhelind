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
#include "libedhel/elfheader.h"

#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <type_traits>


namespace {
    constexpr std::size_t ehdr32_magic_offset = offsetof(Elf32_Ehdr, ei_magic);
    constexpr std::size_t ehdr32_class_offset = offsetof(Elf32_Ehdr, ei_class);
    constexpr std::size_t ehdr32_encoding_offset = offsetof(Elf32_Ehdr, ei_data);
    constexpr std::size_t ehdr32_fileversion_offset = offsetof(Elf32_Ehdr, ei_version);
    constexpr std::size_t ehdr32_osabi_offset = offsetof(Elf32_Ehdr, ei_osabi);
    constexpr std::size_t ehdr32_type_offset = offsetof(Elf32_Ehdr, e_type);
    constexpr std::size_t ehdr32_machine_offset = offsetof(Elf32_Ehdr, e_machine);
    constexpr std::size_t ehdr32_version_offset = offsetof(Elf32_Ehdr, e_version);
    constexpr std::size_t ehdr32_entry_offset = offsetof(Elf32_Ehdr, e_entry);
    constexpr std::size_t ehdr32_phoff_offset = offsetof(Elf32_Ehdr, e_phoff);
    constexpr std::size_t ehdr32_shoff_offset = offsetof(Elf32_Ehdr, e_shoff);
    constexpr std::size_t ehdr32_flags_offset = offsetof(Elf32_Ehdr, e_flags);
    constexpr std::size_t ehdr32_ehsize_offset = offsetof(Elf32_Ehdr, e_ehsize);
    constexpr std::size_t ehdr32_phentsize_offset = offsetof(Elf32_Ehdr, e_phentsize);
    constexpr std::size_t ehdr32_phnum_offset = offsetof(Elf32_Ehdr, e_phnum);
    constexpr std::size_t ehdr32_shentsize_offset = offsetof(Elf32_Ehdr, e_shentsize);
    constexpr std::size_t ehdr32_shnum_offset = offsetof(Elf32_Ehdr, e_shnum);
    constexpr std::size_t ehdr32_shstrndx_offset = offsetof(Elf32_Ehdr, e_shstrndx);

    /* First 5 fields are same as Elf32_Ehdr, use that instead */
    constexpr std::size_t ehdr64_type_offset = offsetof(Elf64_Ehdr, e_type);
    constexpr std::size_t ehdr64_machine_offset = offsetof(Elf64_Ehdr, e_machine);
    constexpr std::size_t ehdr64_version_offset = offsetof(Elf64_Ehdr, e_version);
    constexpr std::size_t ehdr64_entry_offset = offsetof(Elf64_Ehdr, e_entry);
    constexpr std::size_t ehdr64_phoff_offset = offsetof(Elf64_Ehdr, e_phoff);
    constexpr std::size_t ehdr64_shoff_offset = offsetof(Elf64_Ehdr, e_shoff);
    constexpr std::size_t ehdr64_flags_offset = offsetof(Elf64_Ehdr, e_flags);
    constexpr std::size_t ehdr64_ehsize_offset = offsetof(Elf64_Ehdr, e_ehsize);
    constexpr std::size_t ehdr64_phentsize_offset = offsetof(Elf64_Ehdr, e_phentsize);
    constexpr std::size_t ehdr64_phnum_offset = offsetof(Elf64_Ehdr, e_phnum);
    constexpr std::size_t ehdr64_shentsize_offset = offsetof(Elf64_Ehdr, e_shentsize);
    constexpr std::size_t ehdr64_shnum_offset = offsetof(Elf64_Ehdr, e_shnum);
    constexpr std::size_t ehdr64_shstrndx_offset = offsetof(Elf64_Ehdr, e_shstrndx);

    struct EHTypeMap {
        eh_type     type_;
        std::string name_;
    };

    std::vector<EHTypeMap> type_name_mapping {
        { eh_type::ET_NONE, "ET_NONE" },
        { eh_type::ET_REL,  "ET_REL" },
        { eh_type::ET_EXEC, "ET_EXEC" },
        { eh_type::ET_DYN,  "ET_DYN" },
        { eh_type::ET_CORE, "ET_CORE" },
    };

} // anonymous


ElfHeader::
ElfHeader(ElfImageView const& imageView)
: m_imageView(imageView)
{
    // See https://www.muppetlabs.com/~breadbox/software/tiny/teensy.html for a
    // description of the smallest (although invalid) ELF file loadable by the
    // 32-bit Linux kernel.  This value might be larger or smaller on other
    // target CPUs or operating systems.
    if (m_imageView.size() < 45)
    {
        throw std::runtime_error("The file is too small to be an ELF binary");
    }

    // Validate the magic
    if (std::memcmp(m_imageView.get_bytes(ehdr32_magic_offset),
                    elf_magic, sizeof(elf_magic)) != 0)
    {
        throw std::runtime_error("The file does not start with ELF magic.");
    }
}


bool ElfHeader::
is64() const
{
    std::size_t class_offset = ehdr32_class_offset;
    ehi_class e_class = static_cast<ehi_class>(m_imageView.get_uint8(class_offset));
    return (e_class == ehi_class::ELFCLASS64);
}


bool ElfHeader::
isLE() const
{
    std::size_t encoding_offset = ehdr32_encoding_offset;
    ehi_data e_encoding = static_cast<ehi_data>(m_imageView.get_uint8(encoding_offset));
    return (e_encoding == ehi_data::ELFDATA2LSB);
}


ehi_osabi ElfHeader::
osabi() const
{
    return static_cast<ehi_osabi>(m_imageView.get_uint8(ehdr32_osabi_offset));
}

eh_type ElfHeader::
type() const
{
    return static_cast<eh_type>(m_imageView.get_uint16(ehdr32_type_offset));
}


std::string ElfHeader::
type_string() const
{
    using std::ostringstream;
    using std::showbase;
    using std::hex;
    using std::underlying_type;

    eh_type type{this->type()};
    for (auto const& it: type_name_mapping) {
        if (it.type_ == type) {
            return it.name_;
        }
    }
    ostringstream ostr;
    ostr << "unknown ("
         << showbase << hex << static_cast<underlying_type<eh_type>::type>(type)
         << ")";
    return ostr.str();
}


eh_machine ElfHeader::
machine() const
{
    return static_cast<eh_machine>(m_imageView.get_uint16(ehdr32_machine_offset));
}


std::uint32_t ElfHeader::
version() const
{
    return m_imageView.get_uint16(ehdr32_version_offset);
}


std::uint64_t ElfHeader::
entry() const
{
    return is64() ? m_imageView.get_uint64(ehdr64_entry_offset)
                  : m_imageView.get_uint32(ehdr32_entry_offset);
}


std::uint64_t ElfHeader::
phoff() const
{
    return is64() ? m_imageView.get_uint64(ehdr64_phoff_offset)
                  : m_imageView.get_uint32(ehdr32_phoff_offset);
}


std::uint64_t ElfHeader::
shoff() const
{
    return is64() ? m_imageView.get_uint64(ehdr64_shoff_offset)
                  : m_imageView.get_uint32(ehdr32_shoff_offset);
}


std::uint32_t ElfHeader::
flags() const
{
    return is64() ? m_imageView.get_uint32(ehdr64_flags_offset)
                  : m_imageView.get_uint32(ehdr32_flags_offset);
}


std::uint16_t ElfHeader::
ehsize() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_ehsize_offset)
                  : m_imageView.get_uint16(ehdr32_ehsize_offset);
}


std::uint16_t ElfHeader::
phentsize() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_phentsize_offset)
                  : m_imageView.get_uint16(ehdr32_phentsize_offset);
}


std::uint16_t ElfHeader::
phnum() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_phnum_offset)
                  : m_imageView.get_uint16(ehdr32_phnum_offset);
}


std::uint16_t ElfHeader::
shentsize() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_shentsize_offset)
                  : m_imageView.get_uint16(ehdr32_shentsize_offset);
}


std::uint16_t ElfHeader::
shnum() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_shnum_offset)
                  : m_imageView.get_uint16(ehdr32_shnum_offset);
}


std::uint16_t ElfHeader::
shstrndx() const
{
    return is64() ? m_imageView.get_uint16(ehdr64_shstrndx_offset)
                  : m_imageView.get_uint16(ehdr32_shstrndx_offset);
}


