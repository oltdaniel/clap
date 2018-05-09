#include "compiler.h"

int compiler_run(char* m, char* buffer) {
  // Remember current position in the buffer
  unsigned long int current = 0;

  // Rememeber current position in the code memory
  unsigned long int ccurrent = 0;

  // Store current character
  char c = 0;

  // Store the last instruction
  char* ins = hmalloc(4);

  // Store the last label
  char* lab = hmalloc(3);

  // Store the found address aliases
  struct label_s* labels = hmalloc(sizeof(struct label_s) * 10);

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

      // Update current position
      current += 3;

      // Check for end of file
      if(lab[2] == 0) break;

      // Remember label type
      int labelt = LAB_UNKNOWN;

      // Identify label type
      if(strcmp(lab, "org") == 0) labelt = LAB_ORG;
      else if(strcmp(lab, "nam") == 0) labelt = LAB_NAM;
      else if(strcmp(lab, "var") == 0) labelt = LAB_VAR;

      // Store the labels
      compiler_label(m, buffer, &current, &ccurrent, labelt, labels, &labelsc);

      // Check for reaming labels list space
      if(labelsc >= labelss) {
        // Increment size of labels list
        labelss += 2;

        // Reallocate space
        labels = realloc(labels, labelss * sizeof(struct label_s));
      }

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
    m[ccurrent++] = (unsigned char)inst;

    // Read parameters
    compiler_parameters(m, buffer, &current, &ccurrent, inst, labels, labelsc);
  }

  // Return code position
  return ccurrent;
}

void compiler_parameters(char* m, char* buffer, unsigned long int* current, unsigned long int* ccurrent, int inst, struct label_s* labels, unsigned int labelsc) {
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

  // Ignore if current char is line end
  if(buffer[(*current)] == '\n') return;

  // Read parameters
  while(1) {
    // Read next character from buffer
    c = buffer[++(*current)];

    // Increment parameter length
    plength++;

    // Check for space or first&last parameter
    if(c == ' ' || c == '\n') {
      // Update parameter count
      parameter++;

      // Check if second parameter reached
      if(parameter == 2) {
        // Ignore whitespace
        plength--;

        // Allocate parameter space
        parameter_two = hmalloc(plength);

        // Read from buffer with offset
        strncpy(parameter_two, buffer + *current - plength, plength);

        break;
      }

      // Allocate parameter space
      parameter_one = hmalloc(plength);

      // Read from buffer with offset
      strncpy(parameter_one, buffer + *current - plength, plength);

      // Reset and new parameter round
      plength = 0;

    }

    // Check for line end and null terminator
    if(c == '\n' || c == 0) break;
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
  compiler_store_parameter(m, ccurrent, parameter_one, parameter_onet, labels, labelsc);

  // Check if parameter 2 exists
  if(parameter_two != NULL) {
    // Store parameter 2 in the memory
    compiler_store_parameter(m, ccurrent, parameter_two, parameter_twot, labels, labelsc);
  }
}

void compiler_store_parameter(char* m, unsigned long int* ccurrent, char* parameter, int parametert, struct label_s* labels, unsigned int labelsc) {
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
    unsigned char* c = hmalloc(1);

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
    unsigned long long int* c = hmalloc(8);

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
    unsigned long int* c = hmalloc(4);

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

  // Check for name parameter type
  } else if(parametert == PAR_NAME) {
    // Store current label
    struct label_s* labelp = NULL;

    // Loop through exisiting labels
    for(unsigned int i = 0; i < labelsc; i++) {
      // Get current label
      labelp = &labels[i];

      // Check if names are equal
      if(strcmp(parameter, labelp->name) == 0) {
        // Allocate space
        unsigned long int* c = hmalloc(4);

        // Set address as content
        *c = labelp->address;

        // Update current pointer
        parameterc = c;

        // Update parameter length
        parametercl = 4;

        // Update parameter type
        parametert = PAR_ADDRESS;

        // End loop
        break;
      }
    }

    // Check if label address is not assigned
    if(parameterc == NULL) {
      // Print error message
      printf("The label `%s` could not be found.\n", parameter);

      // Exit with error code
      exit(EX_FAL);
    }
  }

  // Store parameter head
  m[(*ccurrent)++] = (unsigned char)parametert;

  // Copy parameter content to the memory
  memcpy(m + *ccurrent, parameterc, parametercl);

  // Update memory position
  *ccurrent += parametercl;
}

void compiler_label(char* m, char* buffer, unsigned long int* current, unsigned long int* ccurrent, int labt, struct label_s* labels, unsigned int* labelsc) {
  // Ignore unknown label
  if(labt == LAB_UNKNOWN) return;

  // Ignore first whitespace
  (*current)++;

  // Remember parameter length
  unsigned int plength = 0;

  // Store current character in buffer
  char c = 0;

  // Read to line end
  while(c != '\n') {
    // Read next character
    c = buffer[(*current)++];

    // Check for null terminator
    if(c == 0) break;

    // Increment parameter length
    plength++;
  }

  // Allocate parameter space
  char* parameter = hmalloc(plength);

  // Copy parameter from buffer
  strncpy(parameter, buffer + *current - plength, plength - 1);

  // Store parameter type
  int parametert = PAR_NAME;

  // Recognize parameter one type
  if(parameter[0] == 'r') parametert = PAR_REGISTER;
  else if(parameter[0] == '@') parametert = PAR_ADDRESS;
  else if(parameter[0] == '#') parametert = PAR_VALUE;
  else if(parameter[0] == '\'') parametert = PAR_STRING;

  // Validate parameter type
  if(
    // Validate org label
    !(labt == LAB_ORG && (parametert == PAR_ADDRESS))
    &&
    // Validate nam label
    !(labt == LAB_NAM && (parametert == PAR_NAME))
    &&
    // Validate var label
    !(labt == LAB_VAR && (parametert == PAR_VALUE
                          || parametert == PAR_STRING))
  ) {
    // Print error message
    printf("Wrong parameter types given.\n");

    // Exit with error code
    exit(EX_FAL);
  }

  // Check for org label
  if(labt == LAB_ORG) {
    // Ignore first character
    parameter++;

    // Check for hex format
    if(parameter[0] == 'x') {
      // Translate memory to integer
      *ccurrent = strtol(parameter + 1, NULL, 16);
    } else {
      // Translate memory to integer
      *ccurrent = atol(parameter);
    }

  // Check for nam label
  } else if(labt == LAB_NAM) {
    // Get label struct from list
    struct label_s* labelp = &labels[(*labelsc)++];

    // Update name property
    labelp->name = parameter;

    // Update address parameter
    labelp->address = *ccurrent;

  // Check for var label
  } else if(labt == LAB_VAR) {
    // Copy parameter to memory depending on type
    if(parametert == PAR_STRING) {
      // Ignore first character
      parameter++;

      // Calculate string length
      unsigned int parameterl = strlen(parameter) - 1;

      // Copy parameter content to the memory
      memcpy(m + *ccurrent, parameter, parameterl);

      // Update memory position
      *ccurrent += parameterl;

    } else if(parametert == PAR_VALUE) {
      // Ignore first character
      parameter++;

      // Store parameter value
      unsigned long long int parameterc = 0;

      // Convert number
      if(parameter[0] == 'x') {
        // Convert hex to int
        parameterc = strtoll(parameter + 1, NULL, 16);

      } else {
        // Convert text to int
        parameterc = atoll(parameter);
      }

      // Convert string to number and store in memory
      memcpy(m + *ccurrent, &parameterc, 8);

      // Update code memory position
      *ccurrent += 8;
    }
  }
}
