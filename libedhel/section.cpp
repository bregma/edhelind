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
#include <iomanip>
#include <iostream>
#include <sstream>
#include "section_strtab.h"


namespace
{
    /*! map sh_type to a well-known string */
    struct TypeNameMapping {
        enum SType type_;
        std::string name_;
    }; 

    std::vector<TypeNameMapping> type_name_mapping {
        { SType::SHT_NULL,           "SHT_NULL" },
        { SType::SHT_PROGBITS,       "SHT_PROGBITS" },
        { SType::SHT_SYMTAB,         "SHT_SYMTAB" },
        { SType::SHT_STRTAB,         "SHT_STRTAB" },
        { SType::SHT_RELA,           "SHT_RELA" },
        { SType::SHT_HASH,           "SHT_HASH" },
        { SType::SHT_DYNAMIC,        "SHT_DYNAMIC" },
        { SType::SHT_NOTE,           "SHT_NOTE" },
        { SType::SHT_NOBITS,         "SHT_NOBITS" },
        { SType::SHT_REL,            "SHT_REL" },
        { SType::SHT_SHLIB,          "SHT_SHLIB" },
        { SType::SHT_DYNSYM,         "SHT_DYNSYM" },
        { SType::SHT_INIT_ARRAY,     "SHT_INIT_ARRAY" },
        { SType::SHT_FINI_ARRAY,     "SHT_FINI_ARRAY" },
        { SType::SHT_PREINIT_ARRAY,  "SHT_PREINIT_ARRAY" },
        { SType::SHT_GROUP,          "SHT_GROUP" },
        { SType::SHT_SYMTAB_SHNDX,   "SHT_SYMTAB_SHNDX" },
        { SType::SHT_NUM,            "SHT_NUM" },
        { SType::SHT_QNXREL,         "SHT_QNXREL" },
        { SType::SHT_GNU_ATTRIBUTES, "SHT_GNU_ATTRIBUTES" },
        { SType::SHT_GNU_HASH,       "SHT_GNU_HASH" },
        { SType::SHT_GNU_LIBLIST,    "SHT_GNU_LIBLIST" },
        { SType::SHT_GNU_CHECKSUM,   "SHT_GNU_CHECKSUM" },
        { SType::SHT_GNU_VERDEF,     "SHT_GNU_VERDEF" },
        { SType::SHT_GNU_VERNEED,    "SHT_GNU_VERNEED" },
        { SType::SHT_GNU_VERSYM,     "SHT_GNU_VERSYM" },
        { SType::SHT_ARM_EXIDX,      "SHT_ARM_EXIDX" },
        { SType::SHT_ARM_PREEMPMAP,  "SHT_ARM_PREEMPMAP" },
        { SType::SHT_ARM_ATTRIBUTES, "SHT_ARM_ATTRIBUTES" },
    };

    struct FlagNameMapping {
        int         flag_mask_;
        std::string name_;
    };

    const std::vector<FlagNameMapping> flag_name_mapping {
        { Elf::SHF_WRITE,      "WRITE" },
        { Elf::SHF_ALLOC,      "ALLOC" },
        { Elf::SHF_EXECINSTR,  "EXEC" },
        { Elf::SHF_MERGE,      "MERGE" },
        { Elf::SHF_STRINGS,    "STRINGS" },
        { Elf::SHF_GROUP,      "GROUP" },
        { Elf::SHF_TLS,        "TLS" },
        { Elf::SHF_COMPRESSED, "COMPRESSED" },
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
Section(ElfFile const& elf_file, ElfImageView const& image_view)
: elf_file_(&elf_file)
, image_view_(image_view)
, is_64bit_(elf_file.is_64bit())
{
}


std::uint32_t Section::
name() const
{
    return image_view_.get_uint32(shdr32_name_offset);
}

std::string Section::
name_string() const
{
    auto shstrndx = elf_file_->elf_header().shstrndx();
    auto const& strtab = dynamic_cast<Section_STRTAB const&>(elf_file_->section(shstrndx));
    return strtab.string(this->name());
}


SType Section::
type() const
{
    return static_cast<SType>(image_view_.get_uint32(shdr32_type_offset));
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
    return is_64bit_ ? image_view_.get_uint64(shdr64_flags_offset)
                     : image_view_.get_uint32(shdr32_flags_offset);
}


std::string Section::
flags_string() const
{
    using std::setw;
    using std::setfill;
    using std::showbase;
    using std::hex;

    std::ostringstream ostr;
    std::uint64_t flags = this->flags();
    std::string sep{" "};

    ostr << "0x" << hex << setfill('0') << setw(8) << flags;
    for (auto const& f: flag_name_mapping)
    {
        if (flags & f.flag_mask_)
        {
            ostr << sep << f.name_;
            sep = ",";
        }
    }
    return ostr.str();
}


std::uint64_t Section::
addr() const
{
    return is_64bit_ ? image_view_.get_uint64(shdr64_addr_offset)
                     : image_view_.get_uint32(shdr32_addr_offset);
}

std::uint64_t Section::
offset() const
{
    return is_64bit_ ? image_view_.get_uint64(shdr64_offset_offset)
                     : image_view_.get_uint32(shdr32_offset_offset);
}

std::uint64_t Section::
size() const
{
    return is_64bit_ ? image_view_.get_uint64(shdr64_size_offset)
                     : image_view_.get_uint32(shdr32_size_offset);
}

std::uint32_t Section::
link() const
{
    return is_64bit_ ? image_view_.get_uint32(shdr64_link_offset)
                     : image_view_.get_uint32(shdr32_link_offset);
}

std::uint32_t Section::
info() const
{
    return is_64bit_ ? image_view_.get_uint32(shdr64_info_offset)
                     : image_view_.get_uint32(shdr32_info_offset);
}

std::uint64_t Section::
addralign() const
{
    return is_64bit_ ? image_view_.get_uint64(shdr32_addralign_offset)
                     : image_view_.get_uint32(shdr32_addralign_offset);
}

std::uint64_t Section::
entsize() const
{
    return is_64bit_ ? image_view_.get_uint64(shdr64_entsize_offset)
                     : image_view_.get_uint32(shdr32_entsize_offset);
}


std::ostream& Section::
printTo(std::ostream& ostr) const
{
    ostr << name_string() << '\t' << type_string()
         << '\t' << "offset=" << std::showbase << std::hex << offset()
         << '\t' << "size=" << std::dec << size()
         << '\n';
    return printDetailTo(ostr);
}

std::ostream& Section::
printDetailTo(std::ostream& ostr) const
{
    return ostr;
}
