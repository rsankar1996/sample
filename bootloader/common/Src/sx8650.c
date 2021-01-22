/**
  ******************************************************************************
  * @file    sx8650.c
  * @brief   This file provides a set of functions needed to manage the SX8650
  *          IO Expander devices.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sx8650.h"
#include <stdio.h>
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup SX8650
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/** @defgroup SX8650_Private_Types_Definitions
  * @{
  */

/* Private define ------------------------------------------------------------*/

/** @defgroup SX8650_Private_Defines
  * @{
  */

/* Private macro -------------------------------------------------------------*/

/** @defgroup SX8650_Private_Macros
  * @{
  */

/* Private variables ---------------------------------------------------------*/
static uint16_t touch_x;
static uint16_t touch_y;

/** @defgroup SX8650_Private_Variables
  * @{
  */

/* Touch screen driver structure initialization */
TS_DrvTypeDef sx8650_ts_drv =
{
 sx8650_Init,
 sx8650_ReadID,
 sx8650_Reset,

 sx8650_TS_Start,
 sx8650_TS_DetectTouch,
 sx8650_TS_GetXY,

 sx8650_TS_EnableIT,
 sx8650_TS_ClearIT,
 sx8650_TS_ITStatus,
 sx8650_TS_DisableIT,
};

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup sx8650_Private_Function_Prototypes
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup sx8650_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the sx8650 and configure the needed hardware resources
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None
  */
void sx8650_Init(uint16_t DeviceAddr)
{
        /* Initialize IO BUS layer */
        IOE_Init();
}

/**
  * @brief  Reset the sx8650 by Software.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None
  */
void sx8650_Reset(uint16_t DeviceAddr)
{
        uint8_t cmd;

        cmd = SOFTRESET_VALUE;
        IOE_WriteMultiple(DeviceAddr, SX8650_REG_RESET, &cmd, 1);
}

/**
  * @brief  Read the sx8650 IO Expander device ID.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval The Device ID (two bytes).
  */
uint16_t sx8650_ReadID(uint16_t DeviceAddr)
{
        return 0;
}

/**
  * @brief  Configures the touch Screen Controller (Single point detection)
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None.
  */
void sx8650_TS_Start(uint16_t DeviceAddr)
{
        uint8_t cmd;

        /** Configuring mode */
        cmd = SX8650_CTRL0_TOUCHRATE_0010CPS | SX8650_CTRL0_POWDLY_8_9US;
        IOE_WriteMultiple(DeviceAddr, SX8650_REG_CTRL0, &cmd, 1);

        cmd = SX8650_CTRL1_RESERVED | SX8650_CTRL1_RPDNT_RESISTOR_VALUE_CFG | SX8650_CTRL1_FILT_NFILT3;
        IOE_WriteMultiple(DeviceAddr, SX8650_REG_CTRL1, &cmd, 1);

        cmd = SX8650_CTRL2_SETDLY_8_9US;
        IOE_WriteMultiple(DeviceAddr, SX8650_REG_CTRL2, &cmd, 1);

        /** Configure X/Y channel */
        cmd = SX8650_CHANMSK_XCONV | SX8650_CHANMSK_YCONV;
        IOE_WriteMultiple(DeviceAddr, SX8650_REG_CHANMSK, &cmd, 1);

        /** Enables pen trigger mode */
        cmd = SX8650_CMD_PENTRG;
        IOE_WriteMultiple(DeviceAddr, SX8650_I2C_ADDRESS, &cmd, 1);
}

/**
  * @brief  Return if there is touch detected or not.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval Touch detected state.
  */
uint8_t sx8650_TS_DetectTouch(uint16_t DeviceAddr)
{
        uint8_t state;
        uint8_t reg;
        uint8_t data[4];
        uint16_t x = 0;
        uint16_t y = 0;
        uint8_t i;

        reg = SX8650_CMD_READ_REG | SX8650_REG_STAT;
        state = IOE_Read(DeviceAddr, reg);

        if ((state & (1 << 7)) == 0) {
                return 0;
        }

        for (i = 0; i < 5; i++) {

                IOE_ReadMultiple(DeviceAddr, SX8650_I2C_ADDRESS, data, sizeof(data));

                if ((((data[0] & 0x0F) << 8) | (data[1] & 0xFF)) == 0xFFF)
                        return 0;


                if (i >= 3) {
                        x += ((data[0] & 0x0F) << 8) | (data[1] & 0xFF);
                        y += ((data[2] & 0x0F) << 8) | (data[3] & 0xFF);
                }


                IOE_Delay(5);
        }

        touch_x = x / 2;
        touch_y = y / 2;

        return 1;
}

/**
  * @brief  Get the touch screen X and Y positions values
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  X: Pointer to X position value
  * @param  Y: Pointer to Y position value
  * @retval None.
  */
void sx8650_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y)
{
        *X = touch_x;
        *Y = touch_y;
}

/**
  * @brief  Configure the selected source to generate a global interrupt or not
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None
  */
void sx8650_TS_EnableIT(uint16_t DeviceAddr)
{
}

/**
  * @brief  Configure the selected source to generate a global interrupt or not
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None
  */
void sx8650_TS_DisableIT(uint16_t DeviceAddr)
{
}

/**
  * @brief  Configure the selected source to generate a global interrupt or not
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval TS interrupts status
  */
uint8_t sx8650_TS_ITStatus(uint16_t DeviceAddr)
{
        uint8_t reg;

        reg = SX8650_CMD_READ_REG | SX8650_REG_STAT;
        return IOE_Read(DeviceAddr, reg);
}

/**
  * @brief  Configure the selected source to generate a global interrupt or not
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval None
  */
void sx8650_TS_ClearIT(uint16_t DeviceAddr)
{
        uint8_t reg;

        reg = SX8650_CMD_READ_REG | SX8650_REG_STAT;
        IOE_Read(DeviceAddr, reg);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/*****END OF FILE****/
