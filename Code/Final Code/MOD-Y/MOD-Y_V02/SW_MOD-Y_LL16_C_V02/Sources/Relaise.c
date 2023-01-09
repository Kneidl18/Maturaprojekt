#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"

#define Relaise1_PORT		PTED_PTED0
#define Relaise2_PORT		PTED_PTED1
#define Relaise3_PORT		PTED_PTED2
#define Relaise4_PORT		PTED_PTED3
#define Relaise5_PORT		PTED_PTED4
#define Relaise6_PORT		PTED_PTED5
#define Relaise7_PORT		PTDD_PTDD5
#define Relaise8_PORT		PTDD_PTDD6
#define Relaise9_PORT		PTDD_PTDD7

#define Relaise1_DIR		PTEDD_PTEDD0
#define Relaise2_DIR		PTEDD_PTEDD1
#define Relaise3_DIR		PTEDD_PTEDD2
#define Relaise4_DIR		PTEDD_PTEDD3
#define Relaise5_DIR		PTEDD_PTEDD4
#define Relaise6_DIR		PTEDD_PTEDD5
#define Relaise7_DIR		PTDDD_PTDDD5
#define Relaise8_DIR		PTDDD_PTDDD6
#define Relaise9_DIR		PTDDD_PTDDD7

#define PCB			0b0000
#define MODULE_RELAISE		0b0110
//************************************************************************************************
//*                                                                                              *
//*     Relaise_Init: initializing the ports for the Relaise	                                 *
//*		Relaise1-9_DIR (PTE0-PTE5, PTD5-PTE7) = input											 *
//*                                                                                              *
//************************************************************************************************
void Relaise_Init(void)
{
	Relaise1_DIR = 0;
	Relaise2_DIR = 0;
	Relaise3_DIR = 0;
	Relaise4_DIR = 0;
	Relaise5_DIR = 0;
	Relaise6_DIR = 0;
	Relaise7_DIR = 0;
	Relaise8_DIR = 0;
	Relaise9_DIR = 0;
}


//************************************************************************************************
//*                                                                                              *
//*     Relaise_CheckState: reads the ports of the Relaise and returns their status (int)		 *
//*                                                                                              *
//************************************************************************************************
int Relaise_CheckState(void)
{
	int Relaise;
	
	Relaise = Relaise1_PORT;
	Relaise = Relaise2_PORT << 1;
	Relaise = Relaise3_PORT << 2;
	Relaise = Relaise4_PORT << 3;
	Relaise = Relaise5_PORT << 4;
	Relaise = Relaise6_PORT << 5;
	Relaise = Relaise7_PORT << 6;
	Relaise = Relaise8_PORT << 7;
	Relaise = Relaise9_PORT << 8;
	
	return (Relaise);
}


void Relaise_PreparePacket(void)
{
	sendPacketArr[0] = 4;
	sendPacketArr[1] = PCB << 4;
	sendPacketArr[1] += MODULE_RELAISE;
	sendPacketArr[2] = (byte)(RelaiseState >> 8);
	sendPacketArr[3] = (byte)(RelaiseState & 0x00FF);
}