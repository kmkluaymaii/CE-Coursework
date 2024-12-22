############################################################################
# 
#                       EC413
#
#    		Assembly Language Lab -- Programming with Loops.
#
############################################################################
#  DATA
############################################################################
        .data           # Data segment
Hello:  .asciiz " \nRawisara Chairat\n"  # declare a zero terminated string
AnInt:	.word	17      # a word initialized to 17
space:	.asciiz	" "	    # declare a zero terminate string
newline: .asciiz "\n"   # a new line
AnotherInt: .word 23	# another word, this time initialized to 23
WordAvg:   .word 0		#use this variable for part 4
ValidInt:  .word 0		#
ValidInt2: .word 0		#
lf:     .byte	10, 0	# string with carriage return and line feed
InLenW:	.word   4       # initialize to number of words in input1 and input2
InLenB:	.word   16      # initialize to number of bytes in input1 and input2
        .align  4       # pad to next 16 byte boundary (address % 16 == 0)
Input1_TAG: .ascii "Input1 starts on next line"
		.align	4
Input1:	.word	0x01020304,0x05060708
	    .word	0x090A0B0C,0x0D0E0F10
        .align  4
Input2_TAG: .ascii "Input2 starts on next line"
        .align  4
Input2: .word   0x01221117,0x090b1d1f   # input
        .word   0x0e1c2a08,0x06040210
        .align  4
Copy_TAG: .ascii "Copy starts on next line"
        .align  4
Copy:  	.space  128		# space to copy input word by word
        .align  4
Input3_TAG: .ascii "Input3 starts on next line"
        .align  4
Input3:	.space	400		# space for data to be transposed
Transpose_TAG: .ascii "Transpose starts on next line"
        .align  4
Transpose: .space 400	# space for transposed data
 
############################################################################
#  CODE
############################################################################
        .text                   # code segment
#
# print out greeting.  
# Task 2:  change the message so that it prints out your name.
main:
        la	$a0,Hello		# address of string Hello to print
        li	$v0,4			# system call code for print_str
        syscall				# print the string
#
# Print the integer value of AnInt
# Task 3: modify the code so that it prints out two integers
# separated by a space.
		lw	$a0,AnInt		# load I/O register with value of AnInt
		li	$v0,1			# system call code for print_int
		syscall				# print the integer

		la	$a0,space		# load I/O register with space
		li	$v0,4			# system call code for print_str
		syscall				# print the integer

		lw	$a0,AnotherInt		# load I/O register with value of AnotherInt
		li	$v0,1			# system call code for print_int
		syscall				# print the integer

#
# Print the integer values of each byte in the array Input1
# Task 4a: modify the code so that it prints spaces between the integers
		la	$s0,Input1		# load pointer to array Input1
		lw	$s1,InLenB		# get length of array Input1 in bytes

		la	$a0,newline		# load I/O register with newline
		li	$v0,4			# system call code for print_str
		syscall				# print the new line

task4:	
		ble	$s1,$zero,done4		# test if done
		lb	$a0,($s0)		# load next byte into I/O register
		li	$v0,1			# specify print integer
		syscall				# print the integer

		la	$a0,space		# load I/O register with space
		li	$v0,4			# system call code for print_str
		syscall				# print the space

		add	$s0,$s0,1		# point to next byte
		sub	$s1,$s1,1		# decrement index variable
		j	task4			# do it again
done4:
#
# Task 4b: copy the Task 4 code and paste here.  Modify the code to print
# the array backwards.
		la	$s0,Input1		# load pointer to array Input1
		lw	$s1,InLenB		# get length of array Input1 in bytes	

		la	$a0,newline		# load I/O register with newline
		li	$v0,4			# system call code for print_str
		syscall				# print the new line

		add	$s0,$s0,$s1		# point to the byte after the last byte
		sub	$s0,$s0,1		# point to the last byte

task4a:	
		ble	$s1,$zero,done4a	# test if done
		lb	$a0,($s0)		# load next byte into I/O register
		li	$v0,1			# specify print integer
		syscall				# print the integer

		la	$a0,space		# load I/O register with space
		li	$v0,4			# system call code for print_str
		syscall				# print the space

		sub	$s0,$s0,1		# point to byte before
		sub	$s1,$s1,1		# decrement index variable
		j	task4a			# do it again

done4a:

#
# Code for Task 5 -- copy the contents of Input2 to Copy
		la	$s0,Input2		# load pointer to array Input2
		la	$s1,Copy		# load pointer to array Copy
		lw	$s2,InLenB		# get length of array Input2 in bytes
task5:
		ble	$s2,$zero,done5		# test if done
		lb	$t0,($s0)		# get the next byte
		sb	$t0,($s1)		# put the byte
		add	$s0,$s0,1		# increment input pointer
		add	$s1,$s1,1		# increment output pointer
		sub	$s2,$s2,1		# decrement index variable
		j	task5			# continue
done5:
#
# Task 5:  copy the Task 5 code and paste here.  Modify the code to copy
# the data in words rather than bytes.
		la	$s0,Input2		# load pointer to array Input2
		la	$s1,Copy		# load pointer to array Copy
		lw	$s2,InLenB		# get length of array Input2 in bytes
task5b:
		ble	$s2,$zero,done5b	# test if done
		lw	$t0,($s0)		# get the word
		sw	$t0,($s1)		# store the word
		add	$s0,$s0,4		# increment input pointer
		add	$s1,$s1,4		# increment output pointer
		sub	$s2,$s2,4		# decrement index variable
		j	task5b			# continue
done5b:
#
# Code for Task 6 -- 
# Print the integer average of the contents of array Input2 (bytes)
		la	$s0,Input2		# load pointer to array Input2
		lw	$s2,InLenB		# get length of array Input2 in bytes
		li 	$s3,0 			# set sum to 0
		li 	$t1,0 			# set counter to 0
		
task6:
		beq	$t1,$s2,average		# test if the sum is done
		lb      $t0,($s0)             	# Load value from array at $s0 (Address of Input2)
		add     $s3,$s3,$t0           	# Add the value in the array to $s3 (sum)
		add     $s0,$s0,1		# increment array pointer 
		add	$t1,$t1,1		# increment counter variable
		j	task6			# continue the loop

average: 

		lw	$s2,InLenB		# get length of array Input2 in bytes
		beq	$s2,$zero,done6		# check that the divisor is not zero
		div 	$s3,$s2			# divide $s3 (sum) by $s2 (number in the array)
		mflo 	$t2			# move the result to $t2

		la	$a0,newline		# load I/O register with newline
		li	$v0,4			# system call code for print_str
		syscall				# print the new line

		move	$a0,$t2			# load I/O register with value from $t2
		li	$v0,1			# system call code for print_int
		syscall				# print the average

		j	done6

done6:
#
# Code for Task 7 --  
# Print the first 25 integers that are divisible by 7 (with spaces)
		li 	$s0,1 			# Initialize s0 to 1, the first number
		li	$s1,25			# Initialize the count to 25
		li 	$t1,7			# Initialize t1 to 7

		la	$a0,newline		# load I/O register with newline
		li	$v0,4			# system call code for print_str
		syscall				# print the new line

task7:
		beq	$s1,$zero,done7 	# test if done
		div 	$s0, $t1		# divide $s0 by $t1
		mfhi 	$t0			# store remainder in $t0
		bne	$t0,$zero,increment	# if remainder is not zero, go to increment

print:
		move	$a0,$s0			# load I/O register with value of $s0
		li	$v0,1			# system call code for print_int
		syscall				# print the integer
		
		la	$a0,space		# load I/O register with space
		li	$v0,4			# system call code for print_str
		syscall				# print the space

		sub     $s1,$s1,1           	# decrement the count 
		j	increment		# continue to increment the number
			
increment:
		add     $s0,$s0,1           	# increment the number 
		j	task7			# continue the loop to test the next number	

done7:
#
# The following code initializes Input3 for Task8
		la	$s0,Input3		# load pointer to Input3
		li	$s1,100			# load size of array in bytes
		li	$t0,3			# start with 3
init8a:
		ble	$s1,$zero,done8a	# test if done
		sb	$t0,($s0)		# write out another byte
		add	$s0,$s0,1		# point to next byte
		sub	$s1,$s1,1		# decrement index variable
		add	$t0,$t0,1		# increase value by 1
		j 	init8a			# continue
done8a:
#
# Code for Task 8 --
# Transpose the 10x10 byte array in Input3 into Transpose
		li 	$s3,10			# set array size to 10 = square root of $s1 
		li 	$t1,0			# set i = 0  
		li 	$t2,0			# set j = 0 
i_loop: 
		beq	$t1,$s3,done8		# test all if the loop is done
		li 	$t2,0			# reset j to 0

j_loop: 
		beq	$t2,$s3,next		# test if the row is all loop through
		mul	$t3,$t1,$s3		# i * array_size
		add 	$t3,$t3,$t2		# i * array_size + j
		lb	$t4,Input3($t3)		# get the word from Input3[I][j]

		mul	$t5,$t2,$s3		# j * array_size
		add 	$t5,$t5,$t1		# j * array_size + i
		sb	$t4,Transpose($t5)	# store the word into Input3[j][i]

		add	$t2,$t2,1		# increment j  	
		j	j_loop			# continue the j_loop

next: 
		add	$t1,$t1,1		# increment i 
		j	i_loop			# continue the i_loop
		
done8:
# ALL DONE!
Exit:
jr $ra


