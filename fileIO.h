#ifndef _FILEIO_H
#define _FILEIO_H

#include "headers.h"

void loadFullFile(char *fileName);
int loadMMapForFile(unsigned char[]);
void loadFileToMMap(unsigned char[]);
extern unsigned char *mapToFile;
extern unsigned char *fileMap;

#endif
