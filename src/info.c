#include "info.h"

const char* VERSION = "v0.0.1p0";
const char* HELP = "Welcome to `clap` %s!\n\n" \
                    "The default usage is `clap [command] {arguments}`\n\n" \
                    "Command: help|version|run|debug\n" \
                    "Agruments: Filename of the program to execute|Debug level\n\n" \
                    "Examples:\n" \
                    "`clap help` -> will print this message\n" \
                    "`clap run examples/add.asm` -> will execute an example code\n" \
                    "`clap debug examples/add.asm 1` -> will debug the machine with level 1\n\n" \
                    "(More infos at https://github.com/oltdaniel/clap)\n\n";
