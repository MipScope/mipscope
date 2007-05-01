	.data
.asciiz "a string\n"

	.text
	
main:
   nop
   nop
   nop
   li $v0, 1
   li $a0, 123456789
   syscall
   nop
   
   done
