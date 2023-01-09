#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "MC9S08LL16.h"
#include "BME680_REG.h"
#include "inputToCommand.h"

#define PCB 		PCB_MOD_X3
#define MODULE_BME	SENSOR_HUMIDITY

//returns the values of the read temperature, pressure, humidity and gas 
void BME680_getSensorData(void)
{
	BME680_triggerMeasurement();
	BME680_readValues();
}


//prepares the paceket for the cc2500 to send the read raw data
void BME680_preparePacketValues(void)
{
	byte i;
	
	SS_type = 1;
	
	CC2500_sendPacketArr[0] = 17;
	CC2500_sendPacketArr[1] = PCB << 4;
	CC2500_sendPacketArr[1] += MODULE_BME;
	
	for(i = 2; i < 15 + 2; i++)
	{
		CC2500_sendPacketArr[i] = BME680_values[i-2];
		Nop();
	}
}

void BME680_readValues(void)
{
	SS_type = 1;
	
	BME680_waitForReadings();
	
	for(i = 0; i < 15; i++)
	{
		BME680_values[i] = SPI_readReg(BME680_STATUS_REGISTER + i);
		Nop();
	}
}


//reset's the BME680
void BME680_reset(void)
{
	SS_type = 1;
	SPI_writeReg(BME680_SOFTRESET_REGISTER, BME680_RESET_CODE);
	
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	
	BME680_Init();
}

//set's oversampling16 for temperature, humidity and pressure
//datasheet says h then t and at last p
void BME680_setOversampling(void)
{
	unsigned char tempRegister;
	
	SS_type = 1;
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

	SS_type = 1;
	BME680_waitForReadings();
	
	tempVal = tempVal & 0b11100011;
	tempVal |= (0b100 << 2);
	
	SPI_writeReg(BME680_CONFIG_REGISTER, tempVal);
}

//setGas set's the gas measurement target temperature and heating time
void BME680_setGas()
{	
	unsigned char gasRegister;

	SS_type = 1;	
	BME680_waitForReadings();
	gasRegister = SPI_readReg(BME680_CONTROL_GAS_REGISTER2);
	
	SPI_writeReg(BME680_GAS_HEATER_REGISTER0, BME680_heatrRes);
		
	SPI_writeReg(BME680_CONTROL_GAS_REGISTER1, 0);
	SPI_writeReg(BME680_GAS_DURATION_REGISTER0, BME680_durval);
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
	
	SS_type = 1;
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
	
	SS_type = 1;
	workRegister |= 1;
	SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, workRegister);
}


//Init, checks up on the chip-id and returnes if it's correct.
//also reads in the configuration values
void BME680_Init(void)
{
	SS_type = 1;
	if(SPI_readReg(BME680_CHIPID_REGISTER) == (Byte)BME680_CHIPID)
	{
		unsigned char workRegister = 0;
		BME680_waitGetData = 0;
		
		//BME680_getCalibration();
		
		workRegister = SPI_readReg(BME680_CONTROL_MEASURE_REGISTER);
		workRegister = workRegister | 1;
		SPI_writeReg(BME680_CONTROL_MEASURE_REGISTER, workRegister);
		
		Nop();
		
		BME680_setOversampling();
		BME680_setIIRFilter();
		
		BME680_heatrRes = 135;
		BME680_durval = 101;
		BME680_setGas();
		
		BME680_ChipIdCorrect = 1;
	}
	else{ BME680_ChipIdCorrect = 0; }
}


void BME680_getCalibration(void)
{
	uint tempVar = 0;
	
	uchar coeff_arr1[BME680_COEFF_SIZE1] = {0};
	uchar coeff_arr2[BME680_COEFF_SIZE2] = {0};
	
	uint i = 0;
	
	SS_type = 1;
	while(i++ < BME680_COEFF_SIZE1)
	{
		coeff_arr1[i] = SPI_readReg(BME680_COEFF_START_ADDRESS1 + i);
	}
	i = 0;
	while(i++ < BME680_COEFF_SIZE2)
	{
		coeff_arr2[i] = SPI_readReg(BME680_COEFF_START_ADDRESS2 + i);
	}
	
	tempVar = SPI_readReg(BME680_ADDR_RES_HEAT_RANGE_ADDR);
	//BME680_resHeatRange = (tempVar & BME680_RHRANGE_MSK) / 16;
	//BME680_resHeat = SPI_readReg(BME680_ADDR_RES_HEAT_VAL_ADDR);
	tempVar = SPI_readReg(BME680_ADDR_RANGE_SW_ERR_ADDR);
//	BME680_rngSwErr = (tempVar & BME680_RSERROR_MSK) / 16;
}