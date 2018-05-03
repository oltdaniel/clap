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
  uint32_t ponep = 0;

  // Cache second parameter type
  uint8_t ptwot = PAR_NAME;

  // Cache second parameter value
  uint64_t ptwov = 0;

  // NOTE: Parameters will be read correctly in each instruction statement

  // Execute instructions
  switch (ins) {
    case INS_NOOP:
      break;

    case INS_HALT:
      /* Stop running */
      m->running = false;
      break;

    case INS_ALLO:
      break;

    case INS_POPE:
      break;

    case INS_PUSH:
      break;

    case INS_JUMZ:
      break;

    case INS_JUMP:
      break;

    case INS_CMPZ:
      break;

    case INS_FLAZ:
      break;

    case INS_MOVZ:
      break;

    case INS_MOVE:
      // Get first parameter type
      ponet = m->memory[m->ip++];

      // Get first parameter pointer
      ponep =  m->ip;

      // Calculate first parameter size
      if(ponet == PAR_REGISTER) m->ip++;
      else m->ip += 4; // Address == 32bit

      // Get second parameter type
      ptwot = m->memory[m->ip++];

      /* Get value behind second parameter */
      // Check for register
      if(ptwot == PAR_REGISTER) {
        // Get value from register, and add offset (register == 8bit)
        ptwov = m->registers[m->memory[m->ip++]];
      
      // Check for address
      } else if(ptwot == PAR_ADDRESS) {
        // Get value from code memory as 64bit
        ptwov = (uint64_t)m->memory[(uint32_t)m->memory[m->ip]];

        // Add offset to instruction pointer (address == 32bit)
        m->ip += 4;

      // Check for value
      } else if(ptwot == PAR_VALUE) {
        // Read value from code memory
        ptwov = (uint64_t)m->memory[m->ip];

        // Add offset to instrction pointer (value == 64bit)
        m->ip += 8;
      }

      /* Execute move */
      // Check for first parameter type register
      if(ponet == PAR_REGISTER) {
        // Move second parameter value to register
        m->registers[(uint8_t)m->memory[ponep]] = ptwov;

      // Check for first parameter type address
      } else if(ponet == PAR_ADDRESS) {
        // Assign value to memory
        m->memory[ponep] = ptwov;
      }

      break;

    case INS_SUBI:
      break;

    case INS_ADDI:
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
