#ifndef MACHINE_H
#define MACHINE_H

#include <stdlib.h>

#include "info.h"
#include "helpers.h"
#include "compiler.h"

#define MEMORY_SIZE 1024

// The machine struct
struct machine_s {
  // 20 registers, 64bit each
  uint64_t* registers;

  // Memory
  char* memory;

  // Pointers
  uint32_t ip;
  uint32_t sp;
  uint32_t ap;

  // Flags
  char flags;

  // Status
  bool running;
};

// The index numbers of the flags
enum {
  FLAG_OVERFLOW,
  FLAG_PARITY,
  FLAG_ZERO,
  FLAG_NEGATIVE,
};

struct machine_s* machine_new();
uint32_t machine_load(struct machine_s*, char*);
void machine_step(struct machine_s*);
void machine_run(struct machine_s*);

#endif
