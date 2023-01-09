#include "Variablen.h"
#include "MC9S08LL16.h"
#include "defines.h"


/****************************************************************************************/
/*                                                                                      */
/*  Realtime-Interrupt und dazugehörige Initialisierung                                 */
/*  Ich stelle den Interrupt auf                                                        */
/*  Bei 4,9152 MHz-Quarz und FLL= 4 ist der Busclock 1/4,9152 MHz = 203,451 nsec        */
/*                                                                                      */                         
/*      Bei $6000 Zyklen (Dezimal: 49.152 Zyklen) á 203,451 nsec = 10 msec              */
/*      $6000 ergibt sich aus 8192 vom Prescaler (im SRTISC-Register) und dem           */
/*      RealTimeCounter, der bis 6 zählt. 6 x 8192 = 49.152 Zyklen                      */
/*                                                                                      */
/****************************************************************************************/


__interrupt void _Realtime (void) {
//    SRTISC = 0x75;
    
/*          SRTISC       #%01110101             ; Dividiert durch 8192 x 203,451... nsec = 1,6666 msec                                          */
/*                         ||||||||                                                                                                             */
/*                         ||||||||_____________  Bit 0 - Bit 2: Divisionsrate 0:0:0 = Disable  0:0:1 = Div 256                                 */
/*                         |||||||______________  0:1:0 = Div 1024   0:1:1 = Div 2048   1:0:0 = Div 4096                                        */
/*                         ||||||_______________  1:0:1 = Div 8192   1:1:0 = Div 16384  1:1:1 = Div 32768                                       */
/*                         |||||________________  Fix auf 0 (ungebraucht)                                                                       */
/*                         ||||_________________  RTIE : Realtime Interrupt enable 1=Yes   0=No                                                 */
/*                         |||__________________  RTICLKS : Realtime Clock Select: 0=intern 1kHz  1=Ext. Clock (Quarz)                          */
/*                         ||___________________  RTIACK: Acknowledge Flag: Muss vor neuem Int. gelöscht werden! "1" schreiben!                 */
/*                         |____________________  RTIF: Zeigt an, ob es einen RealTime gab - nur zum Auslesen!                                  */
       

    
    
    RealTimeCounter++;
    if (RealTimeCounter == 6) {
    	RealTimeFlag=1;
    	RealTimeCounter=0;
    }
}


void InitRealtime (void) {
//        SRTISC = 0x75;
        RealTimeFlag=0;
        RealTimeCounter=0;
}



/****************************************************************************************/
/*                                                                                      */
/*  Dummy Interrupt-Routine: braucht es, um ggf. falsch auftretende (und nicht gewollte)*/
/*  Interrupts abzufangen!                                                              */
/*                                                                                      */
/****************************************************************************************/


__interrupt void _Dummy (void){
        Nop();
}


/*** ICSC1 - ICS Control Register 1; 0x00000038 ***/
//*typedef union {
/*  byte Byte;
  struct {
    byte IREFSTEN    :1;                                       /* Internal Reference Stop Enable *
    byte IRCLKEN     :1;                                       /* Internal Reference Clock Enable *
    byte IREFS       :1;                                       /* Internal Reference Select *
    byte RDIV0       :1;                                       /* Reference Divider, bit 0 *
    byte RDIV1       :1;                                       /* Reference Divider, bit 1 *
    byte RDIV2       :1;                                       /* Reference Divider, bit 2 *
    byte CLKS0       :1;                                       /* Clock Source Select, bit 0 *
    byte CLKS1       :1;                                       /* Clock Source Select, bit 1 *
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpRDIV :3;
    byte grpCLKS :2;
  } MergedBits;
} ICSC1STR;
extern volatile ICSC1STR _ICSC1 @0x00000038;
#define ICSC1                           _ICSC1.Byte
#define ICSC1_IREFSTEN                  _ICSC1.Bits.IREFSTEN
#define ICSC1_IRCLKEN                   _ICSC1.Bits.IRCLKEN
#define ICSC1_IREFS                     _ICSC1.Bits.IREFS
#define ICSC1_RDIV0                     _ICSC1.Bits.RDIV0
#define ICSC1_RDIV1                     _ICSC1.Bits.RDIV1
#define ICSC1_RDIV2                     _ICSC1.Bits.RDIV2
#define ICSC1_CLKS0                     _ICSC1.Bits.CLKS0
#define ICSC1_CLKS1                     _ICSC1.Bits.CLKS1
#define ICSC1_RDIV                      _ICSC1.MergedBits.grpRDIV
#define ICSC1_CLKS                      _ICSC1.MergedBits.grpCLKS

#define ICSC1_IREFSTEN_MASK             1
#define ICSC1_IRCLKEN_MASK              2
#define ICSC1_IREFS_MASK                4
#define ICSC1_RDIV0_MASK                8
#define ICSC1_RDIV1_MASK                16
#define ICSC1_RDIV2_MASK                32
#define ICSC1_CLKS0_MASK                64
#define ICSC1_CLKS1_MASK                128
#define ICSC1_RDIV_MASK                 56
#define ICSC1_RDIV_BITNUM               3
#define ICSC1_CLKS_MASK                 192
#define ICSC1_CLKS_BITNUM               6


/*** ICSC2 - ICS Control Register 2; 0x00000039 ***
typedef union {
  byte Byte;
  struct {
    byte EREFSTEN    :1;                                       /* External Reference Stop Enable *
    byte ERCLKEN     :1;                                       /* External Reference Enable *
    byte EREFS       :1;                                       /* External Reference Select *
    byte LP          :1;                                       /* Low Power Select *
    byte HGO         :1;                                       /* High Gain Oscillator Select *
    byte RANGE       :1;                                       /* Frequency Range Select *
    byte BDIV0       :1;                                       /* Bus Frequency Divider, bit 0 *
    byte BDIV1       :1;                                       /* Bus Frequency Divider, bit 1 *
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpBDIV :2;
  } MergedBits;
} ICSC2STR;
extern volatile ICSC2STR _ICSC2 @0x00000039;
#define ICSC2                           _ICSC2.Byte
#define ICSC2_EREFSTEN                  _ICSC2.Bits.EREFSTEN
#define ICSC2_ERCLKEN                   _ICSC2.Bits.ERCLKEN
#define ICSC2_EREFS                     _ICSC2.Bits.EREFS
#define ICSC2_LP                        _ICSC2.Bits.LP
#define ICSC2_HGO                       _ICSC2.Bits.HGO
#define ICSC2_RANGE                     _ICSC2.Bits.RANGE
#define ICSC2_BDIV0                     _ICSC2.Bits.BDIV0
#define ICSC2_BDIV1                     _ICSC2.Bits.BDIV1
#define ICSC2_BDIV                      _ICSC2.MergedBits.grpBDIV

#define ICSC2_EREFSTEN_MASK             1
#define ICSC2_ERCLKEN_MASK              2
#define ICSC2_EREFS_MASK                4
#define ICSC2_LP_MASK                   8
#define ICSC2_HGO_MASK                  16
#define ICSC2_RANGE_MASK                32
#define ICSC2_BDIV0_MASK                64
#define ICSC2_BDIV1_MASK                128
#define ICSC2_BDIV_MASK                 192
#define ICSC2_BDIV_BITNUM               6


/*** ICSTRM - ICS Trim Register; 0x0000003A ***
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* ICS Trim Setting, bit 0 *
    byte TRIM1       :1;                                       /* ICS Trim Setting, bit 1 *
    byte TRIM2       :1;                                       /* ICS Trim Setting, bit 2 *
    byte TRIM3       :1;                                       /* ICS Trim Setting, bit 3 *
    byte TRIM4       :1;                                       /* ICS Trim Setting, bit 4 *
    byte TRIM5       :1;                                       /* ICS Trim Setting, bit 5 *
    byte TRIM6       :1;                                       /* ICS Trim Setting, bit 6 *
    byte TRIM7       :1;                                       /* ICS Trim Setting, bit 7 *
  } Bits;
} ICSTRMSTR;
extern volatile ICSTRMSTR _ICSTRM @0x0000003A;
#define ICSTRM                          _ICSTRM.Byte
#define ICSTRM_TRIM0                    _ICSTRM.Bits.TRIM0
#define ICSTRM_TRIM1                    _ICSTRM.Bits.TRIM1
#define ICSTRM_TRIM2                    _ICSTRM.Bits.TRIM2
#define ICSTRM_TRIM3                    _ICSTRM.Bits.TRIM3
#define ICSTRM_TRIM4                    _ICSTRM.Bits.TRIM4
#define ICSTRM_TRIM5                    _ICSTRM.Bits.TRIM5
#define ICSTRM_TRIM6                    _ICSTRM.Bits.TRIM6
#define ICSTRM_TRIM7                    _ICSTRM.Bits.TRIM7

#define ICSTRM_TRIM0_MASK               1
#define ICSTRM_TRIM1_MASK               2
#define ICSTRM_TRIM2_MASK               4
#define ICSTRM_TRIM3_MASK               8
#define ICSTRM_TRIM4_MASK               16
#define ICSTRM_TRIM5_MASK               32
#define ICSTRM_TRIM6_MASK               64
#define ICSTRM_TRIM7_MASK               128


/*** ICSSC - ICS Status and Control Register; 0x0000003B ***
typedef union {
  byte Byte;
  struct {
    byte FTRIM       :1;                                       /* ICS Fine Trim *
    byte OSCINIT     :1;                                       /* OSC Initialization *
    byte CLKST0      :1;                                       /* Clock Mode Status, bit 0 *
    byte CLKST1      :1;                                       /* Clock Mode Status, bit 1 *
    byte IREFST      :1;                                       /* Internal Reference Status *
    byte DMX32       :1;                                       /* DCO Maximum frequency with 32.768 kHz reference *
    byte DRST_DRS0   :1;                                       /* DCO Range Status/Range Select, bit 0 *
    byte DRST_DRS1   :1;                                       /* DCO Range Status/Range Select, bit 1
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpCLKST :2;
    byte         :1;
    byte grpDMX_32 :1;
    byte grpDRST_DRS :2;
  } MergedBits;
} ICSSCSTR;
extern volatile ICSSCSTR _ICSSC @0x0000003B;
#define ICSSC                           _ICSSC.Byte
#define ICSSC_FTRIM                     _ICSSC.Bits.FTRIM
#define ICSSC_OSCINIT                   _ICSSC.Bits.OSCINIT
#define ICSSC_CLKST0                    _ICSSC.Bits.CLKST0
#define ICSSC_CLKST1                    _ICSSC.Bits.CLKST1
#define ICSSC_IREFST                    _ICSSC.Bits.IREFST
#define ICSSC_DMX32                     _ICSSC.Bits.DMX32
#define ICSSC_DRST_DRS0                 _ICSSC.Bits.DRST_DRS0
#define ICSSC_DRST_DRS1                 _ICSSC.Bits.DRST_DRS1
#define ICSSC_CLKST                     _ICSSC.MergedBits.grpCLKST
#define ICSSC_DRST_DRS                  _ICSSC.MergedBits.grpDRST_DRS

#define ICSSC_FTRIM_MASK                1
#define ICSSC_OSCINIT_MASK              2
#define ICSSC_CLKST0_MASK               4
#define ICSSC_CLKST1_MASK               8
#define ICSSC_IREFST_MASK               16
#define ICSSC_DMX32_MASK                32
#define ICSSC_DRST_DRS0_MASK            64
#define ICSSC_DRST_DRS1_MASK            128
#define ICSSC_CLKST_MASK                12
#define ICSSC_CLKST_BITNUM              2
#define ICSSC_DRST_DRS_MASK             192
#define ICSSC_DRST_DRS_BITNUM           6

*/



