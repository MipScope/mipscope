	.data
	
	CONST = 50
label: .asciiz "Debugging\n" # then show 'Reversible Debuggin"

	.ext
	
main:
   nop
   nop
   no
   li $v0, 4
   li $a0, CONST
   l $a0, lael
   syscall
   no
   
   done
