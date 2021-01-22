#include <stdio.h>
#include "main.h"
#include "lcd.h"
#include "fonts.h"
#include "font24.c"
#include "sdram.h"

extern LTDC_HandleTypeDef hltdc;

static LCD_DrawPropTypeDef DrawProp;

uint16_t lcd_buff[400*272];

void config_lcd_layer(void)
{
        LTDC_LayerCfgTypeDef pLayerCfg = {0};
        pLayerCfg.WindowX0 = 0;
        pLayerCfg.WindowX1 = 800;
        pLayerCfg.WindowY0 = 0;
        pLayerCfg.WindowY1 = 480;
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
        pLayerCfg.Alpha = 255;
        pLayerCfg.Alpha0 = 0;
        pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
        pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
        pLayerCfg.FBStartAdress = SDRAM_BANK_START;
        pLayerCfg.ImageWidth = 800;
        pLayerCfg.ImageHeight = 480;
        pLayerCfg.Backcolor.Blue = 0;
        pLayerCfg.Backcolor.Green = 0;
        pLayerCfg.Backcolor.Red = 0;

        if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK) {
                Error_Handler();
        }

	DrawProp.BackColor = LCD_COLOR_WHITE;
  	DrawProp.pFont     = &Font24;
  	DrawProp.TextColor = LCD_COLOR_BLACK;
}

static uint32_t BSP_LCD_GetXSize(void)
{
  	return hltdc.LayerCfg->ImageWidth;
}

static uint32_t BSP_LCD_GetYSize(void)
{
  	return hltdc.LayerCfg->ImageHeight;
}

static void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
  	*(__IO uint32_t*) (hltdc.LayerCfg->FBStartAdress + (4*(Ypos*BSP_LCD_GetXSize() + Xpos))) = RGB_Code;
}

static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t  offset;
  uint8_t  *pchar;
  uint32_t line;

  height = DrawProp.pFont->Height;
  width  = DrawProp.pFont->Width;

  offset =  8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {

    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }

    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.TextColor);
      }
      else
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.BackColor);
      }
    }
    Ypos++;
  }
}

static void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *\
    DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);
}

void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  uint16_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0;
  uint8_t  *ptr = Text;

  while (*ptr++) size ++ ;

  xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);

  switch (Mode)
  {
  case CENTER_MODE:
    {
      refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn = - Xpos + ((xsize - size)*DrawProp.pFont->Width);
      break;
    }
  default:
    {
      refcolumn = Xpos;
      break;
    }
  }

  if ((refcolumn < 1) || (refcolumn >= 0x8000))
  {
    refcolumn = 1;
  }

  while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
  {
    BSP_LCD_DisplayChar(refcolumn, Ypos, *Text);
    refcolumn += DrawProp.pFont->Width;

    Text++;
    i++;
  }
}

