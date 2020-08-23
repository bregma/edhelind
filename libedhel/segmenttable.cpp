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
#include "libedhel/segmenttable.h"

#include "libedhel/elffile.h"
#include "libedhel/segment.h"
#include <stdexcept>


SegmentTable::
SegmentTable()
{
}


SegmentTable::
SegmentTable(ElfFile const& elfFile)
: image_view_(elfFile.view(elfFile.elf_header().phoff(),
                           elfFile.elf_header().phnum() * elfFile.elf_header().phentsize()))
{
    std::size_t phoff = 0;
    std::size_t phentsize = elfFile.elf_header().phentsize();
    for (auto i = 0; i < elfFile.elf_header().phnum(); ++i)
    {
        auto segmentView = image_view_.view(phoff, phentsize);
        Segment tmpSegment(elfFile, segmentView);
        segments_.emplace_back(std::make_unique<Segment>(elfFile, segmentView));
        
        phoff += phentsize;
    }
}


/*!
 * Destroy a @pSegmentTable
 *
 * This destructor is explicitly defined in the file so the vector of
 * unique_ptrs will have some place to go to die.
 */
SegmentTable::
~SegmentTable()
{
}


Segment const& SegmentTable::
segment(std::uint32_t index) const
{
    if (index >= segments_.size())
    {
        throw std::out_of_range("segment index out of range");
    }

    return *segments_[index];
}


void SegmentTable::
iterate_segments(std::function<void(Segment const&)> visit) const
{
    for (auto const& segment: segments_)
    {
        visit(*segment);
    }
}
