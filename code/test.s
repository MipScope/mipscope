
# "  " #

main:
   jal test
   
   li $v0, 1
   li $a0, 3333
   syscall

   done

test:
   jr $ra
