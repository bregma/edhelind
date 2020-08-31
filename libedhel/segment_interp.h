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
#ifndef EDHELIND_SEGMENT_INTERP_H
#define EDHELIND_SEGMENT_INTERP_H

#include "libedhel/segment.h"


/*!
 * A PH_INTERP segment
 */
class Segment_INTERP
: public Segment
{
public:
    Segment_INTERP(ElfFile const& elf_file, ElfImageView const& image_view);

    /** Retrieve the interpreter string */
    std::string
    interp() const;

private:
    std::ostream&
    printDetailTo(std::ostream& ostr) const override;

private:
    std::string interp_;
};

#endif /* EDHELIND_SEGMENT_INTERP_H */
