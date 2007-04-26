PRINT_SYSCALL = 1
addr:      .word    30001

main:
   li $v0, PRINT_SYSCALL
   li $t0, 640001
   addi $a0, $t0, 40
   #lw $a0, addr
   syscall

