.data


.text
	
main:
	
	li $s0, -0xFFFF
	srl $a0, $s0, 1
	
	# Print out what's in $a0
	li $v0, 1
	syscall
		
	# End
	li $v0, 10
	syscall
	