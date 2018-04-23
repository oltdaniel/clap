#include "compiler.h"

int compiler_run(char* m, char* buffer) {
  // Remember current position in the buffer
  unsigned int current = 0;

  // Rememeber current position in the code memory
  unsigned int ccurrent = 0;

  // Store current character
  char c = 0;

  // Store the last instruction
  char* ins = hmalloc(4);

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
    }

    // TODO: Check for labels

    // Get next instruction
    strncpy(ins, buffer + current, 4);

    // Update current position
    current += 4;

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

    // Add instruction to memory
    m[ccurrent++] = inst;

    // Read parameters
    compiler_parameters(m, buffer, &current, &ccurrent, inst);
  }

  // Return success code
  return EX_OKA;
}

void compiler_parameters(char* m, char* buffer, unsigned int* current, unsigned int* ccurrent, int inst) {
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
  (*current)++;

  // Read parameters
  while(1) {
    // Read next character from buffer
    c = buffer[(*current)++];

    // Check for null terminator
    if(c == 0) break;

    // Check for space
    if(c == ' ') {
      // Update parameter count
      parameter++;

      // Allocate parameter space
      parameter_one = hmalloc(plength);

      // Read from buffer with offset
      strncpy(parameter_one, buffer + (*current - plength - 1), plength);

      // Reset and new parameter round
      plength = 0;
      continue;
    } else if(c == '\n') {
      // Update parameter count
      parameter++;

      // Allocate parameter space
      parameter_two = hmalloc(plength);

      // Read from buffer with offset
      strncpy(parameter_two, buffer + (*current - plength - 1), plength);

      // Reset and break reading
      plength = 0;
      break;
    }

    // Increment parameter length
    plength++;
  }

  // Decremt position
  (*current)--;

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

  // Recognize parameter two type
  if(parameter_two[0] == 'r') parameter_twot = PAR_REGISTER;
  else if(parameter_two[0] == '@') parameter_twot = PAR_ADDRESS;
  else if(parameter_two[0] == '#') parameter_twot = PAR_VALUE;

  // Validate parameter combination
  // TODO: Validate parameter types
  if(0) {
    // Print error message
    printf("Wrong parameter types given.\n");

    // Exit with error code
    exit(EX_FAL);
  }

  // TODO: Store parameter with union type
}
