/*
 * app_params.c
 *
 *  Created on: Dec 4, 2019
 *      Author: Miroslav
 */
#include "app_params.h"

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NumbOfVar] = {0x0001, 0x0002, 0x0003, 0x0004,
									 0x0005, 0x0006, 0x0007, 0x0008,
									 0x0009, 0x000A, 0x000B, 0x000C,
									 0x000D, 0x000E, 0x000F, 0x0010, 0x0011};

void

