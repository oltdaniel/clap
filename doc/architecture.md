# Architecture

The architecture of the machine was kept simple, since it is a research project
only. It is based on some common CPU architectures, that had been reduced or
extended depending on the possibilities and virtual machine has on an running
computer.

## Registers

The machine has 20 registers, named from `r00` to `r19`. Each can store values
up to a size of `64bit`. Since this is a virtual machine, these memory sections
are stored in the RAM which is multiple times slower than registers in an real
cpu architecture.

## Memory

The memory in this machine is fixed. It doesn't allow to scale during runtime
or make use of any other memory left on the host machine. It contains the code
memory, the stack space and the heap.

#### Code memory

The code memory is based in the lowest part of the memory, consuming the
address spaces it needs to fit in.

#### Heap

The heap starts at the lowest memory address after the code memory. This
position allows allocating more code memory and add instructions during the
runtime.

#### Stack

The stack grows from the highest to the lowest address and is positioned at the
top of the memory address spaces. Each item has a fixed size of `64bits`.

## Pointers

The machine uses an instruction pointer `ip` in order to track the current
instruction that will be executed, an stack pointer `sp` to keep track of the
current position in the stack and an allocation pointer `ap` to keep track of
the next address the next memory space in the heap will be allocated.

## Flags

The machine contains the following registers:

- **Overflow flag**: to indicate when an arithmetic overflow has occurred
- **Parity flag**: to indicate if the number of set bits is odd or even in the
  binary representation of the last operation
- **Zero flag**: is used to check if the result of an arithmetic operation is 0
- **Negative flag**: to indicate whether the result of the last mathematical
  operation resulted in a value in which the most significant bit was set

###### Source: [Wikipedia](https://en.wikipedia.org/w/index.php?title=FLAGS_register&oldid=823659522)
