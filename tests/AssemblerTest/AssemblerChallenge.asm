
		sll $sp, $imm1, $imm2, $zero, 1, 11                 # set $sp = 1 << 11 = 2048 

		add $sp, $sp, $imm2, $zero, 0, -3  	                # adjust stack for 3 items


		sw $ra, $sp, $imm2, $zero, 0, 2			 	        # store value of ra in stack
		sw $s0, $sp, $imm2, $zero, 0, 1
		sw $s1, $sp, $imm2, $zero, 0, 0			            # store value of s1 in stack

		add $s0, $imm1, $zero, $zero, 7, 0					# s0 = 7

 for:	
        blt $zero, $s0, $zero, $imm2, 0, return             # if s0 < 0 jump to return
        blt $zero, $s0, $zero, $imm2, 0, return
        
       
 	 	out $zero	, $imm1 	,  $zero 	 ,  	$zero 	,   16  	  	, 	 	 	 0 		 	 				 		# 	 	 diskbuffer 	 	 = 0
		out $zero, $imm1, $zero, $s0, 15, 0					# disksector = s0
		out $zero, $imm1, $zero, $imm2, 14, 1				# diskcmd = 1
		jal $ra, $zero, $zero, $imm2, 0, wait				# wait 
	asd: out $zero	, $imm1 	,  $zero 	 ,  	$zero 	,   16  	  	, 	 	 	 0 		 	 				 		# 	 	 diskbuffer 	 	 = 0

out $zero	, $imm1 	,  $zero 	 ,  	$zero 	,   16  	  	, 	 	 	 0 		 	 				 		# 	 	 diskbuffer 	 	 = 0
 	 	 	  	 	add $s1, $s0, $imm1 ,$zero, 1, 0					# s1 = s0 + 1
		out $zero, $imm1, $zero, $zero, 16, 0				# diskbuffer = 0
		out $zero, $imm1, $zero, $s1, 15, 0					# disksector = s1
		out $zero, $imm1, $zero, $imm2, 14, 2				# diskcmd = 2 
		jal $ra, $zero, $zero, $imm2, 0, wait				# wait

		sub $s0, $s0, $imm1, $zero, 1, 0                    # s0--
		beq $zero, $zero, $zero, $imm2, 0, for				# jump to for

 	wait:														# while diskstatus == 1
		in $s1, $imm1, $zero, $zero, 17, 0					# s1 = diskstatus
		beq $zero, $s1, $imm1, $imm2, 1, wait				# if s1 == 1 jump to wait
		beq $zero, $s1, $imm1, $ra, 0, 0					# if s1 == 0 jump back

return:
        lw $s1, $sp, $imm2, $zero, 0, 0                     # restore $s1
        lw $s0, $sp, $imm2, $zero, 0, 1                     # restore $s0
		lw $ra, $sp, $imm2, $zero, 0, 2						# restore $ra
        add $sp, $sp, $imm2, $zero, 0, 3                    # pop 3 items from stack
		halt $zero, $zero, $zero, $zero, 0, 0               # halt