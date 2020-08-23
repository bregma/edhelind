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
#ifndef EDHELIND_SEGMENTTABLE_H
#define EDHELIND_SEGMENTTABLE_H

#include "libedhel/elfimage.h"
#include <functional>
#include <memory>
#include <vector>


class ElfFile;
class Segment;

class SegmentTable
{
public:
    SegmentTable();
    SegmentTable(ElfFile const& elfFile);

    ~SegmentTable();

    SegmentTable(SegmentTable const&) = delete;
    SegmentTable& operator=(SegmentTable const&) = delete;

    /*! Get indicated segment */
    Segment const&
    segment(std::uint32_t index) const;

    /** Pretty much the classic dl_iterate_phdr() */
    void
    iterate_segments(std::function<void(Segment const&)>) const;

private:
    using OwningSegmentPtr = std::unique_ptr<Segment>;

    ElfImageView                  image_view_;
    std::vector<OwningSegmentPtr> segments_;
};

#endif /* EDHELIND_SEGMENTTABLE_H */
