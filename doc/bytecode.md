# Bytecode

In order to execute the given assembly code, the input file will be translated to bytecode. In this document you will find a description of how the bytecode looks and which byte causes which actions.

## Instructions

An instruction will consume `8bit` in the memory. The first byte of the memory will be the starting instruction that will be read and executed.

## Parameters

An parameter will use `8/32/64bits` depending on its type. An register will use `8bits`, and address to a memory location `32bits` and a value based parameter `64bit`.

## Variables

An variable that will be directly written to the memory, will consume as much memory as it requires. A value will consume `64bits`, an address `32bit` and a string as much memory as required.

# Example

Here the example memory of `examples/math.asm` to the time of this commit _(commit before was [d3ee64e3f81e3face8d9961723adec9b5a49ba7b](https://github.com/oltdaniel/clap/commit/d3ee64e3f81e3face8d9961723adec9b5a49ba7b))_:

```
memory:
 0000 | 0b 02 00 03 0c 00 00 00 00 00 00 00 0d 02 00 03
 0010 | 03 00 00 00 00 00 00 00 0b 02 01 03 0c 00 00 00
 0020 | 00 00 00 00 0c 02 01 03 02 00 00 00 00 00 00 00
 0030 | 0b 02 02 03 b0 04 00 00 00 00 00 00 0d 02 02 03
 0040 | c4 09 00 00 00 00 00 00 0b 02 03 03 e0 2e 00 00
 0050 | 00 00 00 00 0c 02 03 03 d0 07 00 00 00 00 00 00
 0060 | 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 0070 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

Source of the assembly:
```assembly
; Add some numbers
move r00 #12
addi r00 #3

; Substract some numbers
move r01 #12
subi r01 #2

; Add some big numbers
move r02 #1200
addi r02 #2500

; Substract some big numbers
move r03 #12000
subi r03 #2000

halt
```
