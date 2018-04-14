#include "helpers.h"

void* hmalloc(size_t size) {
  // Allocate space
  void* space = malloc(size);

  // Check for error
  if(space == NULL) {
    // Print error message
    printf("Error at allocating %zubytes.\n", size);

    // Exit with error code
    exit(EX_FAL);
  }

  // Return address
  return space;
}
