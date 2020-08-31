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
#ifndef EDHELIND_DETAILABLE_H
#define EDHELIND_DETAILABLE_H

#include <iosfwd>


/**
 * Mix-in class to make something get its details printed.
 */
class Detailable
{
public:
    friend std::ostream&
    operator<<(std::ostream& ostr, Detailable const& detailable)
    {
        return detailable.printTo(ostr);
    }

private:
    virtual std::ostream&
    printTo(std::ostream&) const = 0;

};



#endif /* EDHELIND_DETAILABLE_H */
