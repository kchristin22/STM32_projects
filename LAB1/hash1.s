.section .data
hash_table:
	.byte 10, 42, 12, 21, 7, 5, 67, 48, 69, 2, 36, 3, 19, 1, 14, 51, 71, 8, 26, 54, 75, 15, 6, 59, 13, 25
	

.section .text
.global hash1
.type hash1, %function
	
hash1:
	.fnstart
	mov R2, #0  //initialize hash_value = 0
	ldr R3, =hash_table //load the first adress of hash table in R3
	
	loop:
		ldrb R1, [R0], #1 //R1 = next char of string, R1 = R0 and R0 = R0 + 1
		cmp R1, #0 //check if end of string input reached 
		beq stop // if end then stop
		
		cmp R1, #65 
		blt not_letter //if R1 < A
		cmp R1, #90 
		bgt loop //if R1 > Z, isnt letter nor digit cause 90>57, go to next char
		
		//if letter
		sub R4, R1, #65 //find i of hashtable
		ldrb R5, [R3, R4] //find key in hash_table
		add R2, R2, R5 //hash = hash + key
		b loop
		
		not_letter:
			cmp R1, #49 
			blt loop //if R1 < 1 not number, b loop
			cmp R1, #57 
			bgt loop //if R1 > 9 not number, b loop
			
			//if number:
			sub R6, R1, #48 //convert ascii to decimal
			sub R2, R2, R6 // hash = hash - digit
			b loop	
						
	stop:
		mov R0, R2 //save hash to R0 and return
		bx lr
	
	.fnend
	