.nam hello
move r00 #1
jump end

; Will not be executed
move r00 #100

.nam end
move r01 #2
halt
