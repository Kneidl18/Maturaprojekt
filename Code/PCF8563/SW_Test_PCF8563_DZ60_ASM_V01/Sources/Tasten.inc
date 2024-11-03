PRELLZEIT 		EQU	5T

TASTE_1_ON		EQU	0
TASTE_2_ON		EQU	0
TASTE_3_ON		EQU 0
TASTE_4_ON		EQU	0

RUN_TASTE	EQU	0
ROUTINE1_TASTE 	EQU	1
ROUTINE2_TASTE	EQU	2
ROUTINE3_TASTE	EQU	3	

RUN_TASTE_PORT	EQU	PTDD
RUN_TASTE_DIR	EQU	PTDDD



Tasten
			brset	RUN_TASTE, RUN_TASTE_PORT, Taste_1_Set

Taste_1_Off	
			lda		#0
			sta		PrellCounter_1
			
			bclr	TASTE_1_ON, Taste_1_Flag
			
			jmp  	Taste_2	

Taste_1_Set
			lda		PrellCounter_1
			add		#1
			sta		PrellCounter_1
			
			cmpa	#PRELLZEIT
			bne		Taste_2
			
			bset	TASTE_1_ON, Taste_1_Flag
			
			
Taste_2			
			brset	ROUTINE1_TASTE, RUN_TASTE_PORT, Taste_2_Set
			
Taste_2_Off	
			lda		#0
			sta		PrellCounter_2
			
			bclr	TASTE_2_ON, Taste_2_Flag
			
			jmp 	Taste_3		

Taste_2_Set
			lda		PrellCounter_2
			add		#1
			sta		PrellCounter_2
			
			cmpa	#PRELLZEIT
			bne		Taste_3
			
			bset	TASTE_2_ON, Taste_2_Flag


Taste_3	
			brset	ROUTINE2_TASTE, RUN_TASTE_PORT, Taste_3_Set
			
Taste_3_Off	
			lda		#0
			sta		PrellCounter_3
			
			bclr	TASTE_3_ON, Taste_3_Flag
			
			jmp 	Taste_4		

Taste_3_Set
			lda		PrellCounter_3
			add		#1
			sta		PrellCounter_3
			
			cmpa	#PRELLZEIT
			bne		Taste_4
			
			bset	TASTE_3_ON, Taste_3_Flag
			
Taste_4
			brset	ROUTINE3_TASTE, RUN_TASTE_PORT, Taste_4_Set
			
Taste_4_Off	
			lda		#0
			sta		PrellCounter_4
			
			bclr	TASTE_4_ON, Taste_4_Flag
			
			jmp 	Exit_Tasten		

Taste_4_Set
			lda		PrellCounter_4
			add		#1
			sta		PrellCounter_4
			
			cmpa	#PRELLZEIT
			bne		Exit_Tasten
			
			bset	TASTE_4_ON, Taste_4_Flag

Exit_Tasten
			rts
		

Init_Tasten
			bclr	RUN_TASTE, 	RUN_TASTE_DIR
			bclr	ROUTINE1_TASTE, RUN_TASTE_DIR
			bclr	ROUTINE2_TASTE, RUN_TASTE_DIR
			bclr	ROUTINE3_TASTE, RUN_TASTE_DIR
			
			bclr	TASTE_1_ON, Taste_1_Flag
			bclr	TASTE_2_ON, Taste_2_Flag
			bclr	TASTE_3_ON, Taste_3_Flag
			bclr	TASTE_4_ON, Taste_4_Flag	
			
			rts
			