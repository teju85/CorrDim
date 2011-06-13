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



#include "basics.h"


using namespace std;


vector<string> listMaps() {
    FILE* fp = fopen("maps.list", "r");
    if(fp == NULL) {
        fprintf(stderr, "Failed to open the file 'maps.list' for reading the list of maps!\n");
        exit(1);
    }
    char line[256];
    vector<string> all_maps;
    while(!feof(fp)) {
        if(fscanf(fp, "%s", line)) {
            all_maps.push_back(line);
        }
    }
    fclose(fp);
    return all_maps;
}

