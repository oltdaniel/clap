# Interface

The machine is basically an simulated cpu behind a console interface. To
execute programs, you can use the following commands that are available to
execute:

```shell
# Print the current version
$ bin/clap version

# Print an help message
$ bin/clap help

# Run an example
$ bin/clap run examples/add.asm

# Debug an program with debug level 1
$ bin/clap debug examples/add.asm 1
```

## Debug levels

The following debug levels are available for different purposes.

#### 0: Default

The default logging level prints a debug log to the screen to log the actions
and events that are happening while execute your program in the machine. This
level doesn't give the developer access to the memory viewer or register overview.

#### 1: Extended

The extended logging level opens an debugger interface with its own set of
commands. It allows the developer to debug through ech step of the insructions
and view the different memory areas as well as the register overview.

#### 2: Pro

The pro logging level has the same interface as the extended logging level with
the difference that it allows the developers to change numbers and memory
during the runtime of the machine.
