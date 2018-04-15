#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"
#include "helpers.h"
#include "machine.h"

// Instructíon ids
enum {
  INS_UNKNOWN,

  INS_NOOP,
  INS_HALT,

  INS_JUMZ,
  INS_JUMP,

  INS_CMPZ,

  INS_FLAZ,
  INS_MOVZ,

  INS_MOVE,

  INS_SUBI,
  INS_ADDI,
};

// Parameter type ids
enum {
  PAR_NAME,
  PAR_ADDRESS,
  PAR_REGISTER,
  PAR_VALUE,
};

int compiler_run(char*, char*);
void compiler_parameters(char*, char*, unsigned int*, unsigned int*, int);

#endif
