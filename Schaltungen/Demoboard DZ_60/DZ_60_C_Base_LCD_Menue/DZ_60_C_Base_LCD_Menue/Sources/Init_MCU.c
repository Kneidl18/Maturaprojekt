#include "MC9S08DZ60.h"

void InitMCU(void) {


               //    SOPT1: COPT=0,STOPE=0,SCI2PS=0,IICPS=0 
                asm clra
                asm sta     SOPT1
                                                               
                // SOPT2: COPCLKS=0,COPW=0,ADHTS=0,MCSEL=0 
                asm clra
                asm sta     SOPT2                                               
                
                // SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 
                asm lda     #$1C
                asm sta     SPMSC1                                               
                
                // SPMSC2: LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 
                asm clra
                asm sta     SPMSC2                                               
                
                // System clock initialization 
                // Test if the device trim value is stored on the specified address
                asm lda     $FFAF
                asm cbeqa   #$FF,SkipTrim
                
                // MCGTRM: Initialize internal clock trim from a non volatile memory 
                asm lda     $FFAF
                asm sta     MCGTRM
                
                // MCGSC: Initialize internal clock trim from a non volatile memory 
                asm lda     $FFAE
                asm sta     MCGSC
 asm SkipTrim:
                // MCGC2: BDIV=2,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 
                asm mov     #$B6,MCGC2             // Set MCGC2 register 
                // MCGC1: CLKS=0,RDIV=7,IREFS=0,IRCLKEN=0,IREFSTEN=0 
                asm mov     #$38,MCGC1             // Set MCGC1 register 
                // MCGC3: LOLIE=0,PLLS=0,CME=0,VDIV=1 
                asm mov     #$01,MCGC3             // Set MCGC3 register 
 asm while_Cpu0:                            // Wait until external reference is stable 
                asm brclr   1,MCGSC,while_Cpu0
 asm whileSet_Cpu0:                         // Wait until external reference is selected 
                asm brset   4,MCGSC,whileSet_Cpu0
 asm while_Cpu1:                            // Wait until FLL is locked 
                asm brclr   6,MCGSC,while_Cpu1
 asm whileVal_Cpu0:                         // Wait until FLL clock is selected as a bus clock reference 
                asm lda     MCGSC
                asm and     #$0C
                asm bne     whileVal_Cpu0
  }


