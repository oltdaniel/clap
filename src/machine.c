#include "machine.h"

struct machine_s* machine_new() {
  // Allocate struct
  struct machine_s* m = hmalloc(sizeof(struct machine_s));

  // Return allocated machine
  return m;
}

int machine_load(struct machine_s* m, char* file) {

  // Return success code
  return EX_OKA;
}

void machine_run(struct machine_s* m) {

}
