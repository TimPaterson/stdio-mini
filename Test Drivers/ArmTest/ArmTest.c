/*
 * ArmTest.c
 *
 * Created: 4/29/2020 4:56:44 PM
 * Author : Tim
 */ 


#include "sam.h"
#include "sam_spec.h"
#include "..\..\libc\include\stdio.h"
#include <stdlib.h>


#define	VERSION		1
#define F_CPU		48000000
#define BAUD_RATE	500000

//*********************************************************************
// Initialization Helpers
//*********************************************************************

enum RxPad
{
	RXPAD_Pad0,
	RXPAD_Pad1,
	RXPAD_Pad2,
	RXPAD_Pad3
};

enum TxPad
{
	TXPAD_Pad0,
	TXPAD_Pad2,
	TXPAD_Pad0_RTS_Pad2_CTS_Pad3,
	TXPAD_Pad0_TE_Pad2
};

//*********************************************************************

void StartClock()
{
	// Two wait states needed for 48MHz operation
	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS(2) | NVMCTRL_CTRLB_MANW;

	// Initialize 48MHz clock
	OSCCTRL->CAL48M.reg = NVM_SOFTWARE_CAL->CAL48M_3V3;
	OSCCTRL->OSC48MDIV.reg = 0;		// Bump it to 48 MHz
}

//*********************************************************************

uint16_t CalcBaudRate(uint32_t rate, uint32_t clock)
{
	uint32_t	quo;
	uint32_t	quoBit;

	rate *= 16;		// actual clock frequency
	// Need 17-bit result of rate / clock
	for (quo = 0, quoBit = 1 << 16; quoBit != 0; quoBit >>= 1)
	{
		if (rate >= clock)
		{
			rate -= clock;
			quo |= quoBit;
		}
		rate <<= 1;
	}
	// Round
	if (rate >= clock)
		quo++;
	return (uint16_t)-quo;
}

//*********************************************************************

void Init()
{
	SERCOM_USART_CTRLA_Type	serCtrlA;

	// Enable clock
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_SERCOM3;

	// Clock it with GCLK0
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 |
		GCLK_PCHCTRL_CHEN;

	PORT->Group[0].WRCONFIG.reg =
		PORT_WRCONFIG_WRPMUX |
		PORT_WRCONFIG_PMUX(MUX_PA24C_SERCOM3_PAD2) |
		PORT_WRCONFIG_INEN |
		PORT_WRCONFIG_PMUXEN |
		PORT_WRCONFIG_WRPINCFG |
		PORT_WRCONFIG_HWSEL |
		PORT_WRCONFIG_PINMASK((PORT_PA24 | PORT_PA25) >> 16);

	SERCOM3->USART.BAUD.reg = CalcBaudRate(BAUD_RATE, F_CPU);

	// standard 8,N,1 parameters
	serCtrlA.reg = 0;
	serCtrlA.bit.DORD = 1;		// LSB first
	serCtrlA.bit.MODE = 1;		// internal clock
	serCtrlA.bit.RXPO = RXPAD_Pad3;
	serCtrlA.bit.TXPO = TXPAD_Pad2;
	serCtrlA.bit.ENABLE = 1;
	SERCOM3->USART.CTRLA.reg = serCtrlA.reg;
	SERCOM3->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN;
}

//*********************************************************************
// File I/O
//*********************************************************************

void WriteByte(void *pv, char c)
{
	while (!SERCOM3->USART.INTFLAG.bit.DRE);
	SERCOM3->USART.DATA.reg = c;
}

int ReadByte(void *pv)
{
	while (!SERCOM3->USART.INTFLAG.bit.RXC);
	return SERCOM3->USART.DATA.reg;
}

FILE SercomIo = FDEV_SETUP_STREAM(WriteByte, ReadByte, _FDEV_SETUP_RW | _FDEV_SETUP_CRLF);

FDEV_STANDARD_STREAMS(&SercomIo, &SercomIo);	// stdout, stdin

//*********************************************************************
// Main program
//*********************************************************************

int main(void)
{
	float	flt;
	double	dbl;
	uint64_t	ull;

    StartClock();
    Init();

	// Test runtime initialization too
	fdev_setup_stream(&SercomIo, WriteByte, ReadByte, _FDEV_SETUP_RW | _FDEV_SETUP_CRLF);

	printf("\nStarting version %i\n", VERSION);
	strtod("12.34E-1", NULL);
	strtof("12.34E-1", NULL);
	printf("%g\n", atof("-123456789E-52"));

    while (1) 
    {
		printf("Enter 64-bit hex value: ");
		scanf("%llx", &ull);
		printf("\n%llx  %llu\n", ull, ull);
		
		printf("Enter float value: ");
		scanf("%f", &flt);
		//printf("\n%g\n", PASS_FLOAT(flt));
		printf("%g\n", flt);
		
		printf("Enter double value: ");
		scanf("%lf", &dbl);
		printf("\n%g\n", dbl);
    }
}
