# Tim O'Donnell (todonne) I2S
# Takes an integer in memory, outputs its ASCII encoding in radix-10.
#
# Takes an integer, writes it out to a buffer in memory in ASCII-encoded radix-10.
#
# Register usage
# Constants are stored in the registers, starting at the last register (eg $s7).
# t0	int		temp variable used in calculating the length of the input int.
# t1	int		temp variable, used to store the current digit
# t2	char		temp variable, ASCII-encoded current digit
# s0	int		stores our integer
# s1	int		the number of digits
# s2	int		memory address of our current position in final_buf
# s3	int		constant, memory address of the beginning of final_buf
# s4	int	48	constant, ASCII value = INT value + 48
# s5	int	0	constant
# s6	int	1	constant
# s7	int	10	constant, our base
#
		.data
theint:		.word	1234567			# the integer to convert
final_buf:	.byte	0:12			# buffer for 11 digits, init to 0.
successmsg:     .asciiz "\nSuccess. Value: "    # a success message

		.text
__start:

	lw	$s0, theint			# int $s0 = theint
	li	$s7, 10				# constant used in division
	li	$s6, 1				# cosntant used to increment / decrement
	li	$s5, 0				# constant, useful
	li	$s4, 48				# constant, ASCII value = INT value + 48
	la	$s3, final_buf			# constant, mem address of final_buf

	# Print $s0 as an integer:
        li	$v0, 1				# system call code for print_int
        move	$a0, $s0			# integer to print
        syscall					# print_int($s0)

	move	$t0, $s0			# $t0 = $s0
	li	$s1, 0				# $s1 = NUMDIGITS = 0

	# Find the length of the number:
do_count:					# do {
	div	$t0, $s7			# 	$t0 /= 10
	mflo	$t0				
	add	$s1, $s1, $s6			#	$s1++
	bne	$t0, $s5, do_count		# } while ($t0 != 0);

	# Ok, so $s1 is the length.
	# We now step backward in final_buf, starting at final_buf[$s1], which must be our terminating NULL.
	
	# $s2 is our index into final_buf.
	la	$s2, final_buf			# $s2 = &final_buf[$s1]
	add	$s2, $s2, $s1
	
	sb	$s5, ($s2)			# *$s2 = NULL // write our null byte
	
do_convert:					# do {
	sub	$s2, $s2, $s6			# 	$s2--

	div	$s0, $s7			# 	$t1 = $s0 % 10
	mflo	$s0				#	$s0 /= 10
	mfhi	$t1				#	t1 is our digit

	add	$t2, $t1, $s4			#	$t2 = $t1 + 48 // $t2 is our ASCII char

	sb	$t2, ($s2)			#	*$s2 = $t2

	bne	$s2, $s3, do_convert		# } while ($s2 != final_buf);


	# Print successmsg:
        li $v0, 4               # system call code for print_str
        la $a0, successmsg	# address of string to print
        syscall
	
	# Print final_buf:
        li $v0, 4               # system call code for print_str
        la $a0, final_buf	# address of string to print
        syscall	

