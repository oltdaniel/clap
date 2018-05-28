# clap :clap:

[![doc](https://img.shields.io/badge/doc-github-green.svg)](https://github.com/oltdaniel/clap/blob/master/doc/README.md)

the vm

## Idea

###### This project is mainly for research purposes.

The idea behind this project is, to create an virtual machine `(=machine)` that
includes all the issues with computers, for example an overflow in the RAM or
wrong memory allocation.

To allow these things there is only a single memory which will contain the
code in the lower part of the memory, followed by some space for the heap
and the stack on the highest address. This will allow the code to change
itself during the runtime.

## Installation

```shell
# Get the code
$ git clone https://github.com/oltdaniel/clap.git
$ cd clap
# Compile the code
$ make
```

#### Usage

```shell
# Execute the machine
$ bin/clap help
# Run an example
$ bin/clap run examples/math.asm
# Clean the project
$ make clean
```

#### Example

```assembly
; Jump over the variable space
jump main

; Define the address label math
.nam math

; Set 64bit of the memory to 0
.var 0

; Define the main label
.nam main

; Move 12 to the register 0
move r00 #12

; Add the value of register 0
; to the variable
addi math r00
```

## About

The clap machine is an `64bit` based virtual machine. It has `20` registers, an
memory that combines the code memory, the heap as well as the stack. To make
use of these parts the clap machine uses its own assembly language, that will
be compiled down to an bytecode file.

One of the first goals is it, to define an ready to use instruction set, that
can be implemented and executed on the machine. For more current `in-progress`
tasks, take a look at the [issues](https://github.com/oltdaniel/clap/issues).

## Documentation

Documentation can be found [here in `/doc`](https://github.com/oltdaniel/clap/blob/master/doc/README.md).

## License

_Just do what you'd like to_

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/oltdaniel/clap/blob/master/LICENSE)

#### Credit

[Daniel Oltmanns](https://github.com/oltdaniel) - creator
