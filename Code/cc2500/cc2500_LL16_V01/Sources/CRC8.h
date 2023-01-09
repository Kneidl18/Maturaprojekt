#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"

void CRC8_Init(void)
{
	CRC8_poly = 0x83;
	CRC8_computeTable();
}

void CRC8_computeTable(void)
{
	int temp, i, j;
        for (i = 0; i < 46; ++i)
    	{
	        temp = i;
	        for (j = 0; j < 8; ++j)
	        {
	            if ((temp & 0x80) != 0)
	                temp = (temp << 1) ^ CRC8_poly;
	            else
	                temp <<= 1;
	        }
	        CRC8_table[i] = (byte)temp;
    }
}


unsigned char CRC8_computeChecksum(unsigned char bytes[])
{
    byte crc = 0;
    byte i;
    
    for (i = 0; i < bytes[0]; ++i)
    {
       crc = CRC8_table[crc ^ bytes[i]];
    }
    
    return crc;
}