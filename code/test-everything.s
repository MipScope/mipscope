# These are tests for every instruction.

.data
	beginmessage:	.asciiz "Begin tests."
	endmessage:		.asciiz "\n\nEnd tests.\n"
	testnum:		.asciiz "Test "
	nwcharacter:	.asciiz "\n"

.text



__start:
	
	# Print out begin message:
	li $v0, 4
	la $a0, beginmessage
	syscall

	li $v0, 1
	li $s8, 0

####################################################################
	# abs
	jal printnewline

	li $t0, 89
	abs $a0, $t0
	syscall
	jal printnewline

	li $t0, -42
	abs $a0, $t0
	syscall
	jal printnewline

	li $a0, -0
	abs $a0, $a0
	syscall
	jal printnewline

	li $a0, 429496729 #add another digit to this once the parser's fixed
	abs $a0, $a0
	syscall
	jal printnewline

	li $a0, -429496729 #add another digit to this once the parser's fixed
	abs $a0, $a0
	syscall
	jal printnewline


	# Print out end message:
	li $v0, 4
	la $a0, endmessage
	syscall

####################################################################
	# add
	

	done


printnewline:
	li $v0, 4
	la $a0, nwcharacter
	syscall
	syscall
	li $v0, 4
	la $a0, testnum
	syscall
	add $s8, $s8, 1
	li $v0, 1
	move $a0, $s8
	syscall
	li $v0, 4
	la $a0, nwcharacter
	syscall
	li $v0, 1
	jr $ra

	
