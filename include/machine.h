#ifndef MACHINE_H
#define MACHINE_H

#include <stdlib.h>
#include <stdint.h>

#include "info.h"
#include "helpers.h"
#include "compiler.h"

// The machine struct
struct machine_s {
  // 20 registers, 64bit each
  uint64_t* registers;

  // Memory
  char* memory;

  // Pointers
  char ip;
  char sp;
  char ap;

  // Flags
  char flags;

  // Status
  int running;
};

// The index numbers of the flags
enum {
  FLAG_OVERFLOW,
  FLAG_PARITY,
  FLAG_ZERO,
  FLAG_NEGATIVE,
};

struct machine_s* machine_new();
int machine_load(struct machine_s*, char*);
void machine_run(struct machine_s*);

#endif
