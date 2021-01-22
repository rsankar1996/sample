/**
  ******************************************************************************
  * @file    sx8650.h
  * @brief   This file contains all the functions prototypes for the
  *          sx8650.c IO expander driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SX8650_H
#define __SX8650_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup SX8650
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup SX8650_Exported_Types
  * @{
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup SX8650_Exported_Constants
  * @{
  */

/*  */
#define SX8650_I2C_ADDRESS                       (0x90)
#define SX8650_REG_CTRL0                         (0x00)
#define SX8650_REG_CTRL1                         (0x01)
#define SX8650_REG_CTRL2                         (0x02)
#define SX8650_REG_CHANMSK                       (0x04)
#define SX8650_REG_STAT                          (0x05)
#define SX8650_REG_RESET                         (0x1f)

#define SX8650_CMD_WRITE_REG                     (0x00)
#define SX8650_CMD_READ_REG                      (0x40)
#define SX8650_CMD_PENTRG                        (0xe0)
#define SX8650_CMD_PENDETECT                     (0xc0)
#define SX8650_CMD_MANAUTO                       (0xb0)
#define SX8650_CMD_SELECT_SEQ                    (0x87)
#define SX8650_CMD_CONVERT_SEQ                   (0x97)



#define SX8650_CHANMSK_XCONV                     (0x1 << 7)
#define SX8650_CHANMSK_YCONV                     (0x1 << 6)
#define SX8650_CHANMSK_Z1CONV                    (0x1 << 5)
#define SX8650_CHANMSK_Z2CONV                    (0x1 << 4)


#define SX8650_CTRL0_TOUCHRATE_0010CPS           (0x01 << 4)
#define SX8650_CTRL0_TOUCHRATE_0000CPS           (0x00 << 4)

#define SX8650_CTRL0_POWDLY_8_9US                (0x0f)

#define SX8650_CTRL1_RPDNT_RESISTOR_VALUE_CFG    (0x1 << 2)
#define SX8650_CTRL1_FILT_NFILT3                 (0x3 << 0)
#define SX8650_CTRL1_RESERVED                    (0x1 << 5)

#define SX8650_CTRL2_SETDLY_8_9US                (0x0f)

#define SOFTRESET_VALUE			   (0xde)

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup sx8650_Exported_Macros
  * @{
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup sx8650_Exported_Functions
  * @{
  */

/**
  * @brief sx8650 Control functions
  */
void     sx8650_Init(uint16_t DeviceAddr);
void     sx8650_Reset(uint16_t DeviceAddr);
uint16_t sx8650_ReadID(uint16_t DeviceAddr);
void     sx8650_TS_Start(uint16_t DeviceAddr);
uint8_t  sx8650_TS_DetectTouch(uint16_t DeviceAddr);
void     sx8650_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y);
void     sx8650_TS_EnableIT(uint16_t DeviceAddr);
void     sx8650_TS_DisableIT(uint16_t DeviceAddr);
uint8_t  sx8650_TS_ITStatus (uint16_t DeviceAddr);
void     sx8650_TS_ClearIT (uint16_t DeviceAddr);

void     IOE_Init(void);
void     IOE_Delay(uint32_t delay);
uint8_t  IOE_Read(uint8_t addr, uint8_t reg);
void IOE_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint16_t IOE_ReadMultiple(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t length);
void     IOE_WriteMultiple(uint8_t addr, uint8_t reg, uint8_t *buffer, uint16_t length);

/* Touch screen driver structure */
typedef struct
{
  void       (*Init)(uint16_t);
  uint16_t   (*ReadID)(uint16_t);
  void       (*Reset)(uint16_t);
  void       (*Start)(uint16_t);
  uint8_t    (*DetectTouch)(uint16_t);
  void       (*GetXY)(uint16_t, uint16_t*, uint16_t*);
  void       (*EnableIT)(uint16_t);
  void       (*ClearIT)(uint16_t);
  uint8_t    (*GetITStatus)(uint16_t);
  void       (*DisableIT)(uint16_t);
} TS_DrvTypeDef;

extern TS_DrvTypeDef sx8650_ts_drv;

#ifdef __cplusplus
}
#endif
#endif /* __SX8650_H */


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
