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


#ifndef __INCLUDED_HENONMAP_H__
#define __INCLUDED_HENONMAP_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "ChaoticMap.h"


/** default value of parameter a */
#define HENON_A      1.12
/** default value of parameter b */
#define HENON_B      0.12
/** default initial value x0 */
#define HENON_X0     1.44
/** default initial value y0 */
#define HENON_Y0     0.89


/**
 * Class to generate numbers from the HenonMap.
 */
class HenonMap : public ChaoticMap {
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

    /**
     * @brief Tells the dimension of each vector for this map
     * @return dimension
     */
    int getDimension() { return 2; }

protected:
    /**
     * @brief Print help message on usage of this class and exit
     */
    void showHelp();

private:
    /**
     * @brief Generate the first 'k' numbers from the HenonMap.
     * @param a the parameter of the HenonMap for x-values.
     * @param b the parameter of the HenonMap for y-values.
     * @param x0 the seed for the HenonMap.
     * @param y0 the seed for the HenonMap.
     * @param k generate the first 'k' numbers.
     * @return an array of first 'k' numbers from the HenonMap.
     *
     * 'x0' must be between 0 and 1.
     * The returned array is the responsibility of the caller. The caller
     * should 'delete' the memory pointer!
     */
    REAL* generate(REAL a, REAL b, REAL x0, REAL y0, int k);
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_HENONMAP_H__

