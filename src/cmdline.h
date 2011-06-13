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


#ifndef __INCLUDED_CMDLINE_H__
#define __INCLUDED_CMDLINE_H__

#ifndef __cplusplus
#error A C++ compiler is required to compile this code!
#endif


#include "basics.h"
#include "maps/ChaoticMap.h"


/** default value of number of points to be discarded on log(CR) vs log(R) graph from the left most point */
#define NUM_DISCARD_L   5
/** default value of number of points to be discarded on log(CR) vs log(R) graph from the right most point */
#define NUM_DISCARD_R   2
/** default value of number of points on the log(CR) vs log(R) graph */
#define NUM_POINTS      25
/** default value of number of vectors considered for the correlation dimension calculation */
#define NUM_ELEMENTS    2000
/** default value of number of histogram bins for distance matrix */
#define NUM_BINS        25
/** default map to be worked upon */
#define DEFAULT_MAP     "LogisticMap"


/**
 * @brief Function to check whether a correct map is specified on commandline
 * @param list list of maps to be compared against
 * @param param the map passed via commandline
 */
void validateMap(std::vector<std::string>& list, const std::string& param);

/**
 * @brief Prints the global help and exits
 * @param list list of all maps currently supported
 */
void showHelp(const std::vector<std::string>& list);


/**
 * Class to store all the cmdline args
 */
class CmdLine {
public:
    int discardl;    ///< number of points to be discarded from the left
    int discardr;    ///< number of points to be discarded from the right
    int numPts;      ///< number of points on the log(CR) vs log(R) plot
    int numEle;      ///< number of points to be considered from the chaotic map
    std::string mapName;  ///< map to be used
    int dimension;        ///< dimension of the map
    std::string dump;     ///< file name where to dump the log(CR) vs log(R) plot values
    std::string distHist; ///< file name where to dump the distance matrix histogram
    int numBins;          ///< number of bins in the histogram
    bool useLowMem;       ///< whether to use CorrDimLowMem or not
    ChaoticMap* map;      ///< pointer to the map to be used
    REAL* array;          ///< pointer to the element array from the map
    std::vector<std::string> list;   ///< list of all maps currently supported

    /**
     * @brief Set the default values for all the parameters in this class
     */
    CmdLine();
    /** Destructor. Currently does nothing */
    ~CmdLine() {}

    /**
     * @brief Prints the global help and exits
     */
    void showHelp();

    /**
     * @brief Prints the parameter values
     */
    void printParams();

    /**
     * @brief Function to check whether cmdline args are correct
     */
    void validateInputs();


private:
    /**
     * @brief Function to check whether a correct map is specified on commandline
     */
    void validateMap();
};


/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */
#endif // __INCLUDED_CMDLINE_H__

