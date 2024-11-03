#ifndef CRC8_H
#define CRC8_H

#include "Arduino.h"

class CRC8
{
public:
    CRC8(byte polynomial=0x83);
    byte ComputeChecksum(byte bytes[]);

private:
    byte table[256];
    byte poly;
    void computeTable();
};
#endif
