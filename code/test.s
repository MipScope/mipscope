
main:
   li $v0, 1
   li $a0, 33
   syscall
   j main      # infinite loop test
