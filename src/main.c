#include "main.h"

/**
 * The main function is the entry point of this whole program. It contains the
 * console interface as well as booting the machine.
 */
int main(int argc, char* argv[]) {
  /*
   * Validate given arguments
   */
  // Require 2 arguments (clap + [command])
  if(argc < 2) {
    // Print error message
    printf("Please use 2 or more arguments.\n");

    // Exit with error code
    return EX_CLI;
  }

  /*
   * Read default arguments
   */
  // Store command
  char* command = argv[1];

  // Check for help command
  if(strcmp(command, "help") == 0) {
    // Print help message
    printf(HELP, VERSION);

    // Exit with success code
    return EX_OKA;
  }

  // Check for version command
  if(strcmp(command, "version") == 0) {
    // Print version number
    printf("%s\n", VERSION);

    // Exit with success code
    return EX_OKA;
  }

  /*
   * Require an extra argument
   */
  // Require 3 arguments (clap + [command] + [file])
  if(argc < 3) {
    // Print error message
    printf("Please provide an file to execute.\n");

    // Exit with error code
    return EX_CLI;
  }

  // Read file argument
  char* file = argv[2];

  /*
   * Read machine arguments
   */
  // Check for run command
  if(strcmp(command, "run") == 0) {
    // Initialize new machine
    struct machine_s* m = machine_new();

    // Load program
    int status = machine_load(m, file);

    // Run machine
    machine_run(m);

    // Exit with success code
    return EX_OKA;

  // Check for debug command
  } else if(strcmp(command, "debug") == 0) {
    // Initialize new machine

    // Load program

    // Initialize debugger

    // Run debugger

    // Exit with success code
    return EX_OKA;
  }

  /*
   * Exit with error message
   */
  // Print error message
  printf("Please lookup `clap help` to see how it works.\n");

  // Exit with error code
  return EX_CLI;
}
