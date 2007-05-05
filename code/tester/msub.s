.data


.text
	
main:

	li $s0, 0
	li $s1, 0

	mtlo $s0
	mthi $s1
 
	
	li $s0, 0xFFFFFFFF
	li $s1, 1
	msub $s0, $s1
	
	mfhi $a0
	
	# Print out what's in $a0
	li $v0, 1
	syscall
	
	# End
	li $v0, 10
	syscall