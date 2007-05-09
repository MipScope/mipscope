	.data
	
	CONST = '\t'
label: .asciiz "Debugging\n"

unusedSpace: .asciiz "Unused Space\n"

	.text
	
main:
   li $v0, 4
   la $a0, label
#   li $a0, CONST
#   sw $1, 20($sp)
   
   syscall
   nop
   
   done