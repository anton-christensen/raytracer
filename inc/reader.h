#ifndef _READER_
#define _READER_

#include <stdio.h>
#include <string.h>
#include "llist.h"
#include "vector.h"
#include "triangle.h"
#include "common.h"

void parse_ply(char* filename, llist* geometry);

#endif