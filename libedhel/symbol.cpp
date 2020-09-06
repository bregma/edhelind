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
#include "libedhel/symbol.h"

#include <iomanip>
#include <iostream>
#include "libedhel/elf.h"
#include "libedhel/section_strtab.h"
#include <vector>


namespace
{
    struct st_shndx_name_t
    {
        st_shndx_t  shndx_;
        std::string name_;
    };
    std::vector<st_shndx_name_t> shndx_name_mapping{
        { SHN_ABS,    "ABS"    },
        { SHN_COMMON, "COMMON" },
    };
    const std::string st_shndx_other{"OTHER"};
    const std::string st_shndx_undef{"UNDEF"};

    struct st_bind_name_t
    {
        st_bind_t   bind_;
        std::string name_;
    };
    std::vector<st_bind_name_t> bind_name_mapping{
        { STB_LOCAL,  "LOCAL"  },
        { STB_GLOBAL, "GLOBAL" },
        { STB_WEAK,   "WEAK"   },
    };
    const std::string st_bind_other{"NONE"};

    struct st_type_name_t
    {
        st_type_t   type_;
        std::string name_;
    };
    std::vector<st_type_name_t> type_name_mapping{
        { STT_NOTYPE,  "NOTYPE"  },
        { STT_OBJECT,  "OBJECT"  },
        { STT_FUNC,    "FUNC"    },
        { STT_SECTION, "SECTION" },
        { STT_FILE,    "FILE"    },
        { STT_COMMON,  "COMMON"  },
        { STT_TLS,     "TLS"     },
        { STT_NUM,     "NUM"     },
    };
    const std::string st_type_other{"(OTHER)"};

    struct st_other_name_t
    {
        st_other_t  other_;
        std::string name_;
    };
    std::vector<st_other_name_t> other_name_mapping{
        { STO_DEFAULT,   "DEFAULT"  },
        { STO_INTERNAL,  "INTERNAL" },
        { STO_HIDDEN,    "HIDDEN"   },
        { STO_PROTECTED, "PROT"     },
    };
    const std::string st_other_other{"(OTHER)"};

    constexpr std::size_t st32_name_offset  = offsetof(Elf32_Sym, st_name);
    constexpr std::size_t st32_value_offset = offsetof(Elf32_Sym, st_value);
    constexpr std::size_t st32_size_offset  = offsetof(Elf32_Sym, st_size);
    constexpr std::size_t st32_info_offset  = offsetof(Elf32_Sym, st_info);
    constexpr std::size_t st32_other_offset = offsetof(Elf32_Sym, st_other);
    constexpr std::size_t st32_shndx_offset = offsetof(Elf32_Sym, st_shndx);

    constexpr std::size_t st64_name_offset  = offsetof(Elf64_Sym, st_name);
    constexpr std::size_t st64_value_offset = offsetof(Elf64_Sym, st_value);
    constexpr std::size_t st64_size_offset  = offsetof(Elf64_Sym, st_size);
    constexpr std::size_t st64_info_offset  = offsetof(Elf64_Sym, st_info);
    constexpr std::size_t st64_other_offset = offsetof(Elf64_Sym, st_other);
    constexpr std::size_t st64_shndx_offset = offsetof(Elf64_Sym, st_shndx);

} // anonymous


Symbol::
Symbol(ElfFile const& elf_file, ElfImageView const& image_view, std::uint32_t strndx)
: elf_file_{&elf_file}
, image_view_{image_view}
, strndx_{strndx}
{
}


std::uint32_t Symbol::
name() const
{
    return elf_file_->is_64bit() ? image_view_.get_uint32(st64_name_offset)
                                 : image_view_.get_uint32(st32_name_offset);
}


std::string Symbol::
name_string() const
{
    auto name = this->name();
    if (name)
    {
        auto const& strtab = dynamic_cast<Section_STRTAB const&>(elf_file_->section(strndx_));
        return strtab.string(name);
    }
    return "";
}


std::uint64_t Symbol::
value() const
{
    return elf_file_->is_64bit() ? image_view_.get_uint64(st64_value_offset)
                                 : image_view_.get_uint32(st32_value_offset);
}


std::uint64_t Symbol::
size() const
{
    return elf_file_->is_64bit() ? image_view_.get_uint64(st64_size_offset)
                                 : image_view_.get_uint32(st32_size_offset);
}


st_shndx_t Symbol::
shndx() const
{
    return elf_file_->is_64bit() ? image_view_.get_uint16(st64_shndx_offset)
                                 : image_view_.get_uint16(st32_shndx_offset);
}


std::string Symbol::
shndx_string() const
{
    auto shndx = this->shndx();
    if (shndx == SHN_UNDEF)
    {
        return st_shndx_undef;
    }

    if ((shndx & SHN_LORESERVE) == 0)
    {
        return std::to_string(shndx);
    }

    for (auto const& mapping: shndx_name_mapping)
    {
        if (shndx == mapping.shndx_)
        {
            return mapping.name_;
        }
    }
    return st_shndx_other;
}


st_info_t Symbol::
info() const
{
    return elf_file_->is_64bit() ? image_view_.get_uint8(st64_info_offset)
                                 : image_view_.get_uint8(st32_info_offset);
}


st_bind_t Symbol::
bind() const
{
    return this->info() >> 4;
}


std::string const& Symbol::
bind_string() const
{
    auto bind = this->bind();
    for (auto const& mapping:bind_name_mapping)
    {
        if (bind == mapping.bind_)
        {
            return mapping.name_;
        }
    }
    return st_bind_other;
}


st_type_t Symbol::
type() const
{
    return this->info() & 0xf;
}


std::string const& Symbol::
type_string() const
{
    auto type = this->type();
    for (auto const& mapping:type_name_mapping)
    {
        if (type == mapping.type_)
        {
            return mapping.name_;
        }
    }
    return st_type_other;
}


st_other_t Symbol::
other() const
{
    return 0;
}


std::string const& Symbol::
other_string() const
{
    auto other = this->other();
    for (auto const& mapping:other_name_mapping)
    {
        if ((other & mapping.other_) == mapping.other_)
        {
            return mapping.name_;
        }
    }
    return st_other_other;
}


std::ostream& Symbol::
printTo(std::ostream& ostr) const
{
    using std::hex;
    using std::left;
    using std::internal;
    using std::setfill;
    using std::setw;
    using std::showbase;

    ostr << " " << setw(10) << setfill('0') << internal << hex << showbase << this->value()
         << " " << setw(10) << setfill('0') << internal << hex << showbase << this->size()
         << " " << setw(6)  << setfill(' ') << left << this->bind_string()
         << " " << setw(7)  << setfill(' ') << left << this->type_string()
         << " " << setw(9)  << setfill(' ') << left << this->other_string()
         << " " << setw(6)  << setfill(' ') << left << this->shndx_string()
         << " " << name_string();
    return ostr;
}


