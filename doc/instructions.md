# Instructions

This document is an detailed specification of the assembly instructions that
are supported in the machine. The assembly used in this document is similiar to
the existing ones, but with some decisive changes.

### Literals

##### address

An address can be recognized by the `@` followed by an address written in
hexadecimal. **Example**: `@x7B` or `@123`

##### register

An register can be recognized by the `r` followed by an 2 digit number, that
needs to be in the range of `00` to `r19` to get access to one of the 20
registers in this machine. **Example**: `r02`

##### name

An name is just an written word without any special characters to be recognized
by the compiler. The only requirement is, that it needs to be alphanumeric to
avoid compiler errors. It will be translated to an address in the memory. **Example**: `nameThis`

###### **NOTE: Do not start the label with `r` since this indicates an register**

##### value

An value is recognized by an `#` followed by the actual value. This value needs
to be decimal or hexadecimal. **Example**: `#123` or `#x7B`

### Labels

Lables are used to tell the compiler which address should be currently used,
which name the code memory has to allow jumps, or which value should be stored
at the current position. Each label requires an `.` before it is written to be
recognizable by the compiler.

| Name | Parameters | Description |
|-|-|-|
| `org` | address | Changes the current position in the memory of the code |
| `name` | name | Gives the current memory address a name to allow jumps |
| `var` | name, value | Puts the value at the current memory address with the specific name |

##### `org`

```assembly
.org x0
; The current memory address for the code is 0x0

.org x512
; The current memory address for the code is 0x512
```

##### `nam`

```assembly
.nam loop
; Gives the address an alias `loop`

.nam exit
; Gives the address an alias `exit`
```

##### `var`

```assembly
.nam message
.var 'Hello World'
; The memory address with the name message contains the value 'Hello World'
```

### Instructions

Instructions are executed by the machine with the specific parameters. Each
instruction is 4 characters long, parameters are optional, but separated by
a space if they are used. _(The `ID` is the number that can be used in the bytecode, also remember that a name is equal to a register as a parameter)_

| ID | Name | Parameters | Description |
|-|-|-|-|
| 1 | `noop` | _none_ | Consumes an cpu cycle like a sleep function |
| 2 | `halt` | _none_ | Stops the machine from running |
| 3 | `allo` | address/register, address/register/value | Allocates x bytes (second parameter) and stores the address into the first parameter |
| 4 | `pope` | address/register | Pops an element from the stack into the destination |
| 5 | `push` | address/register/value | Push an element to the stack |
| 6 | `junz` | name | Jumps if zero flag is not activated |
| 7 | `jumz` | name | Jumps if the zero flag is activated |
| 8 | `jump` | name | Jumps to an specified label |
| 9 | `cmpz` | address/register/value | Checks if the value of the parameter is zero, if true set the zero flag |
| 10 | `flaz` | _none_ | Clears all flags to the default state |
| 11 | `movz` | register | Clears the first parameter value |
| 12 | `move` | address/register, address/register/value | Moves the value of the second parameter to the first parameter |
| 13 | `subi` | address/register, address/register/value | Subtracts the value of both parameters and stores it in the first parameter |
| 14 | `addi` | address/register, address/register/value | Adds the value of both parameters and stores it in the first parameter |
| 15 | `muli` | address/register, address/register/value | Multiplies the first parameter with the second parameter and stores it in the first parameter |
| 16 | `divi` | address/register, address/register/value | Divides the first parameter with the second one and stores it in the first parameter |

###### **TODO**: Add more instructions
