/*
 * Copyright 2020  Stephen M. Webb <stephen.webb@bregmasoft.ca>
 * 
 * This file is part of ELF Parser.
 * 
 * ELF Parser is free software: you can redistribute it and/or modify
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
#include "libedhel/section.h"

#include <algorithm>
#include "libedhel/elffile.h"
#include <iostream>
#include "section_strtab.h"


namespace
{
    /*! map sh_type to a well-known string */
    struct TypeNameMapping {
        enum s_type type_;
        std::string name_;
    }; 

    std::vector<TypeNameMapping> type_name_mapping {
        { s_type::SHT_NULL,           "SHT_NULL" },
        { s_type::SHT_PROGBITS,       "SHT_PROGBITS" },
        { s_type::SHT_SYMTAB,         "SHT_SYMTAB" },
        { s_type::SHT_STRTAB,         "SHT_STRTAB" },
        { s_type::SHT_RELA,           "SHT_RELA" },
        { s_type::SHT_HASH,           "SHT_HASH" },
        { s_type::SHT_DYNAMIC,        "SHT_DYNAMIC" },
        { s_type::SHT_NOTE,           "SHT_NOTE" },
        { s_type::SHT_NOBITS,         "SHT_NOBITS" },
        { s_type::SHT_REL,            "SHT_REL" },
        { s_type::SHT_SHLIB,          "SHT_SHLIB" },
        { s_type::SHT_DYNSYM,         "SHT_DYNSYM" },
        { s_type::SHT_INIT_ARRAY,     "SHT_INIT_ARRAY" },
        { s_type::SHT_FINI_ARRAY,     "SHT_FINI_ARRAY" },
        { s_type::SHT_PREINIT_ARRAY,  "SHT_PREINIT_ARRAY" },
        { s_type::SHT_GROUP,          "SHT_GROUP" },
        { s_type::SHT_SYMTAB_SHNDX,   "SHT_SYMTAB_SHNDX" },
        { s_type::SHT_NUM,            "SHT_NUM" },
        { s_type::SHT_QNXREL,         "SHT_QNXREL" },
        { s_type::SHT_GNU_ATTRIBUTES, "SHT_GNU_ATTRIBUTES" },
        { s_type::SHT_GNU_HASH,       "SHT_GNU_HASH" },
        { s_type::SHT_GNU_LIBLIST,    "SHT_GNU_LIBLIST" },
        { s_type::SHT_GNU_CHECKSUM,   "SHT_GNU_CHECKSUM" },
        { s_type::SHT_GNU_VERDEF,     "SHT_GNU_VERDEF" },
        { s_type::SHT_GNU_VERNEED,    "SHT_GNU_VERNEED" },
        { s_type::SHT_GNU_VERSYM,     "SHT_GNU_VERSYM" },
        { s_type::SHT_ARM_EXIDX,      "SHT_ARM_EXIDX" },
        { s_type::SHT_ARM_PREEMPMAP,  "SHT_ARM_PREEMPMAP" },
        { s_type::SHT_ARM_ATTRIBUTES, "SHT_ARM_ATTRIBUTES" },
    };

    constexpr std::size_t shdr32_name_offset      = offsetof(Elf32_Shdr, sh_name);
    constexpr std::size_t shdr32_type_offset      = offsetof(Elf32_Shdr, sh_type);
    constexpr std::size_t shdr32_flags_offset     = offsetof(Elf32_Shdr, sh_flags);
    constexpr std::size_t shdr32_addr_offset      = offsetof(Elf32_Shdr, sh_addr);
    constexpr std::size_t shdr32_offset_offset    = offsetof(Elf32_Shdr, sh_offset);
    constexpr std::size_t shdr32_size_offset      = offsetof(Elf32_Shdr, sh_size);
    constexpr std::size_t shdr32_link_offset      = offsetof(Elf32_Shdr, sh_link);
    constexpr std::size_t shdr32_info_offset      = offsetof(Elf32_Shdr, sh_info);
    constexpr std::size_t shdr32_addralign_offset = offsetof(Elf32_Shdr, sh_addralign);
    constexpr std::size_t shdr32_entsize_offset   = offsetof(Elf32_Shdr, sh_entsize);

    constexpr std::size_t shdr64_name_offset      = offsetof(Elf64_Shdr, sh_name);
    constexpr std::size_t shdr64_type_offset      = offsetof(Elf64_Shdr, sh_type);
    constexpr std::size_t shdr64_flags_offset     = offsetof(Elf64_Shdr, sh_flags);
    constexpr std::size_t shdr64_addr_offset      = offsetof(Elf64_Shdr, sh_addr);
    constexpr std::size_t shdr64_offset_offset    = offsetof(Elf64_Shdr, sh_offset);
    constexpr std::size_t shdr64_size_offset      = offsetof(Elf64_Shdr, sh_size);
    constexpr std::size_t shdr64_link_offset      = offsetof(Elf64_Shdr, sh_link);
    constexpr std::size_t shdr64_info_offset      = offsetof(Elf64_Shdr, sh_info);
    constexpr std::size_t shdr64_addralign_offset = offsetof(Elf64_Shdr, sh_addralign);
    constexpr std::size_t shdr64_entsize_offset   = offsetof(Elf64_Shdr, sh_entsize);

} // anonymous


Section::
Section(ElfFile const& elfFile, ElfImageView const& imageView)
: m_elfFile(&elfFile)
, m_imageView(imageView)
, m_is64bit(elfFile.is_64bit())
{
}


std::uint32_t Section::
name() const
{
    return m_imageView.get_uint32(shdr32_name_offset);
}

std::string Section::
name_string() const
{
    auto shstrndx = m_elfFile->elf_header().shstrndx();
    auto const& strtab = dynamic_cast<Section_STRTAB const&>(m_elfFile->section(shstrndx));
    return strtab.string(this->name());
}


s_type Section::
type() const
{
    return static_cast<s_type>(m_imageView.get_uint32(shdr32_type_offset));
}


std::string Section::
type_string() const
{
    auto it = std::find_if(type_name_mapping.begin(), type_name_mapping.end(),
                           [this](auto const& lhs) { return lhs.type_ == this->type(); });
    if (it != type_name_mapping.end())
    {
        return it->name_;
    }
    return "(unknown)";
}


std::uint64_t Section::
flags() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr64_flags_offset)
                     : m_imageView.get_uint32(shdr32_flags_offset);
}


std::uint64_t Section::
addr() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr64_addr_offset)
                     : m_imageView.get_uint32(shdr32_addr_offset);
}

std::uint64_t Section::
offset() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr64_offset_offset)
                     : m_imageView.get_uint32(shdr32_offset_offset);
}

std::uint64_t Section::
size() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr64_size_offset)
                     : m_imageView.get_uint32(shdr32_size_offset);
}

std::uint32_t Section::
link() const
{
    return m_is64bit ? m_imageView.get_uint32(shdr64_link_offset)
                     : m_imageView.get_uint32(shdr32_link_offset);
}

std::uint32_t Section::
info() const
{
    return m_is64bit ? m_imageView.get_uint32(shdr64_info_offset)
                     : m_imageView.get_uint32(shdr32_info_offset);
}

std::uint64_t Section::
addralign() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr32_addralign_offset)
                     : m_imageView.get_uint32(shdr32_addralign_offset);
}

std::uint64_t Section::
entsize() const
{
    return m_is64bit ? m_imageView.get_uint64(shdr64_entsize_offset)
                     : m_imageView.get_uint32(shdr32_entsize_offset);
}


std::ostream& Section::
printTo(std::ostream& ostr) const
{
    ostr << name_string() << '\t' << type_string()
         << '\t' << "offset=" << std::showbase << std::hex << offset()
         << '\t' << "size=" << std::dec << size()
         << '\n';
    return ostr;
}
