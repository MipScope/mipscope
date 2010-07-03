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
	
	# note that this test fails for SPIM version < 7.4 due to a bug in SPIM
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
	
####################################################################
	# lui
	lui $a0, 0
	syscall
	jal printnewline	
	
	lui $a0, 1
	syscall
	jal printnewline
	
	lui $a0, 4245
	syscall
	jal printnewline
	
	lui $a0, 65535
	syscall
	jal printnewline
	
	lui $a0, 65531
	syscall
	jal printnewline
	
	lui $a0, 23245
	syscall
	jal printnewline
		
	lui $a0, 7382
	syscall
	jal printnewline	
	
####################################################################
	# li
	li $a0, 0
	syscall
	jal printnewline	
	
	li $a0, 1
	syscall
	jal printnewline
	
	li $a0, -1
	syscall
	jal printnewline
	
	li $a0, 276325624
	syscall
	jal printnewline
	
	li $a0, -23423456
	syscall
	jal printnewline
	
	li $a0, 0xFFFFFFF
	syscall
	jal printnewline
	
	li $a0, -0xFFFFFF
	syscall
	jal printnewline
	
	li $a0, 73829
	syscall
	jal printnewline	
	
####################################################################
	# lb
	
	.data
	int1: .word 15
	int2: .word -13
	string1: .asciiz "abcdefghijklmnopqrstuvqxyz"	

	.text
	lb $a0, int1
	syscall
	jal printnewline
	
	lb $a0, int2
	syscall
	jal printnewline
		
	lb $a0, int1 + 1
	syscall
	jal printnewline
	
#	lb $a0, int2 - 1
#	syscall
#	jal printnewline	
	
	la $a0, string1 + 5
	li $v0, 4 # print_string
	syscall
	li $v0, 1
	jal printnewline	
	
	lb $a0, string1 + 8
	syscall
	jal printnewline
	
####################################################################
	# lbu	
	
	lbu $a0, int1
	syscall
	jal printnewline
	
	lbu $a0, int2
	syscall
	jal printnewline
		
	lbu $a0, int1 + 1
	syscall
	jal printnewline
	
#	lbu $a0, int2 - 1
#	syscall
#	jal printnewline	
	
	la $a0, string1 + 5
	li $v0, 4 # print_string
	syscall
	li $v0, 1
	jal printnewline	
	
	lbu $a0, string1 + 8
	syscall
	jal printnewline
	

####################################################################
	# lh	
	
	lh $a0, int1
	syscall
	jal printnewline
	
	lh $a0, int2
	syscall
	jal printnewline
		
	lh $a0, int1 + 2
	syscall
	jal printnewline
		
	lh $a0, string1 + 8
	syscall
	jal printnewline	
	
####################################################################
	# lhu	
	
	lhu $a0, int1
	syscall
	jal printnewline
	
	lhu $a0, int2
	syscall
	jal printnewline
		
	lhu $a0, int1 + 2
	syscall
	jal printnewline
		
	lhu $a0, string1 + 8
	syscall
	jal printnewline		

####################################################################
	# lw	
	
	lw $a0, int1
	syscall
	jal printnewline
	
	lw $a0, int2
	syscall
	jal printnewline
	
	lw $a0, int1 + 4
	syscall
	jal printnewline
		
	lw $a0, int1 + 8
	syscall
	jal printnewline
		
	lw $a0, string1 + 8
	syscall
	jal printnewline

	la $s0, int1
	lw $a0, ($s0)
	syscall
	jal printnewline
	
	la $s0, int1
	lw $a0, 4($s0)
	syscall
	jal printnewline
	
####################################################################
	# lwl	
	
	li $a0, 0	
	lwl $a0, int1 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int1 + 2
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int1 + 0
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int2
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int1 + 4
	syscall
	jal printnewline
	
	li $a0, 0		
	lwl $a0, int1 + 8
	syscall
	jal printnewline
	
	li $a0, 0		
	lwl $a0, string1 + 8
	syscall
	jal printnewline

	li $a0, 0	
	la $s0, int1
	lwl $a0, ($s0)
	syscall
	jal printnewline
	
	li $a0, 0	
	la $s0, int1
	lwl $a0, 4($s0)
	syscall
	jal printnewline	
	
	li $a0, 0	
	lwl $a0, int1 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int2 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwl $a0, int1 + 3
	syscall
	jal printnewline
	
	li $a0, 3		
	lwl $a0, int1 + 7
	syscall
	jal printnewline
		
	li $a0, 0xA
	lwl $a0, string1 + 7
	syscall
	jal printnewline

	li $a0, 0	
	la $s0, int1
	lwl $a0, 2($s0)
	syscall
	jal printnewline
	
	li $a0, 0xDEADB11F
	la $s0, int1
	lwl $a0, 3($s0)
	syscall
	jal printnewline
	
	li $a0, 0xA6
	lwl $a0, string1 + 5
	syscall
	jal printnewline
	
	li $a0, -0x2345
	lwl $a0, string1 + 3
	syscall
	jal printnewline
	
	
####################################################################
	# lwr	
	
	li $a0, 0	
	lwr $a0, int1 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int1 + 2
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int1 + 0
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int2
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int1 + 4
	syscall
	jal printnewline
	
	li $a0, 0		
	lwr $a0, int1 + 8
	syscall
	jal printnewline
	
	li $a0, 0		
	lwr $a0, string1 + 8
	syscall
	jal printnewline

	li $a0, 0	
	la $s0, int1
	lwr $a0, ($s0)
	syscall
	jal printnewline
	
	li $a0, 0	
	la $s0, int1
	lwr $a0, 4($s0)
	syscall
	jal printnewline	
	
	li $a0, 0	
	lwr $a0, int1 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int2 + 1
	syscall
	jal printnewline
	
	li $a0, 0	
	lwr $a0, int1 + 3
	syscall
	jal printnewline
	
	li $a0, 3		
	lwr $a0, int1 + 7
	syscall
	jal printnewline
		
	li $a0, 0xA
	lwr $a0, string1 + 7
	syscall
	jal printnewline

	li $a0, 0	
	la $s0, int1
	lwr $a0, 2($s0)
	syscall
	jal printnewline
	
	li $a0, 0xDEADB11F
	la $s0, int1
	lwr $a0, 3($s0)
	syscall
	jal printnewline
	
	li $a0, 0xA6
	lwr $a0, string1 + 5
	syscall
	jal printnewline
	
	li $a0, -0x2345
	lwr $a0, string1 + 3
	syscall
	jal printnewline
	
	
#############################################
#  ld
	.data
		double1: .word 0xDEADBEEF, 0xABCDEF
		double2: .word 0x12345678, 0xABBE9
	.text
	
	ld $10, double1
	move $a0, $10
	syscall
	move $a0, $11
	syscall
	jal printnewline
	
	ld $17, double2
	move $a0, $17
	syscall
	move $a0, $18
	syscall
	
	ld $10, double1 + 4
	move $a0, $10
	syscall
	move $a0, $11
	syscall
	jal printnewline

#############################################		
# ulh	
	
	ulh $a0, int1
	syscall
	jal printnewline
	
	ulh $a0, int2
	syscall
	jal printnewline
	
	ulh $a0, int1 + 4
	syscall
	jal printnewline
		
	ulh $a0, int1 + 8
	syscall
	jal printnewline
		
	ulh $a0, string1 + 8
	syscall
	jal printnewline

	la $s0, int1
	ulh $a0, ($s0)
	syscall
	jal printnewline
	
	la $s0, int1
	ulh $a0, 4($s0)
	syscall
	jal printnewline	
	
	
#############################################		
# ulhu	
	
	ulhu $a0, int1
	syscall
	jal printnewline
	
	ulhu $a0, int2
	syscall
	jal printnewline
	
	ulhu $a0, int1 + 1
	syscall
	jal printnewline
	
	ulhu $a0, int2 + 1
	syscall
	jal printnewline
	
	ulhu $a0, int1 + 2
	syscall
	jal printnewline
	
	ulhu $a0, int2 + 2
	syscall
	jal printnewline
	
	ulhu $a0, int1 + 4
	syscall
	jal printnewline
		
	ulhu $a0, int1 + 8
	syscall
	jal printnewline
		
	ulhu $a0, string1 + 8
	syscall
	jal printnewline

	la $s0, int1
	ulhu $a0, ($s0)
	syscall
	jal printnewline
	
	la $s0, int1
	ulhu $a0, 4($s0)
	syscall
	jal printnewline	
	
	
#############################################		
# ulw	
	
	ulw $a0, int1
	syscall
	jal printnewline
	
	ulw $a0, int2
	syscall
	jal printnewline
	
	ulw $a0, int1 + 1
	syscall
	jal printnewline
	
	ulw $a0, int2 + 1
	syscall
	jal printnewline
	
	ulw $a0, int1 + 2
	syscall
	jal printnewline
	
	ulw $a0, int2 + 2
	syscall
	jal printnewline
	
	ulw $a0, int1 + 4
	syscall
	jal printnewline
		
	ulw $a0, int1 + 8
	syscall
	jal printnewline
		
	ulw $a0, string1 + 8
	syscall
	jal printnewline

	la $s0, int1
	ulw $a0, ($s0)
	syscall
	jal printnewline
	
	la $s0, int1
	ulw $a0, 4($s0)
	syscall
	jal printnewline	
	
#############################################		
	# sb	
	.data
	hunk: .space 1000
	
	.text
	
	li $s0, 0xDEADBEEF
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	sb $s0, hunk + 0
	sb $s1, hunk + 1
	sb $s2, hunk + 2
	sb $s3, hunk + 3
	sb $s2, hunk + 4
	sb $0, hunk + 5 #null terminate it
	
	li $v0, 4
	la $a0, hunk
	syscall
	li $v0, 1
	jal printnewline
	
	sb $s0, hunk + 0
	sb $s1, hunk + 1
	sb $0, hunk + 1
	sb $s2, hunk + 2
	sb $s3, hunk + 3
	sb $s2, hunk + 4
	sb $0, hunk + 5 #null terminate it
	
	li $v0, 4
	la $a0, hunk
	syscall
	li $v0, 1
	jal printnewline
	
	sb $0, hunk + 0
	sb $0, hunk + 1
	sb $0, hunk + 2
	sb $0, hunk + 3
	sb $0, hunk + 5 #null terminate it
	
	li $v0, 4
	la $a0, hunk
	syscall
	li $v0, 1
	jal printnewline
	
#############################################		
	# sh
	
	.data
	sphunk: .space 1000
	
	.text
	
	li $s0, 0x2324
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	sh $s0, sphunk + 0
	sh $s1, sphunk + 2
	sh $s2, sphunk + 4
	sh $s3, sphunk + 6
	sh $s2, sphunk + 8
	sh $s2, sphunk + 10
	sh $s2, sphunk + 12
	sh $0, sphunk + 14 #null terminate it
	
	lb $a0, sphunk
	syscall
	jal printnewline
	
	lb $a0, sphunk + 1
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, sphunk
	syscall
	li $v0, 1
	jal printnewline
	
	
#############################################		
	# ush
	
	.data
	ushhunk: .space 1000
	
	.text
	
	li $s0, 0x2324
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	ush $s0, ushhunk + 0
	ush $s1, ushhunk + 3
	ush $s2, ushhunk + 4
	ush $s3, ushhunk + 5
	ush $s2, ushhunk + 6
	ush $s2, ushhunk + 8
	ush $s2, ushhunk + 10
	ush $s2, ushhunk + 12
	ush $0, ushhunk + 14 #null terminate it
	
	lb $a0, ushhunk
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, ushhunk
	syscall
	li $v0, 1
	jal printnewline	
	
	
#############################################		
	# sw
	
	.data
	swhunk: .space 1000
	
	.text
	
	li $s0, 0x2324
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	sw $s0, swhunk + 0
	sw $s1, swhunk + 4
	sw $s2, swhunk + 8
	sw $s3, swhunk + 12
	sw $s2, swhunk + 16
	sw $s2, swhunk + 20
	sw $s2, swhunk + 24
	sw $0, swhunk + 28 #null terminate it
	
	lb $a0, swhunk
	syscall
	jal printnewline
	
	lb $a0, swhunk + 1
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, swhunk
	syscall
	li $v0, 1
	jal printnewline
	
	
#############################################		
	# usw
	
	.data
	uswhunk: .space 1000
	
	.text
	
	li $s0, 0x2324
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	usw $s0, uswhunk + 0
	usw $s1, uswhunk + 2
	usw $s2, uswhunk + 6
	usw $s3, uswhunk + 11
	usw $s2, uswhunk + 16
	usw $s2, uswhunk + 20
	usw $s2, uswhunk + 24
	usw $0, uswhunk + 28 #null terminate it
	
	lb $a0, uswhunk
	syscall
	jal printnewline
	
	lb $a0, uswhunk + 1
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, uswhunk
	syscall
	li $v0, 1
	jal printnewline
		
#############################################		
	# movn, movz
	
	li $s0, 10
	li $s1, 11
	li $s2, 12
	li $a0, 23
	
	movn $a0, $s0, $0
	syscall
	jal printnewline	
	
	movn $a0, $s0, $s1
	syscall
	jal printnewline
	
	movn $a0, $s0, $s0
	syscall
	jal printnewline
	
	
#############################################		
	# swl
	.data
	swlhunk: .space 1000
	
	.text
	
	li $s0, 0xDEADBEEF
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	swl $s0, swlhunk + 0
	swl $s1, swlhunk + 100
	swl $s2, swlhunk + 99
	swl $s3, swlhunk + 3
	swl $s2, swlhunk + 4
	#swl $0, hunk + 5 #null terminate it
	
	lb $a0, swlhunk
	syscall
	jal printnewline
	
	lb $a0, swlhunk + 1
	syscall
	jal printnewline
	
	lb $a0, swlhunk + 100
	syscall
	jal printnewline
	
	lb $a0, swlhunk + 99
	syscall
	jal printnewline
	
	lb $a0, swlhunk + 3
	syscall
	jal printnewline
	
	lb $a0, swlhunk + 4
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, swlhunk
	syscall
	li $v0, 1
	jal printnewline
	
	
#############################################		
#	# swr
	.data
	swrhunk: .space 1000
		
	.text
	
	li $s0, 0xDEADBEEF
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	swr $s0, swrhunk + 0
	swr $s1, swrhunk + 100
	swr $s2, swrhunk + 99
	swr $s3, swrhunk + 3
	swr $s2, swrhunk + 4
	#swl $0, hunk + 5 #null terminate it
	
	lb $a0, swrhunk
	syscall
	jal printnewline
	
	lb $a0, swrhunk + 1
	syscall
	jal printnewline
	
	lb $a0, swrhunk + 100
	syscall
	jal printnewline
	
	lb $a0, swrhunk + 99
	syscall
	jal printnewline
	
	lb $a0, swrhunk + 3
	syscall
	jal printnewline
	
	lb $a0, swrhunk + 4
	syscall
	jal printnewline
	
	li $v0, 4
	la $a0, swrhunk
	syscall
	li $v0, 1
	jal printnewline
	
#############################################
# sd	
	.data
	sdspace: .space 1000
	.text
	
	li $s0, 0xDEADBEEF
	li $s1, 56
	li $s2, -234
	li $s3, 0xFFFFFFFF
	
	sd $s0, sdspace
	
	lw $t0, sdspace
	lw $t1, sdspace + 4
	
	move $a0, $t0
	syscall
	jal printnewline
	
	move $a0, $t1
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




	
