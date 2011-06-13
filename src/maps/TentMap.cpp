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



#include "TentMap.h"


REAL* TentMap::generate(REAL mu, REAL x0, int k) {
    REAL* arr = new REAL[k];
    for(int i=0;i<k;i++) {
        x0 = (x0 >= 0.5)?  mu * (1 - x0)  :  mu * x0;
        arr[i] = x0;
    }
    return arr;
}

void TentMap::showHelp() {
    fprintf(stdout, "OPTIONS FOR TENT MAP:\n");
    fprintf(stdout, "      [-help, -mu <mu>, -x0 <x0>]\n");
    fprintf(stdout, "  -help         Print this help and exit.\n");
    fprintf(stdout, "  -mu <mu>      Value of mu for the map. [%f]\n", TENT_MU);
    fprintf(stdout, "  -x0 <x0>      Initial value for the map. [%f]\n", TENT_X0);
    exit(0);
}

REAL* TentMap::generateVectors(int numEle, int pos, int argc, char** argv) {
    REAL mu = TENT_MU;
    REAL x0 = TENT_X0;
    Timer tim;
    for(;pos<argc;pos++) {
        if(!strcmp("-help", argv[pos])) {
            this->showHelp();
        }
        else if(!strcmp("-mu", argv[pos])) {
            OPTION_CHECK("-mu", pos, argc);
            GET_NUMBER(mu, "-mu", argv[pos]);
        }
        else if(!strcmp("-x0", argv[pos])) {
            OPTION_CHECK("-x0", pos, argc);
            GET_NUMBER(x0, "-x0", argv[pos]);
        }
        else {
            fprintf(stderr, "Unknown option passed '%s'!\n", argv[pos]);
            exit(1);
        }
    }
    CHECK_RANGE(x0, "-x0", 0.0, 1.0);
    CHECK_RANGE(mu, "-mu", 0.0, 2.0);
    fprintf(stdout, "PARAMETERS: numEle=%d mu=%f x0=%f\n", numEle, mu, x0);
    fprintf(stdout, "Generating numbers from TentMap... ");
    tim.start();
    REAL* arr = this->generate(mu, x0, numEle);
    tim.stopAndPrintTime("Time taken: %f s\n");
    return arr;
}

