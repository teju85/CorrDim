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


#ifndef __INCLUDED_LOGISTICMAP_H__
#define __INCLUDED_LOGISTICMAP_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "ChaoticMap.h"


/** default value for lambda */
#define LOGISTIC_LAMBDA   3.97
/** initial value for the map */
#define LOGISTIC_X0       0.44


/**
 * Class to generate numbers from the LogisticMap.
 */
class LogisticMap : public ChaoticMap {
public:
    /**
     * @brief generate the first 'numEle' vectors from this ChaoticMap
     * @param numEle number of elements in the output vector
     * @param pos start of the arguments in 'argv'
     * @param argc total number of ALL commandline arguments.
     * @param argv list of ALL commandline arguments.
     * @return the desired vector
     */
    REAL* generateVectors(int numEle, int pos, int argc, char** argv);

protected:
    /**
     * @brief Print help message on usage of this class and exit
     */
    void showHelp();

private:
    /**
     * @brief Generate the first 'k' numbers from the LogisticMap.
     * @param lambda the parameter of the LogisticMap.
     * @param x0 the seed for the LogisticMap.
     * @param k generate the first 'k' numbers.
     * @return an array of first 'k' numbers from the LogisticMap.
     *
     * 'x0' must be between 0 and 1.
     * The returned array is the responsibility of the caller. The caller
     * should 'delete' the memory pointer!
     */
    REAL* generate(REAL lambda, REAL x0, int k);
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_LOGISTICMAP_H__
