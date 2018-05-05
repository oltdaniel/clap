; Endless loop with address
move r00 #1

.org @x10
addi r00 #1
jump @x10

halt
