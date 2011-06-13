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



#include "HenonMap.h"


REAL* HenonMap::generate(REAL a, REAL b, REAL x0, REAL y0, int k) {
    REAL* arr = new REAL[k<<1];
    int j = 0;
    for(int i=0;i<k;i++,j+=2) {
        arr[j] = y0 + 1 - (a * x0 * x0);
        arr[j+1] = b * x0;
        x0 = arr[j];
        y0 = arr[j+1];
    }
    return arr;
}

void HenonMap::showHelp() {
    fprintf(stdout, "OPTIONS FOR Henon MAP:\n");
    fprintf(stdout, "      [-help, -a <a>, -b <b>, -x0 <x0>, -y0 <y0>]\n");
    fprintf(stdout, "  -help         Print this help and exit.\n");
    fprintf(stdout, "  -a <a>        Value of 'a' for the map. [%f]\n",
            HENON_A);
    fprintf(stdout, "  -b <b>        Value of 'b' for the map. [%f]\n",
            HENON_B);
    fprintf(stdout, "  -x0 <x0>      Initial x-value for the map. [%f]\n",
            HENON_X0);
    fprintf(stdout, "  -y0 <y0>      Initial y-value for the map. [%f]\n",
            HENON_X0);
    exit(0);
}

REAL* HenonMap::generateVectors(int numEle, int pos, int argc, char** argv) {
    REAL a = HENON_A;
    REAL b = HENON_B;
    REAL x0 = HENON_X0;
    REAL y0 = HENON_Y0;
    Timer tim;
    for(;pos<argc;pos++) {
        if(!strcmp("-help", argv[pos])) {
            this->showHelp();
        }
        else if(!strcmp("-a", argv[pos])) {
            OPTION_CHECK("a", pos, argc);
            GET_NUMBER(a, "-a", argv[pos]);
        }
        else if(!strcmp("-b", argv[pos])) {
            OPTION_CHECK("b", pos, argc);
            GET_NUMBER(b, "-b", argv[pos]);
        }
        else if(!strcmp("-x0", argv[pos])) {
            OPTION_CHECK("-x0", pos, argc);
            GET_NUMBER(x0, "-x0", argv[pos]);
        }
        else if(!strcmp("-y0", argv[pos])) {
            OPTION_CHECK("-y0", pos, argc);
            GET_NUMBER(y0, "-y0", argv[pos]);
        }
        else {
            fprintf(stderr, "Unknown option passed '%s'!\n", argv[pos]);
            exit(1);
        }
    }
    CHECK_RANGE(b, "-b", 0.0, 1.0);
    fprintf(stdout, "PARAMETERS: numEle=%d a=%f b=%f x0=%f y0=%f\n",
            numEle, a, b, x0, y0);
    fprintf(stdout, "Generating numbers from HenonMap... ");
    tim.start();
    REAL* arr = this->generate(a, b, x0, y0, numEle);
    tim.stopAndPrintTime("Time taken: %f s\n");
    return arr;
}

