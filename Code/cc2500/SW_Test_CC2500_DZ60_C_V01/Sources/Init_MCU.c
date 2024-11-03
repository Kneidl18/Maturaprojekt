#include "MC9S08DZ60.h"

void InitMCU(void)
{
	// InitMCU of the AW32
/*	
	SMCLK = 0;
	ICGFLTU = 0;
	ICGS2 = 0;
	ICGS1 = 0;
	ICGC2 = 1;
	ICGFLTL = 0x0C;
	ICGC1 = 0x7C;
	
	
	lda	#%01111100
	;	  ||||||||
	;	  ||||||||____ Fix 0
	;	  |||||||_____ Loss of Clock detection enabled (0)
	;	  ||||||______ Enable Osc in OFF_Mode (1)
	;	  |||||_______ Clock Mode 1:1 = FLL enabled, external Reference
	;	  ||||________ Clock Mode
	;	  |||_________ External reference 0=External Clock 1=Ext.Crystal
	;	  ||__________ Freq.Range 0=LOW Frequency 1=High Frequency
	;	  |___________ High Gain Osc Select 0=Low Power 1=High Gain
	
	sta	ICGC1
	*/


	//InitMCU of the DZ60
	
	// SOPT1: COPT=0,STOPE=0,SCI2PS=0,IICPS=0 
	SOPT1 = 0;	
        //CLRA
        //STA     SOPT1                                               
        // SOPT2: COPCLKS=0,COPW=0,ADHTS=0,MCSEL=0 
        SOPT2 = 0;
        //CLRA
        //STA     SOPT2                                               
        // SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 
        SPMSC1 = 0x1C;
        //LDA     #$1C
        //STA     SPMSC1                                               
        // SPMSC2: LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 
        SPMSC2 = 0;
        //CLRA
        //STA     SPMSC2                                               
  //  System clock initialization 
        //Test if the device trim value is stored on the specified address
        
        _asm LDA     $FFAF;
        _asm CBEQA   #$FF, SkipTrim;
        // MCGTRM: Initialize internal clock trim from a non volatile memory 
        MCGTRM = 0xFFAF;
        //LDA     $FFAF
        //STA     MCGTRM
        // MCGSC: Initialize internal clock trim from a non volatile memory 
        MCGSC = 0xFFAE;
        //LDA     $FFAE
        //STA     MCGSC
_asm SkipTrim:
        // MCGC2: BDIV=2,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 
        _asm MOV     #$B6,MCGC2;             // Set MCGC2 register 
        // MCGC1: CLKS=0,RDIV=7,IREFS=0,IRCLKEN=0,IREFSTEN=0 
        _asm MOV     #$38,MCGC1;             // Set MCGC1 register 
        // MCGC3: LOLIE=0,PLLS=0,CME=0,VDIV=1 
        _asm MOV     #$01,MCGC3;             // Set MCGC3 register 
_asm while_Cpu0:                            // Wait until external reference is stable 
        _asm BRCLR   1,MCGSC,while_Cpu0;
_asm whileSet_Cpu0:                         // Wait until external reference is selected 
        _asm BRSET   4,MCGSC,whileSet_Cpu0;
_asm while_Cpu1:                            // Wait until FLL is locked 
        _asm BRCLR   6,MCGSC,while_Cpu1;
_asm whileVal_Cpu0:                         // Wait until FLL clock is selected as a bus clock reference 
        _asm LDA     MCGSC;
        _asm AND     #$0C;
        _asm BNE     whileVal_Cpu0;
}
