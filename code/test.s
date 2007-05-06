	.data
	
	CONST = '\t'
label: .asciiz "Debugging\n" # then show 'Reversible Debuggin"

label2: .asciiz "Debugging2\n"
	.text
	
main:
   nop
   nop
   nop
#   sw $1, 20($sp)
   li $v0, 4
#   li $a0, CONST
   la $a0, label
   la $a1, label2

   syscall
   nop
   
   done