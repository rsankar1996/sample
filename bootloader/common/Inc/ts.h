/**
  ******************************************************************************
  * @file    ts.h
  * @brief   This file contains all the functions prototypes for the
  *          touch screen layer.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TS_H
#define __TS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "sx8650.h"

#define TS_SWAP_NONE                    ((uint8_t)0x00)
#define TS_SWAP_X                       ((uint8_t)0x01)
#define TS_SWAP_Y                       ((uint8_t)0x02)
#define TS_SWAP_XY                      ((uint8_t)0x04)

typedef struct
{
  uint16_t TouchDetected;
  uint16_t x;
  uint16_t y;
  uint16_t z;
}TS_StateTypeDef;


typedef enum
{
  TS_OK       = 0x00,
  TS_ERROR    = 0x01,
  TS_TIMEOUT  = 0x02
}TS_StatusTypeDef;

uint8_t TS_Init(uint16_t xSize, uint16_t ySize);
uint8_t TS_DeInit(void);
uint8_t TS_GetState(TS_StateTypeDef *TS_State);
uint8_t TS_ITConfig(void);
uint8_t TS_ITGetStatus(void);
void    TS_ITClear(void);

void touch_detect(void); //This should be moved to lcd layer
void touch_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TS_H */


/*****END OF FILE****/
