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

void machine_step(struct machine_s* m) {
  // Get instruction
  uint8_t ins = m->memory[m->ip++];

  // Cache first parameter type
  uint8_t ponet = PAR_NAME;

  // Cache first parameter position
  uint64_t ponev = 0;

  // Cache second parameter type
  uint8_t ptwot = PAR_NAME;

  // Cache second parameter value
  uint64_t ptwov = 0;

  /* Parse first parameter if needed */
  switch (ins) {
    case INS_ALLO:
    case INS_POPE:
    case INS_PUSH:
    case INS_JUMZ:
    case INS_JUMP:
    case INS_CMPZ:
    case INS_MOVZ:
    case INS_MOVE:
    case INS_SUBI:
    case INS_ADDI:
      // Get first parameter type
      ponet = m->memory[m->ip++];

      /* Get value behind first parameter */
      // Check for register
      if(ponet == PAR_REGISTER) {
        // Get register number, and add offset to instruction pointer
        // (register == 8bit)
        ponev = (uint8_t)m->memory[m->ip++];

      // Check for address
      } else if(ponet == PAR_ADDRESS) {
        // Get address
        ponev = (uint32_t)m->memory[m->ip];

        // Add offset to instruction pointer (address == 32bit)
        m->ip += 4;

      // Check for value
      } else if(ponet == PAR_VALUE) {
        // Get value
        ponev = (uint64_t)m->memory[m->ip];

        // Add offset to instruction pointer (value == 64bit)
        m->ip += 8;
      }
      break;
  }

  /* Parse second parameter if needed */
  switch(ins) {
    case INS_ALLO:
    case INS_MOVE:
    case INS_SUBI:
    case INS_ADDI:
      // Get second parameter type
      ptwot = m->memory[m->ip++];

      /* Get value behind second parameter */
      // Check for register
      if(ptwot == PAR_REGISTER) {
        // Get value from register, and add offset (register == 8bit)
        ptwov = (uint8_t)m->memory[m->ip++];
      
      // Check for address
      } else if(ptwot == PAR_ADDRESS) {
        // Get value from code memory as 64bit
        ptwov = (uint32_t)m->memory[m->ip];

        // Add offset to instruction pointer (address == 32bit)
        m->ip += 4;

      // Check for value
      } else if(ptwot == PAR_VALUE) {
        // Read value from code memory
        ptwov = (uint64_t)m->memory[m->ip];

        // Add offset to instrction pointer (value == 64bit)
        m->ip += 8;
      }
  }

  /* Execute instructions */
  // Check for instruction
  switch(ins) {
    case INS_HALT:
      // Stop machine
      m->running = false;
      break;
  }

  // Log values of registers
  for(uint8_t r = 0; r < 20; r++) {
    printf("Register #%d = %lu\n", r, m->registers[r]);
  }
}

void machine_run(struct machine_s* m) {
  // Update status
  m->running = true;

  // Execute code
  while(m->running) {
    // Execute step
    machine_step(m);
  }
}
