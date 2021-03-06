#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"
#include "helpers.h"
#include "machine.h"

// Instruction ids
enum {
  INS_UNKNOWN,

  INS_NOOP,
  INS_HALT,

  INS_ALLO,
  INS_POPE,
  INS_PUSH,

  INS_JUMZ,
  INS_JUNZ,
  INS_JUMP,

  INS_CMPZ,

  INS_FLAZ,
  INS_MOVZ,

  INS_MOVE,

  INS_SUBI,
  INS_ADDI,
  INS_MULI,
  INS_DIVI,
};

// Parameter type ids
enum {
  PAR_NAME,
  PAR_ADDRESS,
  PAR_REGISTER,
  PAR_VALUE,
  PAR_STRING,
};

// Label type ids
enum {
  LAB_UNKNOWN,
  LAB_ORG,
  LAB_NAM,
  LAB_VAR,
};

// Store an address alias
struct label_s {
  char* name;
  uint32_t address;
};

int compiler_run(char*, char*);
void compiler_parameters(char*, char*, uint32_t*, uint32_t*, uint8_t, struct label_s*, uint16_t, struct label_s*, uint16_t*);
void compiler_store_parameter(char*, uint32_t*, char*, uint8_t, struct label_s*, uint16_t, struct label_s*, uint16_t*);
void compiler_label(char*, char*, uint32_t*, uint32_t*, uint8_t, struct label_s*, uint16_t*, struct label_s*, uint16_t);

#endif
