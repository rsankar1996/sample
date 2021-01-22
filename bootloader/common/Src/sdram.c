#include "main.h"
#include "sdram.h"

#include <stdio.h>
#include <string.h>

FMC_SDRAM_CommandTypeDef command;
extern SDRAM_HandleTypeDef hsdram1;

void lcd_fill_buff(void)
{
	uint32_t sdram_lcd_base = SDRAM_BANK_START;

	while(sdram_lcd_base < SDRAM_BANK_END) {
		*(__IO uint16_t*)sdram_lcd_base++ = 0xFFFF;
	}
}


void sdram_setup()
{
        __IO uint32_t tmpmrd =0;
        /* Configure a clock configuration enable command */
        command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0;

        /* Send the command */
        HAL_SDRAM_SendCommand(&hsdram1, &command, SDRAM_TIMEOUT);

        /* Insert 100 us minimum delay */
        /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
        HAL_Delay(1);

        /* Configure a PALL (precharge all) command */
        command.CommandMode = FMC_SDRAM_CMD_PALL;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0;

        /* Send the command */
        HAL_SDRAM_SendCommand(&hsdram1, &command, SDRAM_TIMEOUT);

        /* Configure a Auto-Refresh command */
        command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 8;
        command.ModeRegisterDefinition = 0;

        /* Send the command */
        HAL_SDRAM_SendCommand(&hsdram1, &command, SDRAM_TIMEOUT);

        /* Program the external memory mode register */
        tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                             SDRAM_MODEREG_CAS_LATENCY_2           |
                             SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                             SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

        command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = tmpmrd;

        /* Send the command */
        HAL_SDRAM_SendCommand(&hsdram1, &command, SDRAM_TIMEOUT);

        /* Set the refresh rate counter */
        /* (15.62 us x Freq) - 20 */
        /* Set the device refresh counter */
        hsdram1.Instance->SDRTR |= ((uint32_t)((1292)<< 1));
}

