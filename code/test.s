	.data
	
	CONST = '\t'
label: .asciiz "Debugging\n" # then show 'Reversible Debuggin"

unusedSpace: .asciiz "Unused Space\n"

	.text
	
main:
   nop
   nop
   nop
#   sw $1, 20($sp)
   li $v0, 4
#   li $a0, CONST
   la $a0, label
   la $a1, unusedSpace

   syscall
   nop
   
   done