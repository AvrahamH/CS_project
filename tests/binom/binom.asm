	.word 0x100 10
	.word 0x101 8

	sll $sp, $imm1, $imm2, $zero, 1, 10			# set $sp = 1 << 10 = 1024
	lw $a0, $zero, $imm2, $zero, 0, 0x100		# get n from address 0x100
	lw $a1, $zero, $imm2, $zero, 0, 0x101  		# get k from address 0x101
	add $ra, $imm1, $zero, $zero, end, 0
binom:
	add $sp, $sp, $imm2, $zero, 0, -4  		# adjust stack for 4 items
	sw $zero, $sp, $imm2, $s0, 0, 3			# save $s0
	sw $zero, $sp, $imm2, $ra, 0, 2			# save return address
	sw $zero, $sp, $imm2, $a1, 0, 1			# save argument
	sw $zero, $sp, $imm2, $a0, 0, 0			# save argument
	beq $zero, $a1, $imm1, $imm2, 0, L2		# jump to L2 if k == 0 
	beq $zero, $a1, $a0, $imm2, 0, L2		# jump to L2 if k == n 
	
L1:
	sub $a0, $a0, $imm2, $zero, 0, 1		# calculate n - 1
	jal $ra, $zero, $zero, $imm2, 0, binom		# calc $v0=binom(n-1,k)
	add $s0, $v0, $imm2, $zero, 0, 0		# $s0 = binom(n-1,k)
	sub $a1, $a1, $imm2, $zero, 0, 1		# calculate k - 1
	jal $ra, $zero, $zero, $imm2, 0, binom	# calc binom(n-1,k-1)
	add $t0, $v0, $zero, $zero, 0, 0 		# $t0 = binom(n-1,k-1) 
	add $v0, $t0, $s0, $zero, 0, 0			# $v0 = binom(n-1,k) + binom(n-1,k-1)
	beq $zero, $zero, $zero, $imm1, L3, 0   #continue to the end of the func
L2:
	add $v0, $imm1, $zero, $zero, 1, 0		# otherwise, binom(n,k) = 1
L3:
	lw $a0, $sp, $imm2, $zero, 0, 0			# restore $a0
	lw $a1, $sp, $imm2, $zero, 0, 1			# restore $a1
	lw $ra, $sp, $imm2, $zero, 0, 2			# restore $ra
	lw $s0, $sp, $imm2, $zero, 0, 3			# restore $s0
	add $sp, $sp, $imm2, $zero, 0, 4		# pop 4 items from stack
	beq $zero, $zero, $zero, $ra, 0, 0		# and return
end:
	sw $zero, $zero, $imm2, $v0, 0, 0x102		# store binom(n,k) in 102
	halt $zero, $zero, $zero, $zero, 0, 0		# halt
