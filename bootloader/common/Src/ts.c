#include "ts.h"

static TS_DrvTypeDef *tsDriver;
static uint16_t tsXBoundary, tsYBoundary;
static uint8_t  tsOrientation;
static uint8_t  I2cAddress;

/**
  * @brief  Initializes and configures the touch screen functionalities and
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  xSize: Maximum X size of the TS area on LCD
  * @param  ySize: Maximum Y size of the TS area on LCD
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t TS_Init(uint16_t xSize, uint16_t ySize)
{
        uint8_t status = TS_OK;
        tsXBoundary = xSize;
        tsYBoundary = ySize;

        IOE_Init();

        /* Initialize the TS driver structure */
        tsDriver = &sx8650_ts_drv;
        I2cAddress = SX8650_I2C_ADDRESS;
        tsOrientation = TS_SWAP_NONE;

        /* Initialize the TS driver */
        tsDriver->Init(I2cAddress);
        tsDriver->Reset(I2cAddress);
        tsDriver->Start(I2cAddress);

        return status;
}

/**
  * @brief  DeInitializes the TouchScreen.
  * @retval TS state
  */
uint8_t TS_DeInit(void)
{
        /* Actually ts_driver does not provide a DeInit function */
        return TS_OK;
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t TS_ITConfig(void)
{
        return TS_OK;
}

/**
  * @brief  Gets the touch screen interrupt status.
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t TS_ITGetStatus(void)
{
        /* Return the TS IT status */
        return (tsDriver->GetITStatus(I2cAddress));
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TS_State: Pointer to touch screen current state structure
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t TS_GetState(TS_StateTypeDef *TS_State)
{
        static uint32_t _x = 0, _y = 0;
        uint16_t x_diff, y_diff , x , y;
        uint16_t swap;

        TS_State->TouchDetected = tsDriver->DetectTouch(I2cAddress);

        if(TS_State->TouchDetected) {
                tsDriver->GetXY(I2cAddress, &x, &y);

                if(tsOrientation & TS_SWAP_X) {
                        x = 4096 - x;
                }

                if(tsOrientation & TS_SWAP_Y) {
                        y = 4096 - y;
                }

                if(tsOrientation & TS_SWAP_XY) {
                        swap = y;
                        y = x;
                        x = swap;
                }

                x_diff = x > _x? (x - _x): (_x - x);
                y_diff = y > _y? (y - _y): (_y - y);

                if (x_diff + y_diff > 5) {
                        _x = x;
                        _y = y;
                }

                TS_State->x = (tsXBoundary * _x) >> 12;
                TS_State->y = (tsYBoundary * _y) >> 12;
        }
        return TS_OK;
}

/**
  * @brief  Clears all touch screen interrupts.
  * @retval None
  */
void TS_ITClear(void)
{
        /* Clear TS IT pending bits */
        tsDriver->ClearIT(I2cAddress);
}


