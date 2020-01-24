//
//  i2c.h
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-08-23.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#ifndef i2c_h
#define i2c_h

#include "types.h"

#define ADDR				(0x78)
#define CONTROL_SENDCOMMAND	(0x00)
#define CONTROL_SENDDATA	(0x40)

void i2c_init();
uint8 i2c_start(uint8 sla);
void i2c_stop(void);
uint8 i2c_send(uint8 data);

#endif /* i2c_h */
