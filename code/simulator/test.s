PRINT_SYSCALL = 1
addr:      .word    1, 2, 3, 4
newLine:   .asciiz  "\n"

main:
   li $v0, PRINT_SYSCALL
   #li $t0, 640001
   #addi $a0, $t0, 40
   #lw $a0, addr
   li $t0, 33
   sw $t0, 4+addr
   la $t0, addr
   lw $a0, 4($t0)
   
   assertequals $a0, 33
   
   syscall
   

