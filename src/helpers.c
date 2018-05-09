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

void hex_dump(char* title, void *addr, uint16_t len) {
  // Convert pointer to buffer
  uint8_t* buf = (uint8_t*)addr;

  // Print dump title
  printf("%s:", title);

  // Print memory by 16 bytes a row
  for(uint16_t i = 0; i < len; i++) {
    // Check for new row
    if((i % 16) == 0) {
      // Print some padding
      printf("\n %04x | ", i);
    }

    // Print byte in hex format
    printf("%02x ", buf[i]);
  }

  // Break line
  printf("\n");
}
