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
#ifndef EDHELIND_SECTION_SYMTAB_H
#define EDHELIND_SECTION_SYMTAB_H

#include <functional>
#include "libedhel/section.h"
#include "libedhel/symbol.h"
#include <vector>


/**
 * An SHT_SYMTAB section
 */
class Section_SYMTAB
: public Section
{
public:
    Section_SYMTAB(ElfFile const& elf_file, ElfImageView const& image_view);

    /** Retrieve the symbol at @p index */
    Symbol const&
    symbol(std::uint32_t index) const;

    /**
     * Visit each symbol in the symbol table
     */
    void
    iterate_symbols(std::function<void(Symbol const&)> visit) const;

private:
    std::ostream&
    printDetailTo(std::ostream& ostr) const override;

private:
    std::vector<Symbol> symbol_table_;
};

#endif /* EDHELIND_SECTION_SYMTAB_H */
