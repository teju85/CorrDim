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


#ifndef __INCLUDED_BASICS_H__
#define __INCLUDED_BASICS_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#define REAL double



#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>


template <typename T>
bool from_string(T& val, const std::string& str) {
    std::istringstream iss(str);
    return !(iss >> val).fail();
}
template <typename T>
bool from_string(T& val, const char* str) {
    std::istringstream iss(str);
    return !(iss >> val).fail();
}


#define OPTION_CHECK(opt, i, argc)                                      \
    i++;                                                                \
    if(i >= argc) {                                                     \
        fprintf(stderr, "Option '%s' requires an argument!\n", opt);    \
        exit(1);                                                        \
    }

#define GET_INTEGER(var, opt, arg)                                      \
    if(!from_string<int>(var, arg)) {                                   \
        fprintf(stderr, "Argument to '%s' must be an integer!\n", opt); \
        exit(1);                                                        \
    }

#define GET_NUMBER(var, opt, arg)                                       \
    if(!from_string<REAL>(var, arg)) {                                  \
        fprintf(stderr, "Argument to '%s' must be a number!\n", opt);   \
        exit(1);                                                        \
    }

#define CHECK_POSITIVE(var, opt)                                        \
    if(var <= 0) {                                                      \
        fprintf(stderr, "Argument to '%s' must be positive!\n", opt);   \
        exit(1);                                                        \
    }

#define CHECK_NEGATIVE(var, opt)                                        \
    if(var > 0) {                                                       \
        fprintf(stderr, "Argument to '%s' must be positive!\n", opt);   \
        exit(1);                                                        \
    }

#define CHECK_RANGE(var, opt, min, max)                                 \
    if((var < min) || (var > max)) {                                    \
        fprintf(stderr, "Range for the argument to '%s' is [%f,%f]!\n", \
                opt, min, max);                                         \
        exit(1);                                                        \
    }

#define CHECK_RANGE_INT(var, opt, min, max)                             \
    if((var < min) || (var > max)) {                                    \
        fprintf(stderr, "Range for the argument to '%s' is [%d,%d]!\n", \
                opt, min, max);                                         \
        exit(1);                                                        \
    }

/**
 * @brief Reads the file 'maps.list' to generate the list of maps supported
 * @return list of maps
 */
std::vector<std::string> listMaps();


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_BASICS_H__

