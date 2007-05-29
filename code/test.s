	.data
	
	CONST = '\t'
label: .asciiz "Debugging\n"

unusedSpace: .asciiz "Unused Space\n"

	.text
	
main:
   li $v0, 4
   la $a0, label
	sw $v0, label
#   li $a0, CONST
#   sw $1, 20($sp)
	
   li $v0, 5
   syscall
   move $a0, $v0
   li $v0, 1
   syscall



   la $10, unusedSpace
   sw $1, 4096($10)
	and $1, $2, $zero
   
   syscall
   nop
   
   done
