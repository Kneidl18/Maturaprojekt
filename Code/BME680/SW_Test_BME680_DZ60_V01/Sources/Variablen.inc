;************************************************************************************************
;*                                                                                              *
;*      Hier kommen meine Variablen rein... Zuerst die Zeropage                                 *
;*                                                                                              *
;************************************************************************************************

                ORG     Z_RAM_START            

;Realtime
TimerFlag               ds      1T
RealTimeCounter         ds      1T             
BCD_Counter             ds      1T   

;BME680
BME680_byte						ds		1B
BME680_error					ds		1T
BME680_measuringFlag			ds		1T
BME680_measuringFaultCounter	ds		1T
BME680_measuringErrorFlag		ds		1T
BME680_coeff_arr1				ds		25T
BME680_coeff_arr2				ds		16T
BME680_coeffCounter				ds		1T

;BME680 calibration values
BME680_T1						ds		2T
BME680_T2						ds		1T
BME680_T3						ds		1T
BME680_P1						ds		2T
BME680_P2						ds		1T
BME680_P3						ds		1T
BME680_P4						ds		1T
BME680_P5						ds		1T
BME680_P6						ds		1T
BME680_P7						ds		1T
BME680_P8						ds		1T
BME680_P9						ds		1T
BME680_P10						ds		1T
BME680_H1						ds		2T
BME680_H2						ds		2T
BME680_H3						ds		1T
BME680_H4						ds		1T
BME680_H5						ds		1T
BME680_H6						ds		1T
BME680_H7						ds		1T
BME680_G1						ds		1T
BME680_G2						ds		1T
BME680_G3						ds		1T
BME680_resHeatRange				ds		1T
BME680_resHeat					ds		1T
BME680_rngSwErr					ds		1T
BME680_tFine					ds		1T
BME680_temperature				ds		1T
BME680_pressure					ds		1T
BME680_humidity					ds		1T
BME680_gas						ds		1T

;SPI
SPI_Adress						ds		1T
SPI_sendByte					ds		1T
SPI_readByte					ds		1T
SPI_testByte1					ds		1T
SPI_testByte2					ds		1T
SPI_testByte3					ds		1T
SPI_testByte4					ds		1T
SPI_testByte5					ds		1T


;************************************************************************************************
;*                                                                                              *
;*      Und hier die sog. "Extend RAM", also der groﬂe Speicher                                 *
;*                                                                                              *
;************************************************************************************************

                ORG     E_RAM_START
                

