#include <Arduino.h>
#include "CRC8.h"

byte poly;
byte table[256];

CRC8::CRC8(byte polynomial = 0x83)
{
    poly=polynomial;
    computeTable();
}

void CRC8::computeTable()
{
        for (int i = 0; i < 256; ++i)
    {
        int temp = i;
        for (int j = 0; j < 8; ++j)
        {
            if ((temp & 0x80) != 0)
                temp = (temp << 1) ^ poly;
            else
                temp <<= 1;
        }
        table[i] = (byte)temp;
    }
}

byte CRC8::ComputeChecksum(byte bytes[])
{
    byte crc = 0;
    if (bytes != NULL && sizeof(bytes) > 0)
        for (byte i = 0; i < sizeof(bytes); ++i)
        {
           //Serial.println("CRC: "+String(crc)+", bytes[i]="+String(bytes[i])+", index: "+String(crc ^ bytes[i])+", table: "+String(table[crc ^ bytes[i]])+"!");
           crc = table[crc ^ bytes[i]];
        }
    //Serial.println("CRC: " + String(crc));
    return crc;
}
