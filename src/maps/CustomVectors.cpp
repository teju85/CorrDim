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



#include "CustomVectors.h"




void CustomVectors::showHelp() {
    fprintf(stdout, "OPTIONS FOR CUSTOM MAP:\n");
    fprintf(stdout, "   [-help, -file <file>]\n");
    fprintf(stdout, "  -help           Print this help and exit\n");
    fprintf(stdout, "  -dim <dim>      Dimension of the vectors. This is a mandatory option!\n");
    fprintf(stdout, "  -file <file>    File from which to read the vectors. This is a\n");
    fprintf(stdout, "                  mandatory option!\n");
    exit(0);
}


REAL* CustomVectors::generateVectors(int numEle, int pos, int argc, char** argv) {
    std::string file = "";
    Timer tim;
    m_dim = -1;
    for(;pos<argc;pos++) {
        if(!strcmp("-help", argv[pos])) {
            this->showHelp();
        }
        else if(!strcmp("-dim", argv[pos])) {
            OPTION_CHECK("-dim", pos, argc);
            GET_INTEGER(m_dim, "-dim", argv[pos]);
            CHECK_POSITIVE(m_dim, "-dim");
        }
        else if(!strcmp("-file", argv[pos])) {
            OPTION_CHECK("-file", pos, argc);
            file = argv[pos];
        }
        else {
            fprintf(stderr, "Unknown option passed '%s'!\n", argv[pos]);
            exit(1);
        }
    }
    if(file == "") {
        fprintf(stderr, "'-file' is a mandatory option for 'custom-map'!\n");
        exit(1);
    }
    if(m_dim == -1) {
        fprintf(stderr, "'-dim' is a mandatory option for 'custom-map'!\n");
        exit(1);
    }
    fprintf(stdout, "PARAMETERS: dim=%d numEle=%d file=%s\n", m_dim, numEle, file.c_str());
    fprintf(stdout, "Generating numbers from file=%s... ", file.c_str());
    tim.start();
    int num = numEle * m_dim;
    FILE* fp = fopen(file.c_str(), "r");
    if(fp == NULL) {
        fprintf(stderr, "Failed to open the file '%s' for reading the vectors!\n", file.c_str());
        exit(1);
    }
    REAL* arr = new REAL[num];
    for(int i=0;i<num;i++) {
        fscanf(fp, "%lf", &(arr[i]));
    }
    fclose(fp);
    tim.stopAndPrintTime("Time taken: %f s\n");
    return arr;
}
