.data


.text
	
main:

	mtlo $0
	mthi $0

	li $s0, 1
	li $s1, -1
	madd $s0, $s1
	mfhi $a0
	
	# Print out what's in $a0
	li $v0, 1
	syscall
	
	# End
	li $v0, 10
	syscall