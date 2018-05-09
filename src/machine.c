#include "machine.h"

struct machine_s* machine_new() {
  // Allocate struct
  struct machine_s* m = hmalloc(sizeof(struct machine_s));

  // Allocate registers
  m->registers = hmalloc(sizeof(unsigned long long int) * 20);

  // Allocate memory
  m->memory = hmalloc(MEMORY_SIZE);

  // Set stack pointer
  m->sp = MEMORY_SIZE;

  // Set status
  m->running = 0;

  // Return allocated machine
  return m;
}

unsigned long int machine_load(struct machine_s* m, char* file) {
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
  unsigned long int ccurrent = compiler_run(m->memory, buffer);

  // Update allocation pointer
  m->ap = ccurrent;

  // Return status code
  return EX_OKA;
}

void machine_step(struct machine_s* m) {
  // Get instruction
  unsigned char ins = m->memory[m->ip++];

  // Ignore step if instruction is unknown
  if(ins == INS_UNKNOWN) return;

  // Cache first parameter type
  unsigned char ponet = PAR_NAME;

  // Cache first parameter position
  unsigned char ponev = 0;

  // Cache second parameter type
  unsigned char ptwot = PAR_NAME;

  // Cache second parameter value
  unsigned long long int ptwov = 0;

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
        ponev = (unsigned char)m->memory[m->ip++];

      // Check for address
      } else if(ponet == PAR_ADDRESS) {
        // Get address
        memcpy(&ponev, m->memory + m->ip, 4);

        // Add offset to instruction pointer (address == 32bit)
        m->ip += 4;

      // Check for value
      } else if(ponet == PAR_VALUE) {
        // Get value
        memcpy(&ponev, m->memory + m->ip, 8);

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
        ptwov = (unsigned char)m->memory[m->ip++];

      // Check for address
      } else if(ptwot == PAR_ADDRESS) {
        // Get value from code memory as 32bit
        memcpy(&ptwov, m->memory + m->ip, 4);

        // Add offset to instruction pointer (address == 32bit)
        m->ip += 4;

      // Check for value
      } else if(ptwot == PAR_VALUE) {
        // Read value from code memory
        memcpy(&ptwov, m->memory + m->ip, 8);

        // Add offset to instrction pointer (value == 64bit)
        m->ip += 8;
      }
  }

  /* Read value behind first parameter if needed */
  switch(ins) {
    case INS_PUSH:
    case INS_CMPZ:
      // Get value depending on parameter type
      if(ponet == PAR_REGISTER) {
        // Get value from register
        ponev = m->registers[ponev];

      } else if(ponet == PAR_ADDRESS) {
        // Get value from memory
        memcpy(&ponev, m->memory + ponev, 8);

      }
      break;
  }

  /* Read value behind first parameter if needed */
  switch(ins) {
    case INS_ALLO:
    case INS_MOVE:
    case INS_SUBI:
    case INS_ADDI:
      // Get value depending on parameter type
      if(ptwot == PAR_REGISTER) {
        // Get value from register
        ptwov = m->registers[ptwov];

      } else if(ptwot == PAR_ADDRESS) {
        // Get value from memory
        memcpy(&ptwov, m->memory + m->ip, 8);

      }
      break;
  }

  /* Execute instructions */
  // Check for instruction
  switch(ins) {
    case INS_HALT:
      // Stop machine
      m->running = false;
      break;

    case INS_JUMP:
      // Change instruction pointer position to given position
      m->ip = (unsigned long int)ponev;
      break;

    case INS_MOVE:
      // Move second parameter value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Move value to register
        m->registers[ponev] = ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Move value to memory
        memcpy(m->memory + ponev, &ptwov, 8);

      }
      break;

    case INS_SUBI:
      // Substract value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Substract value from register
        m->registers[ponev] -= ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Store target
        unsigned long int target = ponev;

        // Get value from first parameter
        memcpy(&ponev, m->memory + ponev, 8);

        // Substract both numbers
        ponev -= ptwov;

        // Copy result to memory
        memcpy(m->memory + target, &ponev, 8);
      }
      break;

    case INS_ADDI:
      // Add value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Add value to register
        m->registers[ponev] += ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Store target
        unsigned long int target = ponev;

        // Get value from first parameter
        memcpy(&ponev, m->memory + ponev, 8);

        // Add both numbers
        ponev += ptwov;

        // Copy result to memory
        memcpy(m->memory + target, &ponev, 8);
      }
      break;
  }

  // Log values of registers
  for(unsigned char r = 0; r < 20; r++) {
    printf("Register #%d = %llu\n", r, m->registers[r]);
  }

  // Dump memory
  hex_dump("memory", m->memory, 128);
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
