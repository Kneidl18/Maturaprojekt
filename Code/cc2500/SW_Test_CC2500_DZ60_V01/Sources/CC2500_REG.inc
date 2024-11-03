;/* Sync word qualifier mode = 30/32 sync word bits detected */
;/* CRC autoflush = false */
;/* Channel spacing = 199.951172 */
;/* Data format = Normal mode */
;/* Data rate = 2.39897 */
;/* RX filter BW = 203.125000 */
;/* Preamble count = 4 */
;/* Whitening = false */
;/* Address config = No address check */
;/* Carrier frequency = 2432.999908 */
;/* Device address = 0 */
;/* TX power = 0 */
;/* Manchester enable = false */
;/* CRC enable = true */
;/* Deviation = 38.085938 */
;/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
;/* Packet length = 255 */
;/* Modulation format = 2-FSK */
;/* Base frequency = 2432.999908 */
;/* Modulated = true */
;/* Channel number = 0 */
;/* PA table */
;#define PA_TABLE {0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
;/***************************************************************
; *  SmartRF Studio(tm) Export
; *
; *  Radio register settings specifed with C-code
; *  compatible #define statements.
; *
; *  RF device: CC2500
; *
; ***************************************************************/
 
 
REG_IOCFG2				EQU          $0000
REG_IOCFG1     			EQU		     $0001
REG_IOCFG0				EQU          $0002
REG_FIFOTHR 			EQU        	 $0003
REG_SYNC1       		EQU		     $0004
REG_SYNC0       		EQU		     $0005
REG_PKTLEN      		EQU    		 $0006
REG_PKTCTRL1    		EQU		     $0007
REG_PKTCTRL0    		EQU		     $0008
REG_ADDR        		EQU		     $0009
REG_CHANNR      		EQU		     $000A
REG_FSCTRL1     		EQU		     $000B
REG_FSCTRL0     		EQU		     $000C
REG_FREQ2       		EQU		     $000D
REG_FREQ1       		EQU		     $000E
REG_FREQ0       		EQU		     $000F
REG_MDMCFG4     		EQU		     $0010
REG_MDMCFG3     		EQU		     $0011
REG_MDMCFG2     		EQU		     $0012
REG_MDMCFG1     		EQU		     $0013
REG_MDMCFG0     		EQU		     $0014
REG_DEVIATN     		EQU		     $0015
REG_MCSM2       		EQU		     $0016
REG_MCSM1       		EQU		     $0017
REG_MCSM0       		EQU		     $0018
REG_FOCCFG      		EQU	         $0019
REG_BSCFG       		EQU		     $001A
REG_AGCCTRL2    		EQU		     $001B
REG_AGCCTRL1    		EQU		     $001C
REG_AGCCTRL0    		EQU		     $001D
REG_WOREVT1     		EQU		     $001E
REG_WOREVT0     		EQU		     $001F
REG_WORCTRL     		EQU		     $0020
REG_FREND1      		EQU		     $0021
REG_FREND0      		EQU		     $0022
REG_FSCAL3      		EQU		     $0023
REG_FSCAL2      		EQU		     $0024
REG_FSCAL1      		EQU		     $0025
REG_FSCAL0      		EQU		     $0026
REG_RCCTRL1     		EQU		     $0027
REG_RCCTRL0     		EQU		     $0028
REG_FSTEST      		EQU		     $0029
REG_PTEST       		EQU		     $002A
REG_AGCTEST     		EQU		     $002B
REG_TEST2       		EQU		     $002C
REG_TEST1       		EQU		     $002D
REG_TEST0       		EQU		     $002E
REG_PARTNUM     		EQU		     $0030
REG_VERSION     		EQU		     $0031
REG_FREQEST     		EQU		     $0032
REG_LQI        			EQU		     $0033
REG_RSSI        		EQU		     $0034
REG_MARCSTATE   		EQU		     $0035
REG_WORTIME1    		EQU		     $0036
REG_WORTIME0    		EQU		     $0037
REG_PKTSTATUS   		EQU		     $0038
REG_VCO_VC_DAC  		EQU		     $0039
REG_TXBYTES    			EQU		     $003A
REG_RXBYTES     		EQU		     $003B
REG_RCCTRL1_STATUS  	EQU			 $003C
REG_RCCTRL0_STATUS   	EQU			 $003D