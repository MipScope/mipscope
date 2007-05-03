# These are tests for every instruction.

.data
	beginmessage:	.asciiz "Begin tests."
	endmessage:		.asciiz "\n\nEnd tests.\n"
	testnum:		.asciiz "Test "
	nwcharacter:	.asciiz "\n"

.text

main:
	
	# Print out begin message:
	li $v0, 4
	la $a0, beginmessage
	syscall
add $a0, $s0, $s1
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
	move $a0, $t0
	syscall
	jal printnewline

	# Test 3
	li $a0, -0
	abs $a0, $a0
	syscall
	jal printnewline
	
	# THIS IS A BUG IN SPIM: (CORRECT OUTPUT: 5, SPIM'S OUTPUT: 2147483647)
#	li $a0, 4294967291
#	abs $a0, $a0
#	syscall
#	jal printnewline

	li $a0, -0xFFFFFFF
	abs $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -0xFFFFFFD
	abs $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 0xFFFFFFFF
	abs $a0, $a0
	syscall
	jal printnewline



####################################################################
	# add
	li $s0, 127328
	li $s1, 151232
	add $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	add $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	add $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	add $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	add $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	add $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	add $a0, $s0, $s1
	syscall	
	jal printnewline
	
		li $s0, 127328
	add $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	add $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	add $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	add $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	add $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	add $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	add $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
####################################################################
	# addu
	li $s0, 127328
	li $s1, 151232
	addu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	addu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	addu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	addu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	addu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	addu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	addu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	addu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	addu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	addu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	addu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	addu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
	
	####################################################################
	# addi
	li $s0, 127328
	addi $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	addi $a0, $s0, -8390
	syscall
	jal printnewline
	
	li $s0, -98736
	addi $a0, $s0, -8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addi $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addi $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	addi $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	addi $a0, $s0, -8983
	syscall	
	jal printnewline
	
####################################################################
	# addiu
	li $s0, 127328
	addiu $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	addiu $a0, $s0, -8390
	syscall
	jal printnewline
	
	li $s0, -98736
	addiu $a0, $s0, -8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addiu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	addiu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	addiu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	addiu $a0, $s0, -8983
	syscall	
	jal printnewline

####################################################################
	# and
	
	li $s0, 0
	li $s1, 151232
	and $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	and $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	and $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	and $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	and $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	and $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	and $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	and $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	and $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	and $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	and $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	and $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	and $a0, $s0, -89832
	syscall	
	jal printnewline
	
####################################################################
	# andi

	li $s0, 0
	andi $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 1
	andi $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 0
	andi $a0, $s0, 1
	syscall
	jal printnewline	

	li $s0, 127328
	andi $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	andi $a0, $s0, 8390
	syscall
	jal printnewline
	
	li $s0, -98736
	andi $a0, $s0, 8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	andi $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	andi $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	andi $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	andi $a0, $s0, 8983
	syscall	
	jal printnewline
	
####################################################################
	# div 3-arg
	
	li $s0, 0
	li $s1, 151232
	div $a0, $s0, $s1
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	div $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	div $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	div $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	div $a0, $s0, $s1
	syscall	
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	div $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	div $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	div $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	div $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	div $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	div $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	div $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	div $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	div $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	div $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	div $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	div $a0, $s0, -89832
	syscall	
	jal printnewline	
	
####################################################################
	# divu 3-arg
	
	li $s0, 0
	li $s1, 151232
	divu $a0, $s0, $s1
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	divu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	divu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	divu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	divu $a0, $s0, $s1
	syscall	
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	divu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	divu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	divu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	divu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	divu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	divu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	divu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	divu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	divu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	divu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	divu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	divu $a0, $s0, -89832
	syscall	
	jal printnewline		

####################################################################
	# div 2-arg
	
	li $s0, 0
	li $s1, 151232
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	div $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline	
	
####################################################################
	# divu 2-arg
	
	li $s0, 0
	li $s1, 151232
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	divu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
####################################################################
	# mult
	
	li $s0, 0
	li $s1, 0
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline	
	
	li $s0, 0
	li $s1, 151232
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2
	li $s1, -3
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline		
	
	li $s0, 0x80000000 #PROBLEM
	li $s1, 2
	mult $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	
	li $s0, 127328
	li $s1, 151232
	mult $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 1273282
	li $s1, 1512325
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
####################################################################
	# multu
	
	li $s0, 0
	li $s1, 0
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline	
	
	li $s0, 0
	li $s1, 151232
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2
	li $s1, -3
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline		
	
	li $s0, 0x80000000 #PROBLEM
	li $s1, 2
	multu $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	
	li $s0, 127328
	li $s1, 151232
	multu $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 1273282
	li $s1, 1512325
	multu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
####################################################################
	# mul
	
	li $s0, 0
	li $s1, 151232
	mul $a0, $s0, $s1
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	mul $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	mul $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	mul $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	mul $a0, $s0, $s1
	syscall	
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	mul $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	mul $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	mul $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	mul $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	mul $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	mul $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	mul $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	mul $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	mul $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	mul $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	mul $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	mul $a0, $s0, -89832
	syscall	
	jal printnewline	


####################################################################
	# mulo
	
	li $s0, 0
	li $s1, 151232
	mulo $a0, $s0, $s1
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	mulo $a0, $s0, $s1
	syscall
	jal printnewline	

	li $s0, 1273
	li $s1, 1512
	mulo $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 51231
	li $s1, -839
	mulo $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -9873
	li $s1, -8983
	mulo $a0, $s0, $s1
	syscall	
	jal printnewline

	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	mulo $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFF
	li $s1, 0xFF
	mulo $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147
	li $s1, -8983
	mulo $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 1273
	mulo $a0, $s0, 1512
	syscall
	jal printnewline	
	
	li $s0, 5123
	mulo $a0, $s0, -8390
	syscall
	jal printnewline
	
	li $s0, -9873
	mulo $a0, $s0, -8983
	syscall	
	jal printnewline	
	
	li $s0, 214
	mulo $a0, $s0, -89
	syscall	
	jal printnewline	
	
	
####################################################################
	# mulou
	
	li $s0, 0
	li $s1, 151232
	mulou $a0, $s0, $s1
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	mulou $a0, $s0, $s1
	syscall
	jal printnewline	

	li $s0, 1273
	li $s1, 1512
	mulou $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 51231
	li $s1, 839
	mulou $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 9873
	li $s1, 8983
	mulou $a0, $s0, $s1
	syscall	
	jal printnewline
		
	li $s0, 0xFFFFFFFF
	li $s1, 1
	mulou $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFF
	li $s1, 0xFF
	mulou $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147
	li $s1, 8983
	mulou $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 1273
	mulou $a0, $s0, 1512
	syscall
	jal printnewline	


	li $s0, 5123
	mulou $a0, $s0, 8390
	syscall
	jal printnewline

	li $s0, 9873
	mulou $a0, $s0, 8983
	syscall	
	jal printnewline	

	li $s0, 214
	mulou $a0, $s0, 89
	syscall	
	jal printnewline
	
	
####################################################################
	# madd
	
	mtlo $0
	mthi $0
	
	li $s0, 0
	li $s1, 0
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 151232
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	mtlo $0
	mthi $0
	
	li $s0, 0xFFFFFFEF
	li $s1, 1
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
		
	
#	li $s0, 0x80000000
#	li $s1, 2
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	jal printnewline
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	
#	li $s0, 127328
#	li $s1, 151232
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	jal printnewline
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 512312
#	li $s1, -8390392
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, -98736
#	li $s1, -89832
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#
#	li $s0, 0xFFFFFFFF
#	li $s1, 0x1
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 0xFFFFFFFF
#	li $s1, 0xFFFFFFFF
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 0xFFFFFFFF
#	li $s1, 1
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, -0xFFFFFFF
#	li $s1, 0xFFFFFFFF
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 2147483647
#	li $s1, -89832
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 127328
#	li $s1, 151232
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 1273282
#	li $s1, 1512325
#	madd $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
	
	
	
	
#############################################
##############################################
##############################################
####################################################
####################################################
#################################################### 	
#############################################
	# Print out end message:
	li $v0, 4
	la $a0, endmessage
	syscall

	li    $v0, 10       # Setup register for exit
    syscall                # Exit
	
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



# unused
printallregisters:
	li $v0, 1
	
	move $a0, $0
	syscall
	
	#move $a0, $1
	#syscall
	
	move $a0, $2
	syscall
	
	move $a0, $3
	syscall
	
	move $a0, $4
	syscall
	
	move $a0, $5
	syscall
	
	move $a0, $6
	syscall
	
	move $a0, $7
	syscall
	
	move $a0, $8
	syscall
	
	move $a0, $9
	syscall
	
	move $a0, $10
	syscall

	move $a0, $11
	syscall
	
	move $a0, $12
	syscall
	
	move $a0, $13
	syscall
	
	move $a0, $14
	syscall
	
	move $a0, $15
	syscall
	
	move $a0, $16
	syscall
	
	move $a0, $17
	syscall
	
	move $a0, $18
	syscall
	
	move $a0, $19
	syscall
	
	move $a0, $20
	syscall
	
	move $a0, $21
	syscall
	
	move $a0, $22
	syscall

	move $a0, $23
	syscall	
	
	move $a0, $24
	syscall
	
	move $a0, $25
	syscall
	
	#move $a0, $26
	#syscall
	
	#move $a0, $27
	#syscall
	
	move $a0, $28
	syscall
	
	move $a0, $29
	syscall
	
	move $a0, $30
	syscall
	
	move $a0, $31
	syscall
	
	mflo $a0
	syscall
	
	mfhi $a0
	syscall
		
	jr $ra

	
