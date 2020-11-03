#ifndef _RDX_H
#define _RDX_H

#include "hash.h"

void read_from_file(char *);
void read_from_dir(char *, HashBucket **);
int count_datafiles(char*);

#endif
