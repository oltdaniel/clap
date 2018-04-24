#include "compiler.h"

// Debug helper NOTE: Remove later
void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

int compiler_run(char* m, char* buffer) {
  // Remember current position in the buffer
  uint32_t current = 0;

  // Rememeber current position in the code memory
  uint32_t ccurrent = 0;

  // Store current character
  char c = 0;

  // Store the last instruction
  char* ins = hmalloc(4);

  // Store the last label
  char* lab = hmalloc(3);

  // Store the found address aliases
  void* labels = hmalloc(sizeof(struct label_s) * 10);

  // Store how many labels have been stored
  unsigned int labelsc = 0;

  // Store how many labels are allocated
  unsigned int labelss = 10;

  // Loop through the buffer
  while(1) {
    // Read next char of buffer
    c = buffer[current++];

    // Check for null terminator
    if(c == 0) break;

    // Check for comment
    if(c == ';') {
      // Loop to the line end
      while(c != '\n') {
        // Read next char from buffer
        c = buffer[current++];

        // Check for null terminator
        if(c == 0) break;
      }

      // Decrement position
      current--;

      // Jump to next round
      continue;

    // Check for label trigger
    } else if(c == '.') {
      // Copy label from buffer
      strncpy(lab, buffer + current, 3);

      // Check for end of file
      if(lab[2] == 0) break;

      // Remember label type
      int labelt = LAB_UNKNOWN;

      // Identify label type
      if(strcmp(lab, "org") == 0) labelt = LAB_ORG;
      else if(strcmp(lab, "nam") == 0) labelt = LAB_NAM;
      else if(strcmp(lab, "var") == 0) labelt = LAB_VAR;

      // Store the labels
      compiler_label(m, buffer, &current, &ccurrent, labelt, labels, &labelsc, &labelss);

      // Jump to next round
      continue;

    // Check for new line
    } else if(c == '\n') {
      // Ignore new line
      continue;
    }

    // Get next instruction
    strncpy(ins, buffer + current - 1, 4);

    // Update current position
    current += 4;

    // Check for file end
    if(ins[3] == 0) break;

    // Store the instruction type
    int inst = INS_UNKNOWN;

    // Recognize instruction
    if(strcmp(ins, "halt") == 0) inst = INS_HALT;
    else if(strcmp(ins, "noop") == 0) inst = INS_NOOP;
    else if(strcmp(ins, "allo") == 0) inst = INS_ALLO;
    else if(strcmp(ins, "pope") == 0) inst = INS_POPE;
    else if(strcmp(ins, "push") == 0) inst = INS_PUSH;
    else if(strcmp(ins, "jumz") == 0) inst = INS_JUMZ;
    else if(strcmp(ins, "jump") == 0) inst = INS_JUMP;
    else if(strcmp(ins, "cmpz") == 0) inst = INS_CMPZ;
    else if(strcmp(ins, "flaz") == 0) inst = INS_FLAZ;
    else if(strcmp(ins, "movz") == 0) inst = INS_MOVZ;
    else if(strcmp(ins, "move") == 0) inst = INS_MOVE;
    else if(strcmp(ins, "subi") == 0) inst = INS_SUBI;
    else if(strcmp(ins, "addi") == 0) inst = INS_ADDI;

    // Check for unknown instruction
    if(inst == INS_UNKNOWN) {
      // Print error message
      printf("Sorry, the instruction `%s` is not defined.\n", ins);

      // Exit with error code
      exit(EX_FAL);
    }

    // Add instruction to memory
    m[ccurrent++] = inst;

    // Read parameters
    compiler_parameters(m, buffer, &current, &ccurrent, inst);

    // Debug message NOTE: Remove later
    hexDump("memory", m, ccurrent);
  }

  // Return success code
  return EX_OKA;
}

void compiler_parameters(char* m, char* buffer, uint32_t* current, uint32_t* ccurrent, int inst) {
  // Ingore unknown instructions
  if(inst == 0) return;

  // Store current character
  char c = 0;

  // Store parameter count
  unsigned int parameter = 0;

  // Store current parameter length
  unsigned int plength = 0;

  // Store parameter values
  char* parameter_one = NULL;
  char* parameter_two = NULL;

  // Ingore first whitespace
  if(buffer[*current] == ' ') (*current)++;

  // Read parameters
  while(c != '\n') {
    // Read next character from buffer
    c = buffer[(*current)++];

    // Check for null terminator
    if(c == 0) break;

    // Check for space or first&last parameter
    if(c == ' ' || (parameter == 0 && c == '\n')) {
      // Update parameter count
      parameter++;

      // Allocate parameter space
      parameter_one = hmalloc(plength);

      // Read from buffer with offset
      strncpy(parameter_one, buffer + (*current - plength - 1), plength);

      // Reset and new parameter round
      plength = 0;
    } else if(c == '\n') {
      // Update parameter count
      parameter++;

      // Allocate parameter space
      parameter_two = hmalloc(plength);

      // Read from buffer with offset
      strncpy(parameter_two, buffer + (*current - plength - 1), plength);

      // Reset and break reading
      plength = 0;
    } else {
      // Increment parameter length
      plength++;
    }
  }

  // Validate parameter count
  if((inst == INS_HALT && parameter != 0)
     || (inst == INS_NOOP && parameter != 0)
     || (inst == INS_ALLO && parameter != 2)
     || (inst == INS_POPE && parameter != 1)
     || (inst == INS_PUSH && parameter != 1)
     || (inst == INS_JUMZ && parameter != 1)
     || (inst == INS_JUMP && parameter != 1)
     || (inst == INS_CMPZ && parameter != 1)
     || (inst == INS_FLAZ && parameter != 0)
     || (inst == INS_MOVZ && parameter != 1)
     || (inst == INS_MOVE && parameter != 2)
     || (inst == INS_SUBI && parameter != 2)
     || (inst == INS_ADDI && parameter != 2)) {
    // Print error message
    printf("Wrong number of parameters for instruction.\n");

    // Exit with error code
    exit(EX_FAL);
  }

  // Ignore next steps if zero parameters given
  if(parameter == 0) return;

  // Remember parameter types
  int parameter_onet = PAR_NAME;
  int parameter_twot = PAR_NAME;

  // Recognize parameter one type
  if(parameter_one[0] == 'r') parameter_onet = PAR_REGISTER;
  else if(parameter_one[0] == '@') parameter_onet = PAR_ADDRESS;
  else if(parameter_one[0] == '#') parameter_onet = PAR_VALUE;

  // Check if parameter two exists
  if(parameter_two != NULL) {
    // Recognize parameter two type
    if(parameter_two[0] == 'r') parameter_twot = PAR_REGISTER;
    else if(parameter_two[0] == '@') parameter_twot = PAR_ADDRESS;
    else if(parameter_two[0] == '#') parameter_twot = PAR_VALUE;
  }

  // Validate parameter combination
  // (Not simplified yet, because this could be changed over time)
  if(
    // Validate allo instruction parameters
    !(inst == INS_ALLO && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER)
                      && (parameter_twot == PAR_ADDRESS
                           || parameter_twot == PAR_REGISTER
                           || parameter_twot == PAR_VALUE))
    && // Validate pope instruction parameters
    !(inst == INS_POPE && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER))
    && // Validate push instruction parameters
    !(inst == INS_PUSH && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER
                           || parameter_onet == PAR_VALUE))
    && // Validate jumz instruction parameters
    !(inst == INS_JUMZ && (parameter_onet == PAR_NAME
                           || parameter_onet == PAR_ADDRESS))
    && // Validate jump instruction parameters
    !(inst == INS_JUMP && (parameter_onet == PAR_NAME
                           || parameter_onet == PAR_ADDRESS))
    && // Validate cmpz instruction parameters
    !(inst == INS_CMPZ && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER
                           || parameter_onet == PAR_VALUE))
    && // Validate movz instruction parameters
    !(inst == INS_MOVZ && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER))
    && // Validate move instruction parameters
    !(inst == INS_MOVE && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER)
                       && (parameter_twot == PAR_ADDRESS
                           || parameter_twot == PAR_REGISTER
                           || parameter_twot == PAR_VALUE))
    && // Validate subi instruction parameters
    !(inst == INS_SUBI && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER
                           || parameter_onet == PAR_VALUE)
                       && (parameter_twot == PAR_ADDRESS
                           || parameter_twot == PAR_REGISTER
                           || parameter_twot == PAR_VALUE))
    && // Validate addi instruction parameters
    !(inst == INS_ADDI && (parameter_onet == PAR_ADDRESS
                           || parameter_onet == PAR_REGISTER
                           || parameter_onet == PAR_VALUE)
                       && (parameter_twot == PAR_ADDRESS
                           || parameter_twot == PAR_REGISTER
                           || parameter_twot == PAR_VALUE))
  ) {
    // Print error message
    printf("Wrong parameter types given.\n");

    // Exit with error code
    exit(EX_FAL);
  }

  // Store parameter 1 in the memory
  compiler_store_parameter(m, ccurrent, parameter_one, parameter_onet);

  // Check if parameter 2 exists
  if(parameter_two != NULL) {
    // Store parameter 2 in the memory
    compiler_store_parameter(m, ccurrent, parameter_two, parameter_twot);
  }
}

void compiler_store_parameter(char* m, uint32_t* ccurrent, char* parameter, int parametert) {
  // Declare some remember variables
  // Remember pointer to allocated parameter content memory
  void* parameterc = NULL;

  // Remmeber content length of parameter
  unsigned int parametercl = 0;

  // Check for register parameter type
  if(parametert == PAR_REGISTER) {
    // Ignore first character
    parameter++;

    // Allocate space
    uint8_t* c = hmalloc(1);

    // Translate memory to integer
    *c = atoi(parameter);

    // Update content pointer
    parameterc = c;

    // Update parameter length
    parametercl = 1;

  // Check for value parameter type
  } else if(parametert == PAR_VALUE) {
    // Ignore first character
    parameter++;

    // Allocate space
    uint64_t* c = hmalloc(8);

    // Check for hex format
    if(parameter[0] == 'x') {
      // Translate memory to integer
      *c = strtoll(parameter + 1, NULL, 16);
    } else {
      // Translate memory to integer
      *c = atoll(parameter);
    }

    // Update content pointer
    parameterc = c;

    // Update parameter length
    parametercl = 8;

  // Check for address parameter type
  } else if(parametert == PAR_ADDRESS) {
    // Ignore first character
    parameter++;

    // Allocate space
    uint32_t* c = hmalloc(4);

    // Check for hex format
    if(parameter[0] == 'x') {
      // Translate memory to integer
      *c = strtol(parameter + 1, NULL, 16);
    } else {
      // Translate memory to integer
      *c = atol(parameter);
    }

    // Update current pointer
    parameterc = c;

    // Update parameter length
    parametercl = 4;
  }

  // If no content needs to be written NOTE: Remove when every parameter is implemented
  if(parameterc == NULL) return;

  // Store parameter head
  m[(*ccurrent)++] = (uint8_t)parametert;

  // Copy parameter content to the memory
  memcpy(m + *ccurrent, parameterc, parametercl);

  // Update memory position
  *ccurrent += parametercl;
}

void compiler_label(char* m, char* buffer, uint32_t* current, uint32_t* ccurrent, int labt, void* labels, unsigned int* labelsc, unsigned int* labelss) {
  // NOTE: Ignore labels for now

  // Store current char
  char c;

  // Loop to the line end
  while(c != '\n') {
    // Read next char from buffer
    c = buffer[(*current)++];

    // Check for null terminator
    if(c == 0) break;
  }

  // Decrement position
  (*current)--;
}
