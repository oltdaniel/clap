#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "info.h"

// Set bool as alias for int
#define bool char

// Define bool values
#define true 1
#define false 0

void* hmalloc(size_t);
void hex_dump(char*, void*, uint16_t);

#endif
