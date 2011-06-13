#!/bin/bash
#
# Generates the list of maps currently supported
#

if [ "$1" != "running_from_Makefile" ]; then
    echo "You cannot run this script from outside 'Makefile'!"
    exit 1
fi

# maps list
outFile="maps.list"
find src/maps -name "*.h" | sed -e 's/src\/maps\///' | sed -e 's/\.h//' | grep -v ChaoticMap > $outFile

# maps list to be included in run.cpp
includeFile="src/allMaps.h"
function includeMaps() {
    echo "/***************************************************************************\\"
    echo "Tool to find the correlation dimension of a sequence."
    echo "Copyright (C) 2010 Tejaswi."
    echo ""
    echo "This program is free software: you can redistribute it and/or modify"
    echo "it under the terms of the GNU General Public License as published by"
    echo "the Free Software Foundation, either version 3 of the License, or"
    echo "(at your option) any later version."
    echo ""
    echo "This program is distributed in the hope that it will be useful,"
    echo "but WITHOUT ANY WARRANTY; without even the implied warranty of"
    echo "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the"
    echo "GNU General Public License for more details."
    echo ""
    echo "You should have received a copy of the GNU General Public License"
    echo "along with this program. If not, see <http://www.gnu.org/licenses/>."
    echo "\\***************************************************************************/"
    echo ""
    echo ""
    echo ""
    echo "#ifndef __INCLUDED_ALLMAPS_H__"
    echo "#define __INCLUDED_ALLMAPS_H__"
    echo ""
    echo "#ifndef __cplusplus"
    echo "#error A C++ compiler is required to compile this code!"
    echo "#endif"
    echo ""
    echo ""
    echo "// THIS IS AN AUTOMATICALLY GENERATED FILE!"
    echo ""
    echo ""
    files=`find src/maps -name "*.h" | sed -e 's/src\///' | grep -v ChaoticMap`
    for file in $files; do
	echo "#include \"$file\""
    done
    echo "#include <string>"
    echo ""
    echo ""
    echo "/**"
    echo " * @brief Gets the appropriate map"
    echo " * @param mapName name of the map for which an object is to be created."
    echo " * @return the pointer to the desired map"
    echo " */"
    echo "ChaoticMap* getMap(const std::string& mapName) {"
    for name in `cat $outFile`; do
	echo "    if(mapName == \"$name\")   return new $name();"
    done
    echo "    return NULL;"
    echo "}"
    echo ""
    echo ""
    echo "/* DO NOT WRITE ANYTHING BELOW THIS LINE!!! */"
    echo "#endif // __INCLUDED_BASICS_H__"
}
includeMaps > $includeFile

exit 0
