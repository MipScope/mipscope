# Tim O'Donnell (todonne) S2I
# Takes an ASCII-encoded int-string stored at thestring.
# Converts it into an int, stores it at theint.
# Outputs the string and its integer value.
# Outputs an error message if a non-integer value is found in the string.
#
# Register usage:
# Constants are stored in the registers, starting at the last register (eg $s7).
# Variables are stored in the registers, starting at the first register (eg $s0).
# The only procedure calls are at the very beginning and very end of the program,
#	so there is effectively no difference between the $t and $s registers.
#
#	$t0	int		the int value of the current char
#	$t1	int		the int value of the current char - 9
#	$s0	char*		the memory address of the current character
#	$s1	int		our converted integer
#	$s2	char* / int	our current character
#	$s3	int	1	constant used to increment the $s0 pointer.
#	$s4	int	10	constant used in multiplying
#	$s5	int	57	constant used in subtraction
#	$s6	int	48	constant used in subtraction
#	$s7	int	0	constant used in comparisons
#	

	.data
	
thestring:	.asciiz	"0123456789"			# the string to convert
theint:		.word	0			# int val of the string
errormsg:	.asciiz	"\nInvalid character #: "	# an error message
successmsg:	.asciiz "\nSuccess. Value: "	# a success message

	.text
	
__start:

	# Print thestring:		
	li $v0, 4        	# system call code for print_str
	la $a0, thestring	# address of thestring to print
	syscall         	# print_str(thestring)

	la	$s0, thestring	# char* $s0 = &thestring;
	li	$s1, 0		# int $s1 will hold our converted integer.
	li	$s7, 0		# int $s7 = NULL  (this is C, not java)
	li	$s6, 48		# int $s6 = 48 (ASCII - 48 = INTEGER value)
	li	$s5, 57		# int $s5 = 57 (ASCII - 57 = INTEGER value - 9)
				# a given byte $s2 is a valid ASCII integer iff:
				#	$s2 - $s6 >= 0 && $s3 - $s5 <= 0
	li	$s4, 10		# int $s4 = 10 (the base we're using)
	li	$s3, 1		# int $s3 = 1
	
loop:						# while (*$s0 != NULL) {
						#  |<-- IMPLEMENTED BY:
	lb	$s2, ($s0)			#  |<--	char $s3 = (* $s0)
	beq	$s2, $s7, end_of_string		#  |<-- branch if *$s0 == 0
	
	sub	$t0, $s2, $s6			# 	int $t0 = char $s3 - 48
						# 	$t0 holds the int value of the current char

	sub	$t1, $s2, $s5			#	int $t1 = char $s3 - 57
						#	$t1 holds the int value - 9

						# 	$s2 is a valid ASCII integer iff:
						#	$t0 >= 0 && $t1 <= 0
						
	bltz	$t0, error			#	if ($t0 >= 0 && $t1 <= 0) {
	bgtz	$t1, error

	mul	$s1, $s1, $s4			#		$s1 = ($s1 * 10) + $t0;
	add	$s1, $s1, $t0			

	j	rest 				#	} // endif
						
error:						#	else { // we have an error
	# If we're here, then $s2 contains an invalid character.

	# Print errormsg:		
	li $v0, 4        					# system call code for print_str
	la $a0, errormsg					# address of string to print
	syscall         					# print_str(errormsg)

	# Print the ASCII value of $s2 as an integer:
	li $v0, 1        					# system call code for print_int
	move $a0, $s2						# integer to print
	syscall         					# print_int($s2)

	j end							# break; // jump out of the while.

rest:						#	} // end else						
	add	$s0, $s0, $s3			# $s0++
	j	loop				# } // endwhile

end_of_string:

	# If we're here, then $s1 is the integer.

	# success
	sw $s1, theint		# store $s1, the int, into memory

	
	# Print successmsg:		
	li $v0, 4        	# system call code for print_str
	la $a0, successmsg	# address of string to print
	syscall         	# print_str(successmsg)
	
	# Print the $s1 as an integer:
	li $v0, 1        	# system call code for print_int
	move $a0, $s1		# integer to print
	syscall         	# print_int($s1)

	j end			# done	

end:
	# woot	
