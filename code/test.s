	.data
	
CONST = '\t'

label: .asciiz "Pascal Rocks!\n"

	.text

main:
	nop
	nop
	li $v0, 4
	la $a0, label
	
#    	SAVE  VERY  OFTEN!!!!!
    
	syscall

   	li $a0, CONST
   	sw $1, 20($sp)
	
	done
