  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.text

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

.global time2string
.global delay

hexasc:
	li	$t0, 0x30	

	andi	$t1, $a0, 0x0000000F	#apply bitmask. Could modify $a0 directly but not sure if that's allowed. $t1 is our "fixed" argument, our character offset

	ble	$t1, 9, hasFixedLetter	#if $t1 is less than or equal to 9 we skip doing letter specific stuff since it's not a letter 
	nop
	li	$t0, 0x41		#if the code ever gets here it means $t1 is a letter, we will update $t0 to be the "start point" for letters
	sub	$t1, $t1, 10		#then we remove 10 from the letter index since the value 10 should be the first letter, 11 should be the second and so on
	
hasFixedLetter:
	add	$t0, $t1, $t0		#now the "start point" for characters should be right no matter what is going to be printed, so we add the offset to it
	
	move	$v0, $t0		#put the value of $t0 in the return value register
	jr	$ra			#return
	nop				#I've tried having move $v0, $t0 here but it doesn't seem to work

delay:
	move	$t0, $a0 			# this is "i"

while:
	ble	$t0, 0, done			# if $t0 is <= 0 we go to done

	addi	$t0, $t0, -1			# remove 1 from $t0

	li	$t1, 6675 # this is the constant that needs to be changed!!
	li	$t2, 0				# this is "i"
innerLoop:
	addi	$t2, $t2, 1
	blt	$t2, $t1, innerLoop

	j while
done:
	jr	$ra
	nop
	
time2string:	
	andi	$t1, $a1, 0x0000F000	# store 4 MSB in $t1
	srl	$t1, $t1, 12
	
	PUSH	$ra			#save ra
	PUSH	$a1			# save $a1
	PUSH	$a0			# save $a0
	
	move	$a0, $t1		# set argument for hexasc to $t1
	jal	hexasc			# call hexasc
	nop
	
	POP	$a0			# read $a0
	POP	$a1			# read $a1
	
	sb	$v0, ($a0)		# store value from hexasc at address $a0

	andi	$t1, $a1, 0x00000F00	# store 4 next MSB in $t2
	srl	$t1, $t1, 8
	
	PUSH	$a1			# re-store $a1
	PUSH	$a0			# re-store $a0
	
	move	$a0, $t1		# set argument for hexasc to $t1
	jal	hexasc			# call hexasc
	nop

	POP	$a0			# read $a0
	POP	$a1			# read $a1
	
	sb	$v0, 1($a0)		# store value from hexasc at address $a0
	
	li	$t0, 0x3A
	sb	$t0, 2($a0)
	
	andi	$t1, $a1, 0x000000F0	# store 4 LSB in $t1
	srl	$t1, $t1, 4
	
	PUSH	$a1			# re-store $a1
	PUSH	$a0			# re-store $a0
	
	move	$a0, $t1		# set argument for hexasc to $t1
	jal	hexasc			# call hexasc
	nop

	POP	$a0			# read $a0
	POP	$a1			# read $a1
	
	sb	$v0, 3($a0)		# store value from hexasc at address $a0
	
	andi	$t1, $a1, 0x0000000F	# store 4 LSB in $t1
	srl	$t1, $t1, 0
	
	PUSH	$a1			# re-store $a1
	PUSH	$a0			# re-store $a0
	
	move	$a0, $t1		# set argument for hexasc to $t1
	jal	hexasc			# call hexasc
	nop

	POP	$a0			# read $a0
	POP	$a1			# read $a1
	POP 	$ra			# read $ra
	
	sb	$v0, 4($a0)		# store value from hexasc at address $a0
	
	li	$t3, 5
	li	$t1, 0
	li	$t2, 0
	andi	$t1, $a1, 0x0000000F
	andi	$t2, $a1, 0x000000F0
	srl	$t2, $t2, 4
	add	$t1, $t1, $t2
	
	bne	$t1, 0, nullByte
	nop
	
	li	$t0, 0x58
	sb	$t0, 5($a0)
	li	$t3, 6
	
nullByte:
	li	$t0, 0x00		# load null byte
	
	li	$t4, 0
	add	$t4, $t3, $a0
	
	sb	$t0, ($t4)		# write null byte
	
	jr	$ra
	nop

.global enable_interrupt
enable_interrupt:
	
	ei 

	jr $ra
	nop
	