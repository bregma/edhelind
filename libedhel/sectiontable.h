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
#ifndef EDHELIND_SECTIONTABLE_H
#define EDHELIND_SECTIONTABLE_H

#include "libedhel/elfimage.h"
#include <functional>
#include <memory>
#include <vector>


class ElfFile;
class Section;

class SectionTable
{
public:
    SectionTable();
    SectionTable(ElfFile const& elfFile);

    ~SectionTable();

    SectionTable(SectionTable const&) = delete;
    SectionTable& operator=(SectionTable const&) = delete;

    /*! Get indicated section */
    Section const&
    section(std::uint32_t index) const;

    void
    iterate_sections(std::function<void(Section const&)>) const;

private:
    using OwningSectionPtr = std::unique_ptr<Section>;

    ElfImageView                  image_view_;
    std::vector<OwningSectionPtr> sections_;
};

#endif /* EDHELIND_SECTIONTABLE_H */
