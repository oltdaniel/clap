# clap :clap:

the vm

## Idea

###### This project is mainly for research purposes.

The idea behind this project is, to create an virtual machine `(=machine)` that
includes all the issues with computers, for example an overflow in the RAM or
wrong memory allocation.

To allow these things there is only a single memory. In this part we have the
code in the lower part of the memory. After the code there is space for the
heap, and at the highest address you will find the stack, which leads to an
other architecture than other machines are using.

## Installation

```shell
# Get the code
$ git clone https://github.com/oltdaniel/clap.git
$ cd clap
# Compile the code
$ make
# Execute the machine
$ bin/clap help
```

## Documentation

Here some descriptions to the single parts of the machine:

- [Description](https://github.com/oltdaniel/clap/blob/master/doc/description.md)
- [Architecture](https://github.com/oltdaniel/clap/blob/master/doc/architecture.md)
- [Instructions](https://github.com/oltdaniel/clap/blob/master/doc/instructions.md)
- [Examples](https://github.com/oltdaniel/clap/blob/master/doc/examples.md)
- [Tutorial](https://github.com/oltdaniel/clap/blob/master/doc/tutorial.md)

## About

The clap machine is an 64bit based virtual machine. It has 20 registers, an
memory that combines the code memory, the heap as well as the stack. To make
use of these parts the clap machine uses its own assembly language, that will
be compiled down to an bytecode file.

One of the first goals is it, to define an ready to use instruction set, that
can be implemented and executed on the machine. For more current `in-progress`
tasks, take a look at the [issues](https://github.com/oltdaniel/clap/issues).

## License

_Just do what you'd like to_

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/oltdaniel/timeparse.cr/blob/master/LICENSE)

#### Credit

[Daniel Oltmanns](https://github.com/oltdaniel) - creator
