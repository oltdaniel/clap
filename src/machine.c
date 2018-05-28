#include "machine.h"

struct machine_s* machine_new() {
  // Allocate struct
  struct machine_s* m = hmalloc(sizeof(struct machine_s));

  // Allocate registers
  m->registers = hmalloc(sizeof(uint64_t) * 20);

  // Allocate memory
  m->memory = hmalloc(MEMORY_SIZE);

  // Set stack pointer
  m->sp = MEMORY_SIZE;

  // Set status
  m->running = 0;

  // Return allocated machine
  return m;
}

uint32_t machine_load(struct machine_s* m, char* file) {
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
  uint16_t sz = (uint16_t)ftell(codep);

  // Reset file position
  fseek(codep, 0, SEEK_SET);

  // Allocate buffer + null terminator
  char* buffer = hmalloc(sz + 1);

  // Read file content
  fread(buffer, sz, 1, codep);

  // Compile the assembly code and store in machine memeory
  uint32_t ccurrent = compiler_run(m->memory, buffer);

  // Update allocation pointer
  m->ap = ccurrent;

  // Return status code
  return EX_OKA;
}

void machine_step(struct machine_s* m) {
  // Get instruction
  uint8_t ins = m->memory[m->ip++];

  // Ignore step if instruction is unknown
  if(ins == INS_UNKNOWN) return;

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
    case INS_MULI:
    case INS_DIVI:
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
    case INS_MULI:
    case INS_DIVI:
      // Get second parameter type
      ptwot = m->memory[m->ip++];

      /* Get value behind second parameter */
      // Check for register
      if(ptwot == PAR_REGISTER) {
        // Get value from register, and add offset (register == 8bit)
        ptwov = (uint8_t)m->memory[m->ip++];

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
    case INS_MULI:
    case INS_DIVI:
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

    case INS_POPE:
      // Copy element
      memcpy(&ptwov, m->memory + m->sp, 8);

      // Increment stack pointer
      m->sp += 8;

      // Put to target depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Set register value
        m->registers[ponev] = ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Copy result to memory
        memcpy(m->memory + ponev, &ptwov, 8);
      }
      break;

    case INS_PUSH:
      // Push value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Copy register value to stack
        memcpy(m->memory + m->sp - 8, &m->registers[ponev], 8);

        // Decrement stack pointer
        m->sp -= 8;
      } else if(ponet == PAR_ADDRESS) {
        // Copy value from memory to stack
        memcpy(m->memory + m->sp - 8, m->memory + ponev, 8);

        // Decrement stack pointer
        m->sp -= 8;
      } else if(ponet == PAR_VALUE) {
        // Copy value to stack
        memcpy(m->memory + m->sp - 8, &ponev, 8);

        // Decrement stack pointer
        m->sp -= 8;
      }
      break;

    case INS_JUMZ:
      // Check if flag is set
      if(((m->flags >> FLAG_ZERO) & 1U) == 1) {
        // Update instruction pointer
        m->ip = ponev;
      }
      break;

    case INS_JUNZ:
      // CHeck if flag is set
      if(((m->flags >> FLAG_ZERO) & 1U) == 0) {
        // Update instruction pointer
        m->ip = ponev;
      }
      break;

    case INS_JUMP:
      // Change instruction pointer position to given position
      m->ip = ponev;
      break;

    case INS_MOVZ:
      // Set value behind first parameter to zero
      if(ponet == PAR_REGISTER) {
        // Set memory to 0
        memset(&m->registers[ponev], 0, 8);

      }
      break;

    case INS_CMPZ:
      // Chekc if first parametrer value is zero
      if(ponev == 0) {
        // Set zero flag
        m->flags |= 1UL << FLAG_ZERO;
      }
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
        uint32_t target = ponev;

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
        uint32_t target = ponev;

        // Get value from first parameter
        memcpy(&ponev, m->memory + ponev, 8);

        // Add both numbers
        ponev += ptwov;

        // Copy result to memory
        memcpy(m->memory + target, &ponev, 8);
      }
      break;

    case INS_MULI:
      // Muliply value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Multply with register
        m->registers[ponev] *= ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Store target
        uint32_t target = ponev;

        // Get value from first parameter
        memcpy(&ponev, m->memory + ponev, 8);

        // Multiply both numbers
        ponev *= ptwov;

        // Copy result to memory
        memcpy(m->memory + target, &ponev, 8);
      }
      break;

    case INS_DIVI:
      // Divide value depending on first parameter
      if(ponet == PAR_REGISTER) {
        // Divide with register
        m->registers[ponev] /= ptwov;

      } else if(ponet == PAR_ADDRESS) {
        // Store target
        uint32_t target = ponev;

        // Get value from first parameter
        memcpy(&ponev, m->memory + ponev, 8);

        // Divide both numbers
        ponev /= ptwov;

        // Copy result to memory
        memcpy(m->memory + target, &ponev, 8);
      }
      break;
  }

  // Log values of registers
  for(uint8_t r = 0; r < 20; r++) {
    printf("Register #%02d = %lu\n", r, m->registers[r]);
  }

  // Dump memory
  hex_dump("memory", m->memory, MEMORY_SIZE);

  // Print flags
  printf("Overflow Flag: %d\n", (m->flags >> FLAG_OVERFLOW) & 1U);
  printf("Parity Flag: %d\n",   (m->flags >> FLAG_PARITY)   & 1U);
  printf("Zero Flag: %d\n",     (m->flags >> FLAG_ZERO)     & 1U);
  printf("Negative Flag: %d\n", (m->flags >> FLAG_NEGATIVE) & 1U);
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
