
CC      := /usr/bin/g++
CFLAGS  := -Wall -g
ECFLAGS := -Wall
CLIBS   := -lm
CSRC    := $(shell find src -name "*.cpp")
COBJ    := $(patsubst %.cpp,%.cppo,${CSRC})
INCLUDES:= -Isrc
DCFG    := .doxygenConfig

#NVCC      := /usr/local/cuda/bin/nvcc
#NVCCFLAGS := -g

EXE     := corrdim

# clean up
TEMP    := $(shell find . -name "*~" -o -name ".*~")
MAPS    := maps.list
MAPS_INC:= src/allMaps.h


default: genMaps doc ${EXE}


genMaps:
	./generateMapList.sh running_from_Makefile


doc:
	doxygen ${DCFG}


${EXE}: ${COBJ}
	${CC} ${ECFLAGS} -o ${EXE} ${COBJ}


## TODO: CUDA files
#%.cuo: %.cu
#	${NVCC} ${NVCCFLAGS} ${CULIBS} -o $@ $<


# C++ files
%.cppo: %.cpp
	${CC} -c ${CFLAGS} ${CLIBS} ${INCLUDES} -o $@ $<


clean:
	rm -f ${EXE}
	rm -f ${COBJ}
	rm -f ${TEMP} ${MAPS} ${MAPS_INC}
