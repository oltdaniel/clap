#include "machine.h"

struct machine_s* machine_new() {
  // Allocate struct
  struct machine_s* m = hmalloc(sizeof(struct machine_s));

  // Allocate registers
  m->registers = hmalloc(sizeof(uint64_t) * 20);

  // Allocate memory
  m->memory = hmalloc(1024);

  // Set status
  m->running = 0;

  // Return allocated machine
  return m;
}

int machine_load(struct machine_s* m, char* file) {
  // Open file
  FILE* codep = fopen(file, "r");

  // Check if file is opened
  if(codep == NULL) {
    // Print error message
    printf("Could not open file %s\n", file);

    // Exit with error code
    exit(EX_FAL);
  }

  // Get to end of the file
  fseek(codep, 0, SEEK_END);

  // Read position (filesize)
  long int sz = ftell(codep);

  // Reset file position
  fseek(codep, 0, SEEK_SET);

  // Allocate buffer + null terminator
  char* buffer = hmalloc(sz + 1);

  // Read file content
  fread(buffer, sz, 1, codep);

  // Compile the assembly code and store in machine memeory
  int status = compiler_run(m->memory, buffer);

  // Return status code
  return status;
}

void machine_run(struct machine_s* m) {
  // Update status
  m->running = 1;

  // Execute code

  // Update status
  m->running = 0;
}
