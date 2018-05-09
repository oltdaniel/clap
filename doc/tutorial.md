# Tutorial

In this tutorial we will write a small math application that will
add two numbers together and subtract one from the result.

## Setup

First we need a compiled version of `clap` which we will generate
by compiling the project with `$ make`.

We can verify the success of this process by running `$ bin/clap version`,
which will output us the current version of the vm.

## Write code

To execute some code, we will create a file called `world.asm`. This is where we
will edit our code with out favorite text editor (`$ vim world.asm`).

To remember what this file will be about let us write a small comment:
```assembly
; This file is an short example for the clap vm
```

Then we will move a value _(for example `123`)_ to a register _(here `r00`)_:

```assembly
; Move 123 into register 0
move r00 #123
```

In order to add a value to 123, or any value from register 0, we will write:
```assembly
; Add 500 to register 0
addi r00 #500
```

And since we spoke about addition and subtraction, we can subtract a value from the addition before:
```assembly
; Subtract 600 from register 0
subi r00 #600
```

## Execution

To execute the code we just wrote, open the command line and write:

```shell
# Execute the file with clap
$ bin/clap run world.asm
```
