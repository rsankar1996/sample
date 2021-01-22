#ifndef __SDRAM_H
#define __SDRAM_H

#include "main.h"


#define SDRAM_BANK_START				((uint32_t)0xC0000000)
#define SDRAM_BANK_END 		  			((uint32_t)0xC00002F0)

#define SDRAM_TIMEOUT				       ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1		       ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL	       ((uint16_t)0x0000)
#define SDRAM_MODEREG_CAS_LATENCY_2		       ((uint16_t)0x0020)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD	       ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE	       ((uint16_t)0x0200)


void sdram_setup();
void lcd_fill_buff(void);

#endif

