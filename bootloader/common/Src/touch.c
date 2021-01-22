#include <stdio.h>
#include "ts.h"
#include "main.h"

#define TS_WIDTH     800
#define TS_HEIGHT    480

static TS_StateTypeDef TS_State;

void touch_init(void)
{
        if (TS_Init(TS_WIDTH, TS_HEIGHT) != TS_OK) {
                printf("Touch Screen init failed\r\n");
        }
}

void touch_detect(void)
{
	TS_GetState(&TS_State);
}

