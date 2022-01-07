	out $zero, $zero, $imm1, $imm2, 1, 1 		# enable irq1
	add $a0, $zero, $zero, $imm2, 0, 7			# sector number = 7
	add $a1, $zero, $zero, $zero, 0, 0			# set return flag = 0
	out $zero, $imm1, $zero, $imm2, 6, RETURN	# set irqhandler as return
READ:
	out $zero, $zero, $imm1, $imm2, 16, 0 		# diskbuffer = 0
	out $zero, $zero, $imm2, $a0, 0, 15			# write sector number
	out $zero, $zero, $imm1, $imm2, 14, 1		# read disk sector
WAIT1:
	beq $zero, $a1, $imm1, $imm2, 0, WAIT1		# wait for interrupt to end
	add $a1, $zero, $zero, $zero, 0, 0			# set return flag = 0
WRITE:
	add $a0, $a0, $zero, $imm2, 0, 1			# write to the next sector
	out $zero, $zero, $imm2, $a0, 0, 15			# write sector number
	out $zero, $zero, $imm1, $imm2, 16, 0 		# diskbuffer = 0
	out $zero, $zero, $imm1, $imm2, 14, 2		# write disk sector
WAIT2:
	beq $zero, $a1, $imm1, $imm2, 0, WAIT2		# wait for interrupt to end
	add $a1, $zero, $zero, $zero, 0, 0			# set return flag = 0
	beq $zero, $imm1, $a0, $imm2, 1, END		# if finished writing to sector 1
	sub $a0, $a0, $zero, $imm2, 0, 2			# next sector to read is 2 sectors behind
	beq $zero, $zero, $zero, $imm2, 0, READ		# jump back to read ater interrupt
RETURN:
	add $a1, $zero, $zero, $imm2, 0, 1			# flag = 1
	out $zero, $zero, $imm2, $zero, 0, 4		# clear irq1 status
	reti $zero, $zero, $zero, $zero, 0, 0		# return from interrupt
END:
	halt $zero, $zero, $zero, $zero, 0, 0		# halt
