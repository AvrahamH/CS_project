	.word 0x100 75

	sll $sp, $imm1, $imm2, $zero, 1, 10			# set $sp = 1 << 10 = 1024
	lw $a0, $zero, $imm2, $zero, 0, 0x100		# get R from address 0x100
	mac $a0, $a0, $a0, $zero, 0, 0				# $a0 = R^2
	out $zero, $zero, $imm2, $imm1, 255, 21		# monitordata = 255
	add $t0, $imm2, $zero, $zero, 0, -1			# i = -1
	add $ra, $imm1, $zero, $zero, end, 0
for_i:
	add $t0, $t0, $imm1, $zero, 1, 0			# i=0, i++
	beq $zero, $t0, $imm2, $imm1, end, 256		# for (i<256)
	add $t1, $imm2, $zero, $zero, 0, -1			# j = -1
for_j:
	add $t1, $t1, $imm1, $zero, 1, 0			# j=0, j++
	beq $zero, $t1, $imm2, $imm1, for_i, 256	# for (j<256)
	sub $a1, $t0, $imm1, $zero, 128, 0			# a1 = y - y_center
	mac $a1, $a1, $a1, $zero, 0, 0				# a1 = a1^2 (y distance from center)
	sub $a2, $t1, $imm1, $zero, 128, 0			# a2 = x - x_center
	mac $a2, $a2, $a2, $zero, 0, 0				# a2 = a2^2 (x distance from center)
	add $a1, $a1, $a2, $zero, 0, 0				# a1 = (x - x_center)^2 + (y - y_center)^2
	bgt $zero, $a1, $a0, $imm1, for_j 			# if(a1 > R^2) return to beginning of loop
	mac $t2, $t0, $imm2, $t1, 0, 256			# buffer offset - i * 256 + j
	out $zero, $zero, $imm2, $t2, 0, 20			# monitoraddr = t2
	out $zero, $zero, $imm2, $imm1, 1, 22		# monitorcmd = 1
	beq $zero, $zero, $zero, $imm1, for_j, 0	# return to beginning of loop
end:
	halt $zero, $zero, $zero, $zero, 0, 0		# halt