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
#ifndef EDHELIND_SYMBOL_H
#define EDHELIND_SYMBOL_H

#include "libedhel/detailable.h"
#include "libedhel/elffile.h"
#include "libedhel/elfimage.h"
#include <string>

using st_info_t = std::uint8_t;

/**
 * @defgroup st_bind Symbol Bindings
 * @{
 */
using st_bind_t = st_info_t;

constexpr inline st_bind_t STB_LOCAL  = 0;    /**< symbol has local binding */
constexpr inline st_bind_t STB_GLOBAL = 1;    /**< symbol has global binding */
constexpr inline st_bind_t STB_WEAK   = 2;    /**< symbol has weak binding */
constexpr inline st_bind_t STB_LOPROC = 13;   /**< start of processor-specific binding values */
constexpr inline st_bind_t STB_HIPROC = 15;   /**< end of processor-specific binding values */

/** @} */

/**
 * @defgroup st_type Symbol Types
 * @{
 */
using st_type_t = st_info_t;

constexpr inline st_type_t STT_NOTYPE   =  0; /**< symbol type is unspecified */
constexpr inline st_type_t STT_OBJECT   =  1; /**< symbol is a data object */
constexpr inline st_type_t STT_FUNC     =  2; /**< symbol is a code object */
constexpr inline st_type_t STT_SECTION  =  3; /**< symbol associated with a section */
constexpr inline st_type_t STT_FILE     =  4; /**< symbol's name is file name */
constexpr inline st_type_t STT_COMMON   =  5; /**< symbol is a common data object */
constexpr inline st_type_t STT_TLS      =  6; /**< symbol is thread-local data object*/
constexpr inline st_type_t STT_NUM      =  7; /**< number of defined types.  */
constexpr inline st_type_t STT_LOOS     = 10; /**< start of OS-specific */
constexpr inline st_type_t STT_GNU_IFUNC =10; /**< symbol is indirect code object */
constexpr inline st_type_t STT_HIOS     = 12; /**< end of OS-specific */
constexpr inline st_type_t STT_LOPROC   = 13; /**< start of processor-specific */
constexpr inline st_type_t STT_HIPROC   = 15; /**< end of processor-specific */

/** @} */

/**
 * @defgroup st_other Symbol Other
 * @{
 */
using st_other_t = std::uint8_t;

constexpr inline st_other_t STO_EXPORT    = 0x03; /**< mask for symbol visibility */
constexpr inline st_other_t STO_DEFAULT   = 0x00; /**< symbol is visible */
constexpr inline st_other_t STO_INTERNAL  = 0x01; /**< symbol is internal */
constexpr inline st_other_t STO_HIDDEN    = 0x02; /**< symbol is hidden */
constexpr inline st_other_t STO_PROTECTED = 0x03; /**< symbol is non-preemptable */
constexpr inline st_other_t STO_OPTIONAL  = 0x04; /**< symbol is optional */

/** @} */

/**
 * @defgroup st_shndx Symbol special st_shndx field values
 * The st_shndx field of a Symbol  usually indexes the section containing the
 * Symbol. However, there is a reserved range of indexes that indicate some
 * other special case.
 *
 * This, of course, gives a practical limit of 65279 unique sections in an elf
 * file.
 * @{
 */
using st_shndx_t = std::uint16_t;

constexpr inline st_shndx_t SHN_UNDEF     = 0x0000; /**< the symbol is undefined */
constexpr inline st_shndx_t SHN_LORESERVE = 0xff00; /**< start of reserved index values*/
constexpr inline st_shndx_t SHN_LOPROC    = 0xff00; /**< start of processor-specific values */
constexpr inline st_shndx_t SHN_HIPROC    = 0xff1f; /**< end of processor-specific values */
constexpr inline st_shndx_t SHN_LOOS      = 0xff20; /**< start of OS-specific values */
constexpr inline st_shndx_t SHN_HIOS      = 0xff3f; /**< start of OS-specific values */
constexpr inline st_shndx_t SHN_ABS       = 0xfff1; /**< symbol location is absolute*/
constexpr inline st_shndx_t SHN_COMMON    = 0xfff2; /**< start of a common block */
constexpr inline st_shndx_t SHN_HIRESERVE = 0xffff; /**< end of reserved index values */

/** @} */

/**
 * A single Symbol
 *
 * A Symbol is keyed by (name, type).
 */
class Symbol
: public Detailable
{
public:
    Symbol(ElfFile const& elf_file, ElfImageView const& image_view, std::uint32_t strndx);

    std::uint32_t
    name() const;

    std::string
    name_string() const;

    std::uint64_t
    value() const;

    std::uint64_t
    size() const;

    st_shndx_t
    shndx() const;

    std::string
    shndx_string() const;

    st_info_t
    info() const;

    st_bind_t
    bind() const;

    std::string const&
    bind_string() const;

    st_type_t
    type() const;

    std::string const&
    type_string() const;

    st_other_t
    other() const;

    std::string const&
    other_string() const;

    std::ostream&
    printTo(std::ostream& ostr) const override;

private:
    virtual std::uint32_t
    get_name() const = 0;

    virtual std::uint64_t
    get_value() const= 0;

    virtual std::uint64_t
    get_size() const = 0;

    virtual st_shndx_t
    get_shndx() const = 0;

    virtual st_info_t
    get_info() const = 0;

    virtual st_other_t
    get_other() const = 0;

protected:
    ElfFile const* elf_file_;
    ElfImageView   image_view_;
    std::uint32_t  strndx_;
};


/**
 * Factory function to create a Symbol from an ElfImageView on an ElfFile
 */
std::unique_ptr<Symbol>
make_symbol(ElfFile const& elf_file, ElfImageView const& image_view, std::uint32_t strndx);

#endif /* EDHELIND_SYMBOL_H */

