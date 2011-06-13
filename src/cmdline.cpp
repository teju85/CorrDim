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



#include "cmdline.h"
#include "allMaps.h"


using namespace std;


void validateMap(vector<string>& list, const string& param) {
    for(vector<string>::const_iterator itr=list.begin();itr!=list.end();itr++) {
        if(*itr == param) {
            return;
        }
    }
    fprintf(stderr, "Bad map name specified '%s'!\n", param.c_str());
    exit(1);
}

void showHelp(const vector<string>& list) {
    fprintf(stdout, "corrdim: Program to evaluate the correlation dimension from the\n");
    fprintf(stdout, "         points on a trajectory of a map.\n");
    fprintf(stdout, "USAGE:\n");
    fprintf(stdout, " corrdim [-h] [-map <map>, -lowmem, -dump <file>, -numpts <pts>,\n");
    fprintf(stdout, "               -numele <ele>, -discardl <pts>, -discardr <pts>,\n");
    fprintf(stdout, "               -dump-dist-hist <file>, -numbins <bins>]\n");
    fprintf(stdout, "          [... options specific for the maps ...]\n");
    fprintf(stdout, "  -h                 Print this help and exit.\n");
    fprintf(stdout, "  -map <map>         The type of chaotic map to use in order to generate the\n");
    fprintf(stdout, "                     vectors. [%s] Possible maps are:\n", DEFAULT_MAP);
    for(vector<string>::const_iterator itr=list.begin();itr!=list.end();itr++) {
        fprintf(stdout, "                        . %s\n", itr->c_str());
    }
    fprintf(stdout, "  -lowmem            Use the low-memory version of the 'CorrDim' class.\n");
    fprintf(stdout, "  -dump <file>       Dump 'log_r' and 'log_cr' arrays into <file>. [\"\"]\n");
    fprintf(stdout, "  -numpts <pts>      Number of 'R' for which correlation sum\n");
    fprintf(stdout, "                     needs to be evaluated. [%d]\n", NUM_POINTS);
    fprintf(stdout, "  -numele <ele>      Number of elements in the trajectory. [%d]\n", NUM_ELEMENTS);
    fprintf(stdout, "  -discardl <pts>    Number of points on left side to be discarded before\n");
    fprintf(stdout, "                     doing the best-fit. [%d]\n", NUM_DISCARD_L);
    fprintf(stdout, "  -discardr <pts>    Number of points on right side to be discarded before\n");
    fprintf(stdout, "                     doing the best-fit. [%d]\n", NUM_DISCARD_R);
    fprintf(stdout, "  -dump-dist-hist <file>   Dump the histogram of the distance matrix into\n");
    fprintf(stdout, "                           the <file>\n");
    fprintf(stdout, "  -numbins <bins>    Number of bins in the above histogram. [%d]\n", NUM_BINS);
    fprintf(stdout, "NOTE:\n");
    fprintf(stdout, " . If you want to know the options specific for the maps, pass a '-help'\n");
    fprintf(stdout, "   option to that map. For eg, if you want to know the options supported by\n");
    fprintf(stdout, "   LogisticMap,   ./corrdim -map LogisticMap -help\n");
    fprintf(stdout, " . All options specific to the maps MUST be at the end of command line!\n");
    exit(0);
}



CmdLine::CmdLine() {
    discardl = NUM_DISCARD_L;
    discardr = NUM_DISCARD_R;
    numPts = NUM_POINTS;
    numEle = NUM_ELEMENTS;
    mapName = DEFAULT_MAP;
    dimension = 0;
    dump = "";
    distHist = "";
    numBins = NUM_BINS;
    useLowMem = false;
    map = NULL;
    array = NULL;
    list = listMaps();
}

void CmdLine::validateMap() {
    for(vector<string>::const_iterator itr=list.begin();itr!=list.end();itr++) {
        if(*itr == mapName) {
            return;
        }
    }
    fprintf(stderr, "Bad map name specified '%s'!\n", mapName.c_str());
    exit(1);
}

void CmdLine::showHelp() {
    fprintf(stdout, "corrdim: Program to evaluate the correlation dimension from the\n");
    fprintf(stdout, "         points on a trajectory of a map.\n");
    fprintf(stdout, "USAGE:\n");
    fprintf(stdout, " corrdim [-h] [-map <map>, -lowmem, -dump <file>, -numpts <pts>,\n");
    fprintf(stdout, "               -numele <ele>, -discardl <pts>, -discardr <pts>,\n");
    fprintf(stdout, "               -dump-dist-hist <file>, -numbins <bins>]\n");
    fprintf(stdout, "          [... options specific for the maps ...]\n");
    fprintf(stdout, "  -h                 Print this help and exit.\n");
    fprintf(stdout, "  -map <map>         The type of chaotic map to use in order to generate the\n");
    fprintf(stdout, "                     vectors. [%s] Possible maps are:\n", DEFAULT_MAP);
    for(vector<string>::const_iterator itr=list.begin();itr!=list.end();itr++) {
        fprintf(stdout, "                        . %s\n", itr->c_str());
    }
    fprintf(stdout, "  -lowmem            Use the low-memory version of the 'CorrDim' class.\n");
    fprintf(stdout, "  -dump <file>       Dump 'log_r' and 'log_cr' arrays into <file>. [\"\"]\n");
    fprintf(stdout, "  -numpts <pts>      Number of 'R' for which correlation sum\n");
    fprintf(stdout, "                     needs to be evaluated. [%d]\n", NUM_POINTS);
    fprintf(stdout, "  -numele <ele>      Number of elements in the trajectory. [%d]\n", NUM_ELEMENTS);
    fprintf(stdout, "  -discardl <pts>    Number of points on left side to be discarded before\n");
    fprintf(stdout, "                     doing the best-fit. [%d]\n", NUM_DISCARD_L);
    fprintf(stdout, "  -discardr <pts>    Number of points on right side to be discarded before\n");
    fprintf(stdout, "                     doing the best-fit. [%d]\n", NUM_DISCARD_R);
    fprintf(stdout, "  -dump-dist-hist <file>   Dump the histogram of the distance matrix into\n");
    fprintf(stdout, "                           the <file>\n");
    fprintf(stdout, "  -numbins <bins>    Number of bins in the above histogram. [%d]\n", NUM_BINS);
    fprintf(stdout, "NOTE:\n");
    fprintf(stdout, " . If you want to know the options specific for the maps, pass a '-help'\n");
    fprintf(stdout, "   option to that map. For eg, if you want to know the options supported by\n");
    fprintf(stdout, "   LogisticMap,   ./corrdim -map LogisticMap -help\n");
    fprintf(stdout, " . All options specific to the maps MUST be at the end of command line!\n");
    exit(0);
}

void CmdLine::printParams() {
    fprintf(stdout, "PARAMETERS: numPts=%d discardl=%d discardr=%d\n", numPts, discardl, discardr);
    fprintf(stdout, "PARAMETERS: dump=%s map=%s\n", dump.c_str(), mapName.c_str());
}

void CmdLine::validateInputs() {
    CHECK_POSITIVE(numPts, "-numpts");
    CHECK_POSITIVE(numBins, "-numbins");
    CHECK_POSITIVE(numEle, "-numele");
    CHECK_POSITIVE(discardl, "-discardl");
    CHECK_POSITIVE(discardr, "-discardr");
    validateMap();
    if(discardl >= (numPts>>1)) {
        fprintf(stderr, "Argument to '-discardl' should be less than half the arg to '-numpts'!\n");
        exit(1);
    }
    if(discardr >= (numPts>>1)) {
        fprintf(stderr, "Argument to '-discardr' should be less than half the arg to '-numpts'!\n");
        exit(1);
    }
    map = getMap(mapName);
}

