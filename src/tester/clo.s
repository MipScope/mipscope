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



##################################################################
	# clo

	lui $a0, 0
	clo $a0, $a0
	syscall
	jal printnewline
	
#	li $s0, -1
#	clo $a0, $s0
#	syscall
#	jal printnewline	
#	
#	li $s0, 1
#	clo $a0, $s0
#	syscall
#	jal printnewline	
#	
#	li $s0, 127328
#	clo $a0, $s0
#	syscall
#	jal printnewline	
#	
#	li $s0, 512312
#	clo $a0, $s0
#	syscall
#	jal printnewline
#	
#	li $s0, -98736
#	clo $a0, $s0
#	syscall	
#	jal printnewline
#	
#	li $s0, 0xFFFFFFFF
#	clo $a0, $s0
#	syscall	
#	jal printnewline
#	
#	li $s0, 0xFFFFFFFF
#	clo $a0, $s0
#	syscall	
#	jal printnewline
#	
#	li $s0, -0xFFFFFFF
#	clo $a0, $s0
#	syscall	
#	jal printnewline
#	
#	li $s0, 2147483647
#	clo $a0, $s0
#	syscall	
#	jal printnewline
	
#########################################################
## Print out end message:
	li $v0, 4
	la $a0, endmessage
	syscall

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