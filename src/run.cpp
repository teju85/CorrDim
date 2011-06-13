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
#include "CorrDim.h"
#include "CorrDimLowMem.h"


using namespace std;



REAL runCorrDim(const CmdLine& cmd, Timer& tim, REAL* log_cr, REAL* log_r,
		REAL* inter, int* hist, REAL* bins, unsigned long int& totalMem) {
    fprintf(stdout, "Initializing 'CorrDim'... ");
    tim.start();
    CorrDim cd = CorrDim(cmd.array, cmd.numEle, cmd.dimension);
    tim.stopAndPrintTime("Time taken: %f s\n");

    fprintf(stdout, "Evaluating corr-dim... ");
    tim.start();
    REAL corrdim = cd.evalCorrDim(cmd.numPts, cmd.discardl, cmd.discardr, log_cr, log_r, inter);
    cd.getDistMatrixHistogram(cmd.numBins, hist, bins);
    tim.stopAndPrintTime("Time taken: %f s\n");

    totalMem = ((cmd.numEle * cmd.dimension * sizeof(REAL)) +       // signal
                (cmd.numEle * (cmd.numEle-1) / 2 * sizeof(REAL)) +  // distance matrix
                (cmd.numBins * sizeof(int)) +                       // histogram-bins
                (cmd.numBins * sizeof(REAL)) +                      // histogram-bin-values
                (3 * cmd.numPts * sizeof(REAL)));                   // log_r, log_cr, inter arrays

    return corrdim;
}


REAL runCorrDimLowMem(const CmdLine& cmd, Timer& tim, REAL* log_cr, REAL* log_r,
		      REAL* inter, int* hist, REAL* bins, unsigned long int& totalMem) {
    fprintf(stdout, "Initializing 'CorrDimLowMem'... ");
    tim.start();
    CorrDimLowMem cd = CorrDimLowMem(cmd.array, cmd.numEle, cmd.dimension);
    tim.stopAndPrintTime("Time taken: %f s\n");

    fprintf(stdout, "Evaluating corr-dim... ");
    tim.start();
    REAL corrdim = cd.evalCorrDim(cmd.numPts, cmd.discardl, cmd.discardr, log_cr, log_r, inter);
    cd.getDistMatrixHistogram(cmd.numBins, hist, bins);
    tim.stopAndPrintTime("Time taken: %f s\n");

    totalMem = ((cmd.numEle * cmd.dimension * sizeof(REAL)) +       // signal
                (cmd.numBins * sizeof(int)) +                       // histogram-bins
                (cmd.numBins * sizeof(REAL)) +                      // histogram-bin-values
                (3 * cmd.numPts * sizeof(REAL)));                   // log_r, log_cr, inter arrays

    return corrdim;
}

void printMemory(unsigned long int totalMem) {
    if(!(totalMem >> 10)) {
        fprintf(stdout, "Maximum memory usage (in B): ~%lu\n", totalMem);
    }
    else if(!(totalMem >> 20)) {
        fprintf(stdout, "Maximum memory usage (in kB): ~%lu\n", totalMem>>10);
    }
    else {
        fprintf(stdout, "Maximum memory usage (in MB): ~%lu\n", totalMem>>20);
    }
}


void run(const CmdLine& cmd) {
    Timer tim;
    REAL *log_cr, *log_r, *inter, *bins;
    int *hist;
    REAL corrdim;
    unsigned long int totalMem;

    log_cr = new REAL[cmd.numPts];
    log_r = new REAL[cmd.numPts];
    inter = new REAL[cmd.numPts];
    hist = new int[cmd.numBins];
    bins = new REAL[cmd.numBins];
    corrdim = (cmd.useLowMem)? runCorrDimLowMem(cmd, tim, log_cr, log_r, inter, hist, bins, totalMem) :
                               runCorrDim(cmd, tim, log_cr, log_r, inter, hist, bins, totalMem);

    if(cmd.dump != "") {
        fprintf(stdout, "Dumping 'log_cr', 'log_r' and 'inter' to '%s'... ", cmd.dump.c_str());
        tim.start();
        FILE* fp = fopen(cmd.dump.c_str(), "w");
        if(fp == NULL) {
            fprintf(stderr, "Failed to open the file '%s' for writing!\n", cmd.dump.c_str());
            exit(1);
        }
        for(int i=0;i<cmd.numPts;i++) {
            fprintf(fp, "%f  %f  %f\n", log_r[i], log_cr[i], inter[i]);
        }
        fclose(fp);
        tim.stopAndPrintTime("Time taken: %f s\n");
    }

    if(cmd.distHist != "") {
        fprintf(stdout, "Dumping distance-matrix histogram to '%s'... ", cmd.distHist.c_str());
        tim.start();
        FILE* fp = fopen(cmd.distHist.c_str(), "w");
        if(fp == NULL) {
            fprintf(stderr, "Failed to open the file '%s' for writing!\n", cmd.distHist.c_str());
            exit(1);
        }
        for(int i=0;i<cmd.numBins;i++) {
            fprintf(fp, "%f  %d\n", bins[i], hist[i]);
        }
        fclose(fp);
        delete hist;
        delete bins;
        tim.stopAndPrintTime("Time taken: %f s\n");
    }

    printMemory(totalMem);
    fprintf(stdout, "... CORRELATION DIMENSION = %f\n", corrdim);

    delete log_cr;
    delete log_r;
    delete inter;
}


int main(int argc, char** argv) {
    Timer tim;
    tim.start();
    int i = 1;
    CmdLine cmd;
    // command line argument parsing
    for(;i<argc;i++) {
        if(!strcmp("-h", argv[i])) {
            cmd.showHelp();
        }
        else if(!strcmp("-dump", argv[i])) {
            OPTION_CHECK("-dump", i, argc);
            cmd.dump = argv[i];
        }
        else if(!strcmp("-lowmem", argv[i])) {
            cmd.useLowMem = true;
        }
        else if(!strcmp("-numpts", argv[i])) {
            OPTION_CHECK("-numpts", i, argc);
            GET_INTEGER(cmd.numPts, "-numpts", argv[i]);
        }
        else if(!strcmp("-numele", argv[i])) {
            OPTION_CHECK("-numele", i, argc);
            GET_INTEGER(cmd.numEle, "-numele", argv[i]);
        }
        else if(!strcmp("-discardl", argv[i])) {
            OPTION_CHECK("-discardl", i, argc);
            GET_INTEGER(cmd.discardl, "-discardl", argv[i]);
        }
        else if(!strcmp("-discardr", argv[i])) {
            OPTION_CHECK("-discardr", i, argc);
            GET_INTEGER(cmd.discardr, "-discardr", argv[i]);
        }
        else if(!strcmp("-map", argv[i])) {
            OPTION_CHECK("-map", i, argc);
            cmd.mapName = argv[i];
        }
        else if(!strcmp("-dump-dist-hist", argv[i])) {
            OPTION_CHECK("-dump-dist-hist", i, argc);
            cmd.distHist = argv[i];
        }
        else if(!strcmp("-numbins", argv[i])) {
            OPTION_CHECK("-numbins", i, argc);
            GET_INTEGER(cmd.numBins, "-numbins", argv[i]);
        }
        // unknown option found!
        // These must be specific to the maps, pass them to the respective maps
        else {
            break;
        }
    }
    cmd.validateInputs();
    cmd.printParams();
    cmd.array = cmd.map->generateVectors(cmd.numEle, i, argc, argv);
    cmd.dimension = cmd.map->getDimension();
    run(cmd);
    tim.stopAndPrintTime("Total time taken: %f s\n");
    return 0;
}
