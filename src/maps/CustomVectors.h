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


#ifndef __INCLUDED_CUSTOMVECTORS_H__
#define __INCLUDED_CUSTOMVECTORS_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "ChaoticMap.h"


/**
 * Class to generate numbers from the given file.
 */
class CustomVectors : public ChaoticMap {
public:
    /**
     * @brief generate the first 'numEle' vectors from the given map
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
    int getDimension() { return m_dim; }

protected:
    /**
     * @brief Print help message on usage of this class and exit
     */
    void showHelp();

private:
    int m_dim;   ///< dimension of the data
};


REAL* customVectors(int numEle, int dimension, int pos, int argc, char** argv);


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_CUSTOMVECTORS_H__
