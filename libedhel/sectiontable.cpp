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
#include "libedhel/sectiontable.h"

#include "libedhel/elffile.h"
#include "libedhel/section.h"
#include "libedhel/section_strtab.h"
#include <stdexcept>


SectionTable::
SectionTable()
{
}


SectionTable::
SectionTable(ElfFile const& elfFile)
: image_view_(elfFile.view(elfFile.elf_header().shoff(),
                           elfFile.elf_header().shnum() * elfFile.elf_header().shentsize()))
{
    std::size_t shoff = 0;
    std::size_t shentsize = elfFile.elf_header().shentsize();
    for (auto i = 0; i < elfFile.elf_header().shnum(); ++i)
    {
        auto sectionView = image_view_.view(shoff, shentsize);
        Section tmpSection(elfFile, sectionView);
        switch (tmpSection.type())
        {
            case s_type::SHT_STRTAB:
                sections_.emplace_back(std::make_unique<Section_STRTAB>(elfFile, sectionView));
                break;

            default:
                sections_.emplace_back(std::make_unique<Section>(elfFile, sectionView));
                break;
        }
        
        shoff += shentsize;
    }
}


/*!
 * Destroy a @pSectionTable
 *
 * This destructor is explicitly defined in the file so the vector of
 * unique_ptrs will have some place to go to die.
 */
SectionTable::
~SectionTable()
{
}


Section const& SectionTable::
section(std::uint32_t index) const
{
    if (index >= sections_.size())
    {
        throw std::out_of_range("section index out of range");
    }

    return *sections_[index];
}


void SectionTable::
iterate_sections(std::function<void(Section const&)> visit) const
{
    for (auto const& section: sections_)
    {
        visit(*section);
    }
}
