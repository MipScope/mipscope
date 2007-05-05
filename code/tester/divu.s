
.text
main:
	li $v0, 1
	li $s0, 0x40000000 #PROBLEM
	li $s1, 4
	mult $s0, $s1
	mflo $a0
	syscall
	mfhi $a0
	syscall