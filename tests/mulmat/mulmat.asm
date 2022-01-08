
.word 0x100 -1
.word 0x101 2
.word 0x102 3  
.word 0x103 4  
.word 0x104 5
.word 0x105 6
.word 0x106 7
.word 0x107 8  
.word 0x108 9  
.word 0x109 10 
.word 0x10a 11
.word 0x10b 12
.word 0x10c 13  
.word 0x10d 14  
.word 0x10e 15  
.word 0x10f 16

.word 0x110 17
.word 0x111 18
.word 0x112 19
.word 0x113 20
.word 0x114 21
.word 0x115 22
.word 0x116 23
.word 0x117 24 
.word 0x118 25
.word 0x119 26
.word 0x11a 27
.word 0x11b 28 
.word 0x11c -29
.word 0x11d -30
.word 0x11e 31
.word 0x11f 32    

	add $sp, $imm1, $zero, $zero, 1, 0 			# $sp = 1
	sll $sp, $imm1, $imm2, $zero, 1, 2			# set $sp = 1 << 2 = 4
	add $a0, $imm1, $zero, $zero, 4, 0 			# a0 = 4
	add $s2, $imm1, $zero, $zero, 0x120, 0		# s2 = c11 adrress (write to) 
	add $t0, $imm1, $zero, $zero, -1, 0			# t0 = -1 [i]
for_i:
	add $t0, $t0, $imm1, $zero, 1, 0			# i=0 in the beginning | i++
	bge $zero, $t0, $a0, $imm1, end, 0			# for (i<4)
	add $t1, $zero, $zero, $zero,  0, 0			# j=0
for_j:		
	bge $zero, $t1, $a0, $imm1, for_i, 0		# for (j<4)
	add $a1, $zero, $imm1, $zero, 0, 0			# sum=0 INITIALIZE
	add $t2, $zero, $imm1, $zero, 0, 0			# k=0 INITIALIZE
	sll $s0, $t0, $imm1, $zero, 2, 0			# s0 = i*4 offset of address of line i
for_k:		
	bge $zero, $t2, $a0, $imm1, C_mat, 0		# finishes for loop if k == 4 
	sll $s1, $t2, $imm1, $zero, 2, 0			# s1=k*4
	add $sp, $sp, $imm1, $zero , 2, 0			# Saves 2 addresses before overwrite by lw
	sw $s0, $sp, $imm1, $zero, -1, 0
	lw $s0, $s0, $imm1, $zero, 0x100, 0			# loading of first term in the sum 0x100+4*i+k				 
	add $s1, $s1, $t1, $zero, 0, 0				# computes address second term in the sum [4*k+j]
	sw $s1, $sp, $imm1, $zero, -2, 0			
	lw $s1, $s1, $imm1, $zero, 0x110, 0			# loading of second term in the sum
	mac $s0, $s1, $s0, $zero, 0, 0				# mul of the two terms
	add $a1, $a1, $s0, $zero, 0, 0				# adds the products to previous calculations
	add $t2, $t2, $imm1, $zero, 1, 0			# k++
	lw $s1, $sp, $imm1, $zero, -2, 0			# address restore [Lack of registers 
	lw $s0, $sp, $imm1, $zero, -1, 0			# to restore both adresses and content in]
	add $sp, $sp, $imm1, $zero, -2, 0			
	add $s0, $s0, $imm1, $zero, 1, 0			# first term address increment [4i+k]
	beq $zero, $zero, $zero, $imm1, for_k, 0 	# jump to for_k as long as it hasn't been finished
C_mat:
	sw $a1, $s2, $imm1, $zero, 0, 0				# store product result in memory
	add $s2, $s2, $imm1, $zero, 1, 0			# write to next place
	add $t1, $t1, $imm1, $zero, 1, 0			# j++
	beq $zero, $zero, $zero, $imm1, for_j, 0	# jump to for_j
end:	
	halt $zero ,$zero, $zero, $zero, 0, 0