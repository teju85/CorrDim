/***************************************************************************\
Tool to find the correlation dimension of a sequence.
Copyright (C) 2010 Tejaswi.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
\***************************************************************************/


#ifndef __INCLUDED_CHAOTICMAP_H__
#define __INCLUDED_CHAOTICMAP_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "basics.h"
#include "Timer.h"


/**
 * Base class to generate numbers from the a chaotic map.
 */
class ChaoticMap {
public:
    /**
     * @brief Constructor of this class.
     */
    ChaoticMap() {}

    /**
     * @brief Destructor of this class.
     */
    virtual ~ChaoticMap() {}

    /**
     * @brief generate the first 'numEle' vectors from this ChaoticMap
     * @param numEle number of elements in the output vector
     * @param pos start of the arguments in 'argv'
     * @param argc total number of ALL commandline arguments.
     * @param argv list of ALL commandline arguments.
     * @return the desired vector
     */
    virtual REAL* generateVectors(int numEle, int pos, int argc, char** argv) = 0;

    /**
     * @brief Tells the dimension of each vector for this map
     * @return dimension
     */
    virtual int getDimension() { return 1; }

protected:
    /**
     * @brief Print help message on usage of this class and exit
     */
    virtual void showHelp() = 0;
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_CHAOTICMAP_H__

