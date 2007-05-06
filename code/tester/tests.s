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
	li $v0, 1


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
	
	
##################################################################
	# clo

	li $a0, 0
	clo $a0, $a0
	syscall
	jal printnewline
	
	li $s0, -1
	clo $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 1
	clo $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 127328
	clo $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 512312
	clo $a0, $s0
	syscall
	jal printnewline
	
	li $s0, -98736
	clo $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	clo $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	clo $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	clo $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	clo $a0, $s0
	syscall	
	jal printnewline	
	

##################################################################
	# clz

	li $a0, 0
	clz $a0, $a0
	syscall
	jal printnewline
	
	li $s0, -1
	clz $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 1
	clz $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 127328
	clz $a0, $s0
	syscall
	jal printnewline	
	
	li $s0, 512312
	clz $a0, $s0
	syscall
	jal printnewline
	
	li $s0, -98736
	clz $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	clz $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	clz $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	clz $a0, $s0
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	clz $a0, $s0
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
	
	li $s0, 0xFFFFFFEF
	li $s1, 1
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0x80000000
	li $s1, 2
	madd $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	
	li $s0, 127328
	li $s1, 151232
	madd $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 1273282
	li $s1, 1512325
	madd $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	

####################################################################
	# msub
	
	mtlo $0
	mthi $0
	
	li $s0, 0
	li $s1, 0
	msub $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 151232
	msub $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	msub $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFEF
	li $s1, 1
	msub $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0x80000000
	li $s1, 2
	msub $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	
	li $s0, 127328
	li $s1, 151232
	msub $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	msub $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
#	li $s0, -98736
#	li $s1, -89832
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#
#	li $s0, 0xFFFFFFFF
#	li $s1, 0x1
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
	
#	li $s0, 0xFFFFFFFF
#	li $s1, 0xFFFFFFFF
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
	
#	li $s0, 0xFFFFFFFF
#	li $s1, 1
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
	
#	li $s0, -0xFFFFFFF
#	li $s1, 0xFFFFFFFF
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 2147483647
#	li $s1, -89832
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
	
#	li $s0, 127328
#	li $s1, 151232
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline
#	
#	li $s0, 1273282
#	li $s1, 1512325
#	msub $s0, $s1
#	mflo $a0
#	syscall
#	mfhi $a0
#	syscall
#	jal printnewline

	mtlo $0
	mthi $0


##################################################################
	# maddu
	
	li $s0, 0
	li $s1, 0
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 151232
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
		
	li $s0, 1
	li $s1, 1
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFEF
	li $s1, 1
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0x80000000
	li $s1, 2
	maddu $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	
	li $s0, 127328
	li $s1, 151232
	maddu $s0, $s1
	mflo $a0
	syscall
	jal printnewline
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 512312
	li $s1, -8390392
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline

	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 151232
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	li $s0, 1273282
	li $s1, 1512325
	maddu $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall
	jal printnewline
	
	
####################################################################
	# neg
	jal printnewline
	
	li $t0, 1
	neg $a0, $t0
	syscall
	jal printnewline
	
	
	li $t0, -1
	neg $a0, $t0
	syscall
	jal printnewline
	
	li $t0, 0
	neg $a0, $t0
	syscall
	jal printnewline

	li $t0, 89
	neg $a0, $t0
	syscall
	jal printnewline
	
	li $t0, -89
	neg $a0, $t0
	syscall
	jal printnewline

	li $a0, -0
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 0xDEADBEEF
	neg $a0, $a0
	syscall
	jal printnewline

	li $a0, -0xFFFFFFF
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -0xFFFFFFD
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 0xFFFFFFFF
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -2147483643 # most negative value
	neg $a0, $a0
	syscall
	jal printnewline	
	
	li $a0, 2147483643
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 2147483647
	neg $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -2147483647
	neg $a0, $a0
	syscall
	jal printnewline


####################################################################
	# negu
	jal printnewline
	
	li $t0, 1
	negu $a0, $t0
	syscall
	jal printnewline
	
	
	li $t0, -1
	negu $a0, $t0
	syscall
	jal printnewline
	
	li $t0, 0
	negu $a0, $t0
	syscall
	jal printnewline

	li $t0, 89
	negu $a0, $t0
	syscall
	jal printnewline
	
	li $t0, -89
	negu $a0, $t0
	syscall
	jal printnewline

	li $a0, -0
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 0xDEADBEEF
	negu $a0, $a0
	syscall
	jal printnewline

	li $a0, -0xFFFFFFF
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -0xFFFFFFD
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 0xFFFFFFFF
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -2147483648 # most negative value
	negu $a0, $a0
	syscall
	jal printnewline	
	
	li $a0, 2147483640
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, 2147483647
	negu $a0, $a0
	syscall
	jal printnewline
	
	li $a0, -2147483647
	negu $a0, $a0
	syscall
	jal printnewline
	
	
	
####################################################################
	# nor
	
	li $s0, 0
	li $s1, 151232
	nor $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	nor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	nor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	nor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	nor $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	nor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	nor $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	nor $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	nor $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	nor $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	nor $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	nor $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	nor $a0, $s0, -89832
	syscall	
	jal printnewline	

####################################################################
	# or
	
	li $s0, 0
	li $s1, 151232
	or $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	or $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	or $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	or $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	or $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	or $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	or $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	or $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	or $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	or $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	or $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	or $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	or $a0, $s0, -89832
	syscall	
	jal printnewline	
	
	
####################################################################
	# ori

	li $s0, 0
	ori $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 1
	ori $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 0
	ori $a0, $s0, 1
	syscall
	jal printnewline	

	li $s0, 127328
	ori $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	ori $a0, $s0, 8390
	syscall
	jal printnewline
	
	li $s0, -98736
	ori $a0, $s0, 8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	ori $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	ori $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	ori $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	ori $a0, $s0, 8983
	syscall	
	jal printnewline	
	

####################################################################
	# rem
	li $s0, 127328
	li $s1, 151232
	rem $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	rem $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	rem $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	rem $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	rem $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	rem $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	rem $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	rem $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	rem $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	rem $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	rem $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	rem $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	rem $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	rem $a0, $s0, -89832
	syscall	
	jal printnewline	
	
	
####################################################################
	# remu
	li $s0, 127328
	li $s1, 151232
	remu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	remu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	remu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	remu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	remu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	remu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	remu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	remu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	remu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	remu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	remu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	remu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	remu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	remu $a0, $s0, -89832
	syscall	
	jal printnewline	
	
	
####################################################################
	# sll
	
	li $s0, 0
	sll $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 0
	sll $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 1
	sll $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 1
	sll $a0, $s0, 1
	syscall
	jal printnewline

	li $s0, 127328
	sll $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 127328
	sll $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 2
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 3
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 5
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 6
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 7
	syscall
	jal printnewline	
	
	li $s0, 127328
	sll $a0, $s0, 8
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 9
	syscall
	jal printnewline	
	
	li $s0, 127328
	sll $a0, $s0, 28
	syscall
	jal printnewline		
	
	li $s0, 127328
	sll $a0, $s0, 29
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 30
	syscall
	jal printnewline
	
	li $s0, 127328
	sll $a0, $s0, 31
	syscall
	jal printnewline	
	
	li $s0, 127328
	sll $a0, $s0, 23
	syscall
	jal printnewline
	
	li $s0, 512312
	sll $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, -98736
	sll $a0, $s0, 25
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sll $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sll $a0, $s0, 17
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sll $a0, $s0, 14
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sll $a0, $s0, 30
	syscall	
	jal printnewline		
	
	


####################################################################
	# srl
	
	li $s0, 0
	srl $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 0
	srl $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 1
	srl $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 1
	srl $a0, $s0, 1
	syscall
	jal printnewline

	li $s0, 127328
	srl $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 127328
	srl $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 2
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 3
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 5
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 6
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 7
	syscall
	jal printnewline	
	
	li $s0, 127328
	srl $a0, $s0, 8
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 9
	syscall
	jal printnewline	
	
	li $s0, 127328
	srl $a0, $s0, 28
	syscall
	jal printnewline		
	
	li $s0, 127328
	srl $a0, $s0, 29
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 30
	syscall
	jal printnewline
	
	li $s0, 127328
	srl $a0, $s0, 31
	syscall
	jal printnewline	
	
	li $s0, 127328
	srl $a0, $s0, 23
	syscall
	jal printnewline
	
	li $s0, 512312
	srl $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, -98736
	srl $a0, $s0, 25
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	srl $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	srl $a0, $s0, 17
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	srl $a0, $s0, 14
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	srl $a0, $s0, 25
	syscall	
	jal printnewline

	
	
####################################################################
	# sllv

	li $s0, 0
	li $s1, 0
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 1
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 1
	sllv $a0, $s0, $s1
	syscall
	jal printnewline

	li $s0, -1
	li $s1, 1
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1	
	li $s1, -1
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 11234545
	li $s1, 13
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1222	
	li $s1, -3
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0,-123423	
	li $s1, 10
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 2
	sllv $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 127328
	li $s1, 151232
	sllv $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sllv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sllv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sllv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sllv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sllv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sllv $a0, $s0, $s1
	syscall	
	jal printnewline
	
####################################################################
	# srlv

	li $s0, 0
	li $s1, 0
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 1
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 1
	srlv $a0, $s0, $s1
	syscall
	jal printnewline

	li $s0, -1
	li $s1, 1
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1	
	li $s1, -1
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 11234545
	li $s1, 13
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1222	
	li $s1, -3
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0,-123423	
	li $s1, 10
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 2
	srlv $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 127328
	li $s1, 151232
	srlv $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	srlv $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	srlv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	srlv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	srlv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	srlv $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	srlv $a0, $s0, $s1
	syscall	
	jal printnewline	
	
####################################################################
	# srav

	li $s0, 0
	li $s1, 0
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 1
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 1
	srav $a0, $s0, $s1
	syscall
	jal printnewline

	li $s0, -1
	li $s1, 1
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1	
	li $s1, -1
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 11234545
	li $s1, 13
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1222	
	li $s1, -3
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0,-123423	
	li $s1, 10
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 127328
	li $s1, 2
	srav $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 127328
	li $s1, 151232
	srav $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	srav $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	srav $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	srav $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	srav $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	srav $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	srav $a0, $s0, $s1
	syscall	
	jal printnewline	
	
	
	
####################################################################
	# sra
	
	li $s0, 0
	sra $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 0
	sra $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 1
	sra $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 1
	sra $a0, $s0, 1
	syscall
	jal printnewline

	li $s0, 127328
	sra $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 127328
	sra $a0, $s0, 1
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 2
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 3
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 5
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 6
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 7
	syscall
	jal printnewline	
	
	li $s0, 127328
	sra $a0, $s0, 8
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 9
	syscall
	jal printnewline	
	
	li $s0, 127328
	sra $a0, $s0, 28
	syscall
	jal printnewline		
	
	li $s0, 127328
	sra $a0, $s0, 29
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 30
	syscall
	jal printnewline
	
	li $s0, 127328
	sra $a0, $s0, 31
	syscall
	jal printnewline	
	
	li $s0, 127328
	sra $a0, $s0, 23
	syscall
	jal printnewline
	
	li $s0, 512312
	sra $a0, $s0, 4
	syscall
	jal printnewline
	
	li $s0, -98736
	sra $a0, $s0, 25
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sra $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sra $a0, $s0, 17
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sra $a0, $s0, 14
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sra $a0, $s0, 25
	syscall	
	jal printnewline	
	
	
	
	
####################################################################
	# rol
	li $s0, 127328
	li $s1, 151232
	rol $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	rol $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	rol $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	rol $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	rol $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xF
	rol $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, 15
	rol $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	rol $a0, $s0, 5
	syscall
	jal printnewline	
	
	li $s0, 512312
	rol $a0, $s0, 8
	syscall
	jal printnewline
	
	li $s0, -98736
	rol $a0, $s0, 4
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	rol $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	rol $a0, $s0, 0xF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	rol $a0, $s0, 0xF
	syscall	
	jal printnewline
	
	li $s0, 21
	rol $a0, $s0, 1
	syscall	
	jal printnewline
		
	
####################################################################
	# ror
	li $s0, 127328
	li $s1, 151232
	ror $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	ror $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	ror $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	ror $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	ror $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xF
	ror $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, 15
	ror $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	ror $a0, $s0, 5
	syscall
	jal printnewline	
	
	li $s0, 512312
	ror $a0, $s0, 8
	syscall
	jal printnewline
	
	li $s0, -98736
	ror $a0, $s0, 4
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	ror $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	ror $a0, $s0, 0xF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	ror $a0, $s0, 0xF
	syscall	
	jal printnewline
	
	li $s0, 21
	ror $a0, $s0, 1
	syscall	
	jal printnewline
	
	
####################################################################
	# sub
	li $s0, 127328
	li $s1, 151232
	sub $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sub $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, -0xFFFFFFF
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 214748364
	li $s1, -89832
	sub $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sub $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sub $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sub $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sub $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sub $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sub $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 214748364
	sub $a0, $s0, -89832
	syscall	
	jal printnewline	
	
	
####################################################################
	# subu
	li $s0, 127328
	li $s1, 151232
	subu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	subu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	subu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	subu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	subu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	subu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647 
	li $s1, -89832
	subu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	subu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	subu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	subu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	subu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	subu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	subu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	subu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
####################################################################
	# xor
	
	li $s0, 0
	li $s1, 151232
	xor $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	xor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	xor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 127328
	li $s1, 151232
	xor $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	xor $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	xor $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	xor $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	xor $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	xor $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	xor $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	xor $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	xor $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	xor $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
	
####################################################################
	# xori

	li $s0, 0
	xori $a0, $s0, 0
	syscall
	jal printnewline	
	
	li $s0, 1
	xori $a0, $s0, 0
	syscall
	jal printnewline
	
	li $s0, 0
	xori $a0, $s0, 1
	syscall
	jal printnewline	

	li $s0, 127328
	xori $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	xori $a0, $s0, 8390
	syscall
	jal printnewline
	
	li $s0, -98736
	xori $a0, $s0, 8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	xori $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	xori $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	xori $a0, $s0, 0xFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	xori $a0, $s0, 8983
	syscall	
	jal printnewline
	
	
	
####################################################################
	# slt
	
	li $s0, 0
	li $s1, 0
	slt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	slt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	slt $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	slt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	slt $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	slt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	slt $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	slt $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	slt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	slt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	slt $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	slt $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	slt $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	slt $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	slt $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	slt $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	slt $a0, $s0, -89832
	syscall	
	jal printnewline	
	
####################################################################
	# sltu
	
	li $s0, 0
	li $s1, 0
	sltu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sltu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sltu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sltu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sltu $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sltu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sltu $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sltu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sltu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sltu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sltu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sltu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sltu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sltu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sltu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sltu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sltu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
####################################################################
	# sgt
	
	li $s0, 0
	li $s1, 0
	sgt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sgt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sgt $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sgt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sgt $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sgt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sgt $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sgt $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sgt $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sgt $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sgt $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sgt $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sgt $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgt $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgt $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sgt $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sgt $a0, $s0, -89832
	syscall	
	jal printnewline
	
	
####################################################################
	# sgtu
	
	li $s0, 0
	li $s1, 0
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sgtu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sgtu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sgtu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sgtu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sgtu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgtu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgtu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sgtu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sgtu $a0, $s0, -89832
	syscall	
	jal printnewline	
	
		
####################################################################
	# sge
	
	li $s0, 0
	li $s1, 0
	sge $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sge $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sge $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sge $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sge $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sge $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sge $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sge $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sge $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sge $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sge $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sge $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sge $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sge $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sge $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sge $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sge $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	sge $a0, $s0, 234232
	syscall	
	jal printnewline	
	


####################################################################
	# sgeu
	
	li $s0, 0
	li $s1, 0
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sgeu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sgeu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sgeu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sgeu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sgeu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgeu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sgeu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sgeu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sgeu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	sgeu $a0, $s0, 234232
	syscall	
	jal printnewline
	
####################################################################
	# sle
	
	li $s0, 0
	li $s1, 0
	sle $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sle $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sle $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sle $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sle $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sle $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sle $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sle $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sle $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sle $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sle $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sle $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sle $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sle $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sle $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sle $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sle $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	sle $a0, $s0, 234232
	syscall	
	jal printnewline	
	
		
####################################################################
	# sleu
	
	li $s0, 0
	li $s1, 0
	sleu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sleu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sleu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sleu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sleu $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sleu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sleu $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sleu $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sleu $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sleu $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sleu $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sleu $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sleu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sleu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sleu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sleu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sleu $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	sleu $a0, $s0, 234232
	syscall	
	jal printnewline
	
####################################################################
	# seq
	
	li $s0, 0
	li $s1, 0
	seq $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	seq $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	seq $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	seq $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	seq $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	seq $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	seq $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	seq $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	seq $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	seq $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	seq $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	seq $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	seq $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	seq $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	seq $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	seq $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	seq $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	seq $a0, $s0, 234232
	syscall	
	jal printnewline		
	
	
####################################################################
	# sne
	
	li $s0, 0
	li $s1, 0
	sne $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 0
	li $s1, 151232
	sne $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 1
	li $s1, 0
	sne $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 1
	li $s1, 1
	sne $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -1
	li $s1, -1
	sne $a0, $s0, $s1
	syscall
	jal printnewline		
	
	li $s0, 0
	li $s1, 1
	sne $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, 4
	li $s1, 3
	sne $a0, $s0, $s1
	syscall
	jal printnewline			
	
	li $s0, 127328
	li $s1, 151232
	sne $a0, $s0, $s1
	syscall
	jal printnewline	
	
	li $s0, 512312
	li $s1, -8390392
	sne $a0, $s0, $s1
	syscall
	jal printnewline
	
	li $s0, -98736
	li $s1, -89832
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0x1
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0xFFFFFFFF
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	li $s1, 0
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	li $s1, 0xFFFFFFFF
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	li $s1, -89832
	sne $a0, $s0, $s1
	syscall	
	jal printnewline
	
	li $s0, 127328
	sne $a0, $s0, 151232
	syscall
	jal printnewline	
	
	li $s0, 512312
	sne $a0, $s0, -8390392
	syscall
	jal printnewline
	
	li $s0, -98736
	sne $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sne $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sne $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sne $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sne $a0, $s0, -89832
	syscall	
	jal printnewline
	
	li $s0, 234232
	sne $a0, $s0, 234232
	syscall	
	jal printnewline			
	
	
####################################################################
	# slti
	li $s0, 127328
	slti $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	slti $a0, $s0, -8390
	syscall
	jal printnewline
	
	li $s0, -98736
	slti $a0, $s0, -8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	slti $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	slti $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	slti $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	slti $a0, $s0, -8983
	syscall	
	jal printnewline
	
####################################################################
	# sltiu
	li $s0, 127328
	sltiu $a0, $s0, 15128
	syscall
	jal printnewline	
	
	li $s0, 512312
	sltiu $a0, $s0, -8390
	syscall
	jal printnewline
	
	li $s0, -98736
	sltiu $a0, $s0, -8983
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sltiu $a0, $s0, 1
	syscall	
	jal printnewline
	
	li $s0, 0xFFFFFFFF
	sltiu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, -0xFFFFFFF
	sltiu $a0, $s0, 0xFFFFFFFF
	syscall	
	jal printnewline
	
	li $s0, 2147483647
	sltiu $a0, $s0, -8983
	syscall	
	jal printnewline	
	
	
	
	
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

	
