#include "MC9S08LL16.h"

void InitMCU(void)
{

			//InitMCU of the LL16
			

	SOPT1 = 0b01000011;
//			  ||||||||______	RSTPE			
//			  |||||||_______	BKGDPE
//			  ||||||________	)
//			  |||||_________	> reserved
//			  ||||__________	)
//			  |||___________	STOPE
//			  ||____________	COPT
//			  |_____________	COPE

	SOPT2 = 0b00000100;
//			  ||||||||______	ACIC
//			  |||||||_______	IICPS
//			  ||||||________	SPIPS
//			  |||||_________	)
//			  ||||__________	> reserved
//			  |||___________	)
//			  ||____________	reserved
//			  |_____________	COPCLKS

	SPMSC1 =0b00011111;
//			  ||||||||______	BGBE	
//			  |||||||_______	reserved
//			  ||||||________	LVDE2
//			  |||||_________	LVDSE
//			  ||||__________	LVDRE2
//			  |||___________	LVWIE
//			  ||____________	LVWACK
//			  |_____________	LVWF

	SPMSC2 =0b00100000;
//			  ||||||||______	LPR	
//			  |||||||_______	LPRS
//			  ||||||________	LPWUI
//			  |||||_________	reserved
//			  ||||__________	PPDF
//			  |||___________	PPDACK
//			  ||____________	PPDE
//			  |_____________	PPDC

	SPMSC3 =0b00001000;
//			  ||||||||______	LVWF
//			  |||||||_______	LVWACK
//			  ||||||________	reserved
//			  |||||_________	reserved
//			  ||||__________	LVWIE
//			  |||___________	)
//			  ||____________	> reserved
//			  |_____________	)

			
	ICSC1 = 0b00010010;
//			  ||||||||______	IREFSTEN: Internal Reference Stop Enable: when ics in stop: 1=IRCLKEN active if active before, 0=stopped
//			  |||||||_______	IRCLKEN: Internal Reference Clock Enable: (1=active, 0=inactive)
//			  ||||||________	IREFS: Internal Reference Select: selects the reference clock source for FLL (1=internal, 0=external)
//			  |||||_________	)
//			  ||||__________	> RDIV: Reference Divider: divides eternel reference Clock, result must be in range 31.25kHz to 39.0625kHz
//			  |||___________	)	
//			  ||____________	)
//			  |					> CLKS: Clock source select (00=FLL output, 01=Internal refecence Clock, 10=External reference Clock)
//			  |_____________	)	
			
	
	ICSC2 = 0b10110110;
//			  ||||||||______	EREFSTEN: 1=external clock DOESN"T stop in stop mode, 0= external clock disabled in stop
//			  |||||||_______	ERCLKEN: External Refernece Enable: 1=ICSERCLK active, 0=ICSERCLK inactive
//			  ||||||________	EREFS: External Reference Select: 1=Oscillator requested, 0=External Clock Sourece requested
//			  |||||_________	LP: Low Power Select: 1=FLL disabled if not in BDM, 0=FLL is not diabled in bypass
//			  ||||__________	HGO: Hihc Gain Oscillator Select: 1=external oscillator for high gain operation, 0=external oscillator for low power operation)
//			  |||___________	RANGE: Frequency Range Select: selects the frequency range for the external oscillator (1=high-, 0=low-range)
//			  ||____________	)
//			  |					> BDIV: Bus Frequency Divider (00=clock/1, 01=clock/2, 10=clock/4, 11=clock/8)
//			  |_____________	)
			  
						  
	ICSSC = 0b00000001;
//			  ||||||||______	FTRIM: finetrim for the internal reference clock frequency: 1=increse the period, 0=decreasing the period
//			  |||||||_______	reserved
//			  ||||||________	)
//			  |||||_________	> reserved
//			  ||||__________	)
//			  |||___________	DMX32: narrows the frequency with 32.768kHz reference (0=default range of 25%, 1=finetuned for max freq. of 32.768kHz)
//			  ||____________	)
//			  |					> DRST: current frequency range for FLL output: (00=Low Range, 01=Mid range, 10=High range)
//			  |_____________	)

	SCGC1 = 0b11111111;		//enable bus clock for TPM1/2, ADC, IIC, SPI
	SCGC2 = 0b11100000;
	
  	LCDC0 = 0b00000000;
  	LCDC1 = 0b00000100;
  	LCDSUPPLY = 0b00110111;
  	LCDRVC = 0b00000000;
}
