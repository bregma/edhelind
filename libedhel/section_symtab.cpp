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
#include "libedhel/section_symtab.h"

#include "libedhel/elffile.h"
#include <iomanip>
#include <iostream>


Section_SYMTAB::
Section_SYMTAB(ElfFile const& elf_file, ElfImageView const& image_view)
: Section(elf_file, image_view)
{
    const std::size_t symbol_size = elf_file.is_64bit() ? sizeof(Elf64::Sym) : sizeof(Elf32::Sym);
    for (std::size_t offset = 0; offset < this->size(); offset += symbol_size)
    {
        symbol_table_.emplace_back(make_symbol(elf_file,
                                               elf_file.view(this->offset() + offset, symbol_size),
                                               this->link()));
    }
}


Symbol const& Section_SYMTAB::
symbol(std::uint32_t index) const
{
    return *symbol_table_.at(index);
}


void Section_SYMTAB::
iterate_symbols(std::function<void(Symbol const&)> visit) const
{
    for (auto const& symbol: symbol_table_)
    {
        visit(*symbol);
    }
}


std::ostream& Section_SYMTAB::
printDetailTo(std::ostream& ostr) const
{
    using std::setw;
    using std::left;

    ostr << " " << setw(10) << std::left << "Value"
         << " " << setw(10) << std::left << "Size"
         << " " << setw(6)  << std::left << "Bind"
         << " " << setw(7)  << std::left << "Type"
         << " " << setw(9)  << std::left << "Vis"
         << " " << setw(6)  << std::left << "Index"
         << " " << "Name"
         << "\n";
    this->iterate_symbols([&](Symbol const& symbol){
        ostr << symbol << "\n";
    });
    return ostr;
}
