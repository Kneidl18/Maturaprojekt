#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "MC9S08DZ60.h"
#include "BME680_REG.h"


//returns the values of the read temperature, pressure, humidity and gas 
void BME680_getSensorData(void)
{
	const uint lookupTable1[16] = {
		2147483647, 2147483647, 2147483647, 2147483647,
		2147483647, 2126008810, 2147483647, 2130303777,
		2147483647, 2147483647, 2143188679, 2136746228,
		2147483647, 2126008810, 2147483647, 2147483647};
	const uint lookupTable2[16] = {
		4096000000, 2048000000, 1024000000, 512000000,
		255744255,  127110228,  64000000,   32258064,
		16016016,   8000000,    4000000,    2000000,
		1000000,    500000,     250000,     125000};

	uint	buff[15], gas_range = 0;                          // declare array for registers
  	int		var1, var2, var3, var4, var5, var6, temp_scaled;  // Work variables
  	uint	adc_temp, adc_pres;                               // Raw ADC temperature and pressure
  	uint	adc_hum, adc_gas_res;                             // Raw ADC humidity and gas
	int _H1, _H2, _T1, _P1, _G2, _T2, _P2, _P4, _P5, _P8, _P9;
	uint i = 0;
  	uint uvar2;
	
	_H1 = (BME680_H1[0] << 8) + BME680_H1[1];
	_H2 = (BME680_H2[0] << 8) + BME680_H2[1];
	_T1 = (BME680_T1[0] << 8) + BME680_T1[1];
	_P1 = (BME680_P1[0] << 8) + BME680_P1[1];
	_G2 = (BME680_G2[0] << 8) + BME680_G2[1];
	_T2 = (BME680_T2[0] << 8) + BME680_T2[1];
	_P2 = (BME680_P2[0] << 8) + BME680_P2[1];
	_P4 = (BME680_P4[0] << 8) + BME680_P4[1];
	_P5 = (BME680_P5[0] << 8) + BME680_P5[1];
	_P8 = (BME680_P8[0] << 8) + BME680_P8[1];
	_P9 = (BME680_P9[0] << 8) + BME680_P9[1];
  
	while (i++ < 15)
	{
		buff[i] = SPI_readReg(BME680_STATUS_REGISTER + i);		//TODO not sure if it works
	}
	
	adc_pres = ((buff[2] << 12) | (buff[3] << 4)| (buff[4] >> 4));
	
	adc_temp = ((buff[5] << 12) | (buff[6] << 4) | (buff[7] >> 4));
	
	adc_hum = ((buff[8] << 8) | buff[9]);
	
	adc_gas_res = ((buff[13] << 2) | (buff[14] >> 6));
	
	gas_range = buff[14] & 0x0f;
	
	//computing the temperature
	var1 = (adc_temp >> 3) - (_T1 << 1);
	var2 = (var1 * _T2) >> 11;
	var3 = ((var1 >> 1) * (var1 >> 1)) >> 12;
	var3 = ((var3) * (BME680_T3 << 4)) >> 14;
	BME680_tFine = (var2 + var3);
	BME680_temperature = ((BME680_tFine * 5) + 128) >> 8;
	
	//computing the pressure
	var1 = (BME680_tFine >> 1) - 64000;
	var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * BME680_P6) >> 2;
	var2 = var2 + ((var1 * _P5) <<1);
	var2 = (var2 >> 2) + (_P4 << 16);
	var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * (BME680_P3)) >> 3) + ((_P2 * var1) >> 1);
	var1 = var1 >> 18;
	var1 = ((32768 + var1) * _P1) >> 15;
	BME680_pressure = 1048576 - adc_pres;
	BME680_pressure = ((BME680_pressure - (var2 >> 12)) * (3125));
	
	if(BME680_pressure >= 0x40000000)
	{
		BME680_pressure = ((BME680_pressure / var1) << 1);
	}
	else
	{
		BME680_pressure = ((BME680_pressure << 1) / var1);
	}
	
	var1 = (_P9 * (((BME680_pressure >> 3) * (BME680_pressure >> 3)) >> 13)) >> 12;
	var2 = ((BME680_pressure >> 2) * _P8) >> 13;
	var3 = ((BME680_pressure >> 8) * (BME680_pressure >> 8) * (BME680_pressure >> 8) * BME680_P10) >> 17;
	BME680_pressure = (BME680_pressure) + ((var1 + var2 + var3 +(BME680_P7 << 7)) >> 4);
	
	//computing the humidity
	temp_scaled = ((BME680_tFine * 5) + 128) >> 8;
	var1 = (adc_hum - ((_H1 << 4))) - (((temp_scaled * BME680_H3) / 100) >> 1);
	var2 = (_H2 * (((temp_scaled * BME680_H4) / 100) + (((temp_scaled * ((temp_scaled * BME680_H5) / 100)) >> 6) / 100) + (1 << 14))) >> 10;
	var3 = var1 * var2;
	var4 = BME680_H6 << 7;
	var4 = (var4 + ((temp_scaled * BME680_H7) / 100)) >> 4;
	var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
	var6 = (var4 * var5) >> 1;
	
	BME680_humidity = (((var3 + var6) >> 10) * 1000) >> 12;
	
	if(BME680_humidity > 100000)
	{
		BME680_humidity = 100000;
	}
	else if (BME680_humidity < 0)
	{
		BME680_humidity = 0;
	}
	
	//computing the gas
	var1 = ((1340 + (5 * BME680_rngSwErr)) * (lookupTable1[gas_range])) >> 16;
	uvar2 = (((adc_gas_res << 15) - 16777216) + var1);
	var3 = ((lookupTable2[gas_range] * var1) >> 9);
	BME680_gas = ((var3 + (uvar2 >>1)) / uvar2);
	BME680_triggerMeasurement();
}

//reset's the BME680
void BME680_reset(void)
{
	uint i = 0;
	SPI_writeReg(BME680_SOFTRESET_REGISTER, BME680_RESET_CODE);
	
	while(i++ < 255)
	{
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
	}
	
	BME680_Init();
}

//calculates the altitude by using the current air pressure
uint BME680_altitude(int press, float seaLevel)
{
	float altitude;
	altitude = 44330.0 * (1.0 - pow((press / 100.0) / seaLevel , 0.1903));
	return (altitude);
}

//set's oversampling16 for temperature, humidity and pressure
//datasheet says h then t and at last p
void BME680_setOversampling(void)
{
	unsigned char tempRegister;
	BME680_waitForReadings();
	
	//humidity
	tempRegister = SPI_readReg(BME680_CONTROL_HUMIDITY_REGISTER);
	tempRegister &= BME680_HUMIDITY_MASK;
	tempRegister |= 1UL << 0;
	tempRegister |= ~(1UL << 1);
	tempRegister |= 1UL << 2;
	
	
	SPI_writeReg(BME680_CONTROL_HUMIDITY_REGISTER, tempRegister);
	//temperature
	tempRegister = SPI_readReg(BME680_CONTROL_MEASURE_REGISTER);
	tempRegister &= BME680_TEMPERATURE_MASK;
	tempRegister |= 1UL << 5;
	tempRegister |= ~(1UL << 6);
	tempRegister |= 1UL << 7;
	
	SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, tempRegister);	
	
	
	//pressure
	tempRegister = SPI_readReg(BME680_CONTROL_MEASURE_REGISTER);
	tempRegister &= BME680_PRESSURE_MASK;
	tempRegister |= 1UL << 2;
	tempRegister |= ~(1UL << 3);
	tempRegister |= 1UL << 4;
	
	SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, tempRegister);
}


//set's the IIR Filter
void BME680_setIIRFilter(void)
{
	unsigned char tempVal = SPI_readReg(BME680_CONFIG_REGISTER);
	BME680_waitForReadings();
	
	tempVal = tempVal & 0b11100011;
	tempVal |= (0b100 << 2);
	
	SPI_writeReg(BME680_CONFIG_REGISTER, tempVal);
}

//setGas set's the gas measurement target temperature and heating time
void BME680_setGas(uint gasTemp, uint gasMillis)
{
	unsigned char heatrRes;
	uint factor = 0;
	uint durval;
	uint var1, var2, var3, var4, var5, heatrResX100;
	unsigned char gasRegister = 0;
	uint G2;
	
	BME680_waitForReadings();
	gasRegister = SPI_readReg(BME680_CONTROL_GAS_REGISTER2);
	
	if (gasTemp < 200)
	{
		gasTemp = 200;
	}
	else if(gasTemp > 400)
	{
		gasTemp = 400;
	}
	
	G2 = BME680_G2[0] << 8;
	G2 += BME680_G2[1];
	
	SPI_writeReg(BME680_CONTROL_GAS_REGISTER1, 0);
	var1 = (((BME680_temperature / 100) * BME680_H3) / 1000) << 8;
	var2 = ((BME680_G1 + 784) * ((((G2 + 154009) * gasTemp * 5) / 100) + 3276800)/ 10);
	var3 = var1 + (var2 / 2);
	var4 = (var3 / (BME680_resHeatRange + 4));
	var5 = (131 * BME680_resHeat) + 65536;
	heatrResX100 = (((var4 / var5) - 250) * 34);
	heatrRes = ((heatrResX100 + 50) / 100);
	
	SPI_writeReg(BME680_GAS_HEATER_REGISTER0, heatrRes);
	
	if (gasMillis >= 0xfc0)
	{
		durval = 0xff;
	}
	else
	{
		while(gasMillis > 0x3F)
		{
			gasMillis = gasMillis >> 2;
			factor++;
		}
		
		durval = (gasMillis + (factor * 64));
	}
	
	SPI_writeReg(BME680_CONTROL_GAS_REGISTER1, 0);
	SPI_writeReg(BME680_GAS_DURATION_REGISTER0, durval);
	SPI_writeReg(BME680_CONTROL_GAS_REGISTER2, (gasRegister | 0b00010000));
}


//waiting for the BME680 to finish measuring
void BME680_waitForReadings(void)
{	
	while (BME680_measuring() == 1)	{ }
}
 
 
//returns 1 if measuring, 0 if not measuring (not sure if it works)
unsigned char BME680_measuring(void)
{
	Byte result = 0;
	unsigned char tempReg = SPI_readReg(BME680_STATUS_REGISTER);
	
	if(tempReg & (1<<BME680_MEASURING_BIT_POSITION) != 0)
	{
		result = 1;
	}
	
	return result;
}

//triggers a new measurement
void BME680_triggerMeasurement(void)
{
	unsigned char workRegister = SPI_readReg(BME680_CONTROL_MEASURE_REGISTER);
	workRegister |= 1;
	SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, workRegister);
}


//Init, checks up on the chip-id and returnes if it's correct.
//also reads in the configuration values
void BME680_Init(void)
{
	if(SPI_readReg(BME680_CHIPID_REGISTER) == (Byte)BME680_CHIPID)
	{
		unsigned char workRegister = 0;
		BME680_waitGetData = 0;
		
		BME680_getCalibration();
		
		workRegister = SPI_readReg(BME680_CONTROL_MEASURE_REGISTER);
		workRegister = workRegister | 1;
		SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, workRegister);
		
		Nop();
		
		BME680_setOversampling();
		BME680_setIIRFilter();
		BME680_set Gas(320, 150);
		BME680_ChipIdCorrect = 1;
	}
	else{ BME680_ChipIdCorrect = 0; }
}

void BME680_getCalibration(void)
{
	uint tempVar = 0;
	
	uint coeff_arr1[BME680_COEFF_SIZE1] = {0};
	uint coeff_arr2[BME680_COEFF_SIZE2] = {0};
	
	uint i = 0;
	while(i++ < BME680_COEFF_SIZE1)
	{
		coeff_arr1[i] = SPI_readReg(BME680_COEFF_START_ADDRESS1 + i);
	}
	i = 0;
	while(i++ < BME680_COEFF_SIZE2)
	{
		coeff_arr2[i] = SPI_readReg(BME680_COEFF_START_ADDRESS2 + i);
	}
	
	BME680_T1[0] = coeff_arr2[BME680_T1_MSB_REG];
	BME680_T1[1] = coeff_arr2[BME680_T1_LSB_REG];  
	BME680_T2[0] = coeff_arr1[BME680_T2_MSB_REG];
	BME680_T2[1] = coeff_arr1[BME680_T2_LSB_REG];
	BME680_T3 	 = coeff_arr1[BME680_T3_REG];
	
	BME680_P1[0] = coeff_arr1[BME680_P1_MSB_REG];
	BME680_P1[1] = coeff_arr1[BME680_P1_LSB_REG];
	BME680_P2[0] = coeff_arr1[BME680_P2_MSB_REG];
	BME680_P2[1] = coeff_arr1[BME680_P2_LSB_REG];
	BME680_P3 	 = coeff_arr1[BME680_P3_REG];
	BME680_P4[0] = coeff_arr1[BME680_P4_MSB_REG];
	BME680_P4[1] = coeff_arr1[BME680_P4_LSB_REG];
	BME680_P5[0] = coeff_arr1[BME680_P5_MSB_REG];
	BME680_P5[1] = coeff_arr1[BME680_P5_LSB_REG];
	BME680_P6 	 = coeff_arr1[BME680_P6_REG];
	BME680_P7 	 = coeff_arr1[BME680_P7_REG];
	BME680_P8[0] = coeff_arr1[BME680_P8_MSB_REG];
	BME680_P8[1] = coeff_arr1[BME680_P8_LSB_REG];
	BME680_P9[0] = coeff_arr1[BME680_P9_MSB_REG];
	BME680_P9[1] = coeff_arr1[BME680_P9_LSB_REG];
	BME680_P10 	 = coeff_arr1[BME680_P10_REG];
	
	BME680_H1[0] = coeff_arr1[BME680_H1_MSB_REG];
	BME680_H1[1] = coeff_arr1[BME680_H1_LSB_REG];
	BME680_H2[0] = coeff_arr1[BME680_H2_MSB_REG];
	BME680_H2[1] = coeff_arr1[BME680_H2_LSB_REG];
	BME680_H3 	 = coeff_arr1[BME680_H3_REG];
	BME680_H4 	 = coeff_arr1[BME680_H4_REG];
	BME680_H5 	 = coeff_arr1[BME680_H5_REG];
	BME680_H6 	 = coeff_arr1[BME680_H6_REG];
	BME680_H7 	 = coeff_arr1[BME680_H7_REG];
	
	BME680_G1 	 = coeff_arr1[BME680_GH1_REG];
	BME680_G2[0] = coeff_arr1[BME680_GH2_MSB_REG];
	BME680_G2[1] = coeff_arr1[BME680_GH2_LSB_REG];
	BME680_G3 	 = coeff_arr1[BME680_GH3_REG];
	
	tempVar = SPI_readReg(BME680_ADDR_RES_HEAT_RANGE_ADDR);
	BME680_resHeatRange = (tempVar & BME680_RHRANGE_MSK) / 16;
	BME680_resHeat = SPI_readReg(BME680_ADDR_RES_HEAT_VAL_ADDR);
	tempVar = SPI_readReg(BME680_ADDR_RANGE_SW_ERR_ADDR);
	BME680_rngSwErr = (tempVar & BME680_RSERROR_MSK) / 16;

}
	
