move r00 #2

.nam loop
subi r00 #1
cmpz r00
jumz end
jump loop

.nam end
halt
