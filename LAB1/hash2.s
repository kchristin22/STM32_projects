.section .text
.global hash2
.type hash2, %function


hash2:
	.fnstart
		mov R1, #0 //initialize the sum of digits to 0
		mov R2, #10 //divisor = 10
		
		// R0 = num
		// R2 = divisor
	loopH:
		cmp R0, #0 //R0: contains the hash_value/input
		beq sum_done //if num == 0 exit loopH
		
		//do div, if number > 0
		//div

		sdiv R4, R0, R2 //R4 = quotient, R0 = dividend, R2 = 10
		mul R3, R4, R2 // R3 = q * 10
		sub R5, R0, R3  // R5 = R0 - q*10 = remainder
		add R1, R1, R5 // total = total + digit, digit = remainder
		mov R0, R4 // R0 = q
		
		b loopH
				
	sum_done:
		cmp R1, #9
		IT gt
		movgt R0, R1 //if total number > 9, then R0 = R1:number to find sum of digits
		bgt hash2 //and repeat hash2
		
		mov R2, #1 //initialize the value of factorial result
		
		cmp R1, #0 
		IT eq
		moveq R2, #1 // if total == 0, then factorial = 1
		beq exit
		
		factorial:
			cmp R1, #1
			beq exit //R1 == 1 exit
			mul R2, R1, R2 
			sub R1, R1, #1
			b factorial
		
		exit:
			mov R0, R2 //save the total_result to R0 and return
			bx lr
		
	.fnend
	
/*		mov R4, R0 // R4 = dividend 
		mov R3, #0 // Q=0
		loop:
			cmp R4, R2
			blt done_div //if dividend < divisor stop division
			sub R4, R4, R2 // dividend = dividend - divisor
			add R3, R3, #1 // quotient = quotient + 1
			b loop
			
			done_div:
				add R1, R1, R4 //total = total + digit, digit = remainder
				mov R0, R3 // num = num /10 = q
*/