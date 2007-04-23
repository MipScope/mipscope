# Containing tests which have passed parser -
# -------------------------------------------

# ------------------
# Instruction Tests:
# ------------------
add $t0, $t1, $t2
add $r31,$sp,     -43
testLabel:__test__=4
la $s1, CONSTANT($sp)# test comment.global newLine

# ---------------
# Directive Tests
# ---------------
CONSTANT= -42
newLine:             .ascii           "__asdlkeowweoeiofs:.fei''"
.align 2
.global newLine
   .data 0x80001
   .text

.ktext 007

.space 40
.byte 10
.byte 31:40
labelB:     .byte 1,-1,       2, -2,      3, -3
.half 10
.half 10:40
labelH:.half 1, -1,2, -2, 3, -3               
.word 10
.word 10:40
labelW:.word 1, -1, 2, -2, 3,    -3


