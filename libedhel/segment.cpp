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
#include "libedhel/segment.h"

#include <iomanip>
#include <iostream>
#include "libedhel/elffile.h"
#include <sstream>
#include <vector>


namespace {

    struct TypeNameMapping {
        PType       type_;
        std::string name_;
    };

    std::vector<TypeNameMapping> type_name_mapping {
        { PType::PT_NULL,    "PT_NULL"    },
        { PType::PT_LOAD,    "PT_LOAD"    },
        { PType::PT_DYNAMIC, "PT_DYNAMIC" },
        { PType::PT_INTERP,  "PT_INTERP"  },
        { PType::PT_NOTE,    "PT_NOTE"    },
        { PType::PT_SHLIB,   "PT_SHLIB"   },
        { PType::PT_PHDR,    "PT_PHDR"    },
        { PType::PT_TLS,     "PT_TLS"     },
    };

    struct FlagsNameMapping {
        PFlags      flag_;
        std::string name_;
    };

    const std::vector<FlagsNameMapping> flags_name_mapping {
        { FP_X, "FP_X" },
        { FP_W, "FP_W" },
        { FP_R, "FP_R" },
    };

    constexpr std::size_t phdr32_type_offset   = offsetof(Elf32_Phdr, p_type);
    constexpr std::size_t phdr32_offset_offset = offsetof(Elf32_Phdr, p_offset);
    constexpr std::size_t phdr32_vaddr_offset  = offsetof(Elf32_Phdr, p_vaddr);
    constexpr std::size_t phdr32_paddr_offset  = offsetof(Elf32_Phdr, p_paddr);
    constexpr std::size_t phdr32_filesz_offset = offsetof(Elf32_Phdr, p_filesz);
    constexpr std::size_t phdr32_memsz_offset  = offsetof(Elf32_Phdr, p_memsz);
    constexpr std::size_t phdr32_flags_offset  = offsetof(Elf32_Phdr, p_flags);
    constexpr std::size_t phdr32_align_offset  = offsetof(Elf32_Phdr, p_align);

    constexpr std::size_t phdr64_type_offset   = offsetof(Elf64_Phdr, p_type);
    constexpr std::size_t phdr64_offset_offset = offsetof(Elf64_Phdr, p_offset);
    constexpr std::size_t phdr64_vaddr_offset  = offsetof(Elf64_Phdr, p_vaddr);
    constexpr std::size_t phdr64_paddr_offset  = offsetof(Elf64_Phdr, p_paddr);
    constexpr std::size_t phdr64_filesz_offset = offsetof(Elf64_Phdr, p_filesz);
    constexpr std::size_t phdr64_memsz_offset  = offsetof(Elf64_Phdr, p_memsz);
    constexpr std::size_t phdr64_flags_offset  = offsetof(Elf64_Phdr, p_flags);
    constexpr std::size_t phdr64_align_offset  = offsetof(Elf64_Phdr, p_align);
} // anonymous


Segment::
Segment(ElfFile const& elf_file, ElfImageView const& image_view)
: elf_file_(&elf_file)
, image_view_(image_view)
, is_64bit_(elf_file_->is_64bit())
{
}


PType Segment::
type() const
{
    return static_cast<PType>(image_view_.get_uint32(phdr32_type_offset));
}


std::string Segment::
type_string() const
{
    using std::ostringstream;
    using std::showbase;
    using std::hex;
    using std::underlying_type;

    PType type{this->type()};
    for (auto const& it: type_name_mapping) {
        if (it.type_ == type) {
            return it.name_;
        }
    }
    ostringstream ostr;
    ostr << "unknown ("
         << showbase << hex << static_cast<underlying_type<EhType>::type>(type)
         << ")";
    return ostr.str();
}


PFlags Segment::
flags() const
{
    return this->is_64bit_ ? static_cast<PFlags>(image_view_.get_uint32(phdr64_flags_offset))
                           : static_cast<PFlags>(image_view_.get_uint32(phdr32_flags_offset));
}


std::string Segment::
flags_string() const
{
    using std::ostringstream;
    using std::setw;
    using std::setfill;
    using std::hex;

    ostringstream ostr;
    PFlags flags = this->flags();
    std::string sep = " (";
    ostr << "0x" << setw(8) << setfill('0') << hex << flags;
    for (auto const& f: flags_name_mapping)
    {
        if (f.flag_ & flags)
        {
            ostr << sep << f.name_;
            sep = ", ";
        }
    }
    if (sep != " (")
    {
        ostr << ")";
    }
    return ostr.str();
}


std::uint64_t Segment::
offset() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_offset_offset)
                           : image_view_.get_uint32(phdr32_offset_offset);
}


std::uint64_t Segment::
vaddr() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_vaddr_offset)
                           : image_view_.get_uint32(phdr32_vaddr_offset);
}


std::uint64_t Segment::
paddr() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_paddr_offset)
                           : image_view_.get_uint32(phdr32_paddr_offset);
}


std::uint64_t Segment::
filesz() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_filesz_offset)
                           : image_view_.get_uint32(phdr32_filesz_offset);
}


std::uint64_t Segment::
memsz() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_memsz_offset)
                           : image_view_.get_uint32(phdr32_memsz_offset);
}


std::uint64_t Segment::
align() const
{
    return this->is_64bit_ ? image_view_.get_uint64(phdr64_align_offset)
                           : image_view_.get_uint32(phdr32_align_offset);
}

std::ostream& Segment::
printTo(std::ostream& ostr) const
{
    ostr << "Segment " << this->type_string() << "\n";
    return printDetailTo(ostr);
}


std::ostream& Segment::
printDetailTo(std::ostream& ostr) const
{
    return ostr;
}



