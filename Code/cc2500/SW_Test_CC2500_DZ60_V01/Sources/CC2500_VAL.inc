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
; ***************************************************************
; *  SmartRF Studio(tm) Export
; *
; *  Radio register settings specifed with C-code
; *  compatible #define statements.
; *
; *  RF device: CC2500
; *
; ***************************************************************/
 

VAL_IOCFG2           EQU          $29
VAL_IOCFG1           EQU          $2E
VAL_IOCFG0           EQU          $06
VAL_FIFOTHR          EQU          $07
VAL_SYNC1            EQU          $D3
VAL_SYNC0            EQU          $91
VAL_PKTLEN           EQU          $FF
VAL_PKTCTRL1         EQU          $04
VAL_PKTCTRL0         EQU          $05
VAL_ADDR             EQU          $00
VAL_CHANNR           EQU          $00
VAL_FSCTRL1          EQU          $08
VAL_FSCTRL0          EQU          $00
VAL_FREQ2            EQU          $5D
VAL_FREQ1            EQU          $93
VAL_FREQ0            EQU          $B1
VAL_MDMCFG4          EQU          $86
VAL_MDMCFG3          EQU          $83
VAL_MDMCFG2          EQU          $03
VAL_MDMCFG1          EQU          $22
VAL_MDMCFG0          EQU          $F8
VAL_DEVIATN          EQU          $44
VAL_MCSM2            EQU          $07
VAL_MCSM1            EQU          $30
VAL_MCSM0            EQU          $18
VAL_FOCCFG           EQU          $16
VAL_BSCFG            EQU          $6C
VAL_AGCCTRL2         EQU          $03
VAL_AGCCTRL1         EQU          $40
VAL_AGCCTRL0         EQU          $91
VAL_WOREVT1          EQU          $87
VAL_WOREVT0          EQU          $6B
VAL_WORCTRL          EQU          $F8
VAL_FREND1           EQU          $56
VAL_FREND0           EQU          $10
VAL_FSCAL3           EQU          $A9
VAL_FSCAL2           EQU          $0A
VAL_FSCAL1           EQU          $00
VAL_FSCAL0           EQU          $11
VAL_RCCTRL1          EQU          $41
VAL_RCCTRL0          EQU          $00
VAL_FSTEST           EQU          $59
VAL_PTEST            EQU          $7F
VAL_AGCTEST          EQU          $3F
VAL_TEST2            EQU          $88
VAL_TEST1            EQU          $31
VAL_TEST0            EQU          $0B
VAL_PARTNUM          EQU          $80
VAL_VERSION          EQU          $03
VAL_FREQEST          EQU          $00
VAL_LQI              EQU          $00
VAL_RSSI             EQU          $00
VAL_MARCSTATE        EQU          $00
VAL_WORTIME1         EQU          $00
VAL_WORTIME0         EQU          $00
VAL_PKTSTATUS        EQU          $00
VAL_VCO_VC_DAC       EQU          $00
VAL_TXBYTES          EQU          $00
VAL_RXBYTES          EQU          $00
VAL_RCCTRL1_STATUS   EQU          $00
VAL_RCCTRL0_STATUS   EQU          $00