; Endless loop
move r00 #1

.org @x10
.nam hello
addi r00 #1
jump hello

halt
