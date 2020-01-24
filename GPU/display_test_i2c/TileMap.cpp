//
//  TileMap.cpp
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-09-04.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#include "TileMap.h"
#include "i2c.h"

CTileMap tileMap;

CTileMap::CTileMap()
{
	
}

void CTileMap::Blit()
{
	int j;
	for( j=0; j<8; j++ )
	{
		i2c_start(ADDR);
		i2c_send(CONTROL_SENDCOMMAND); //................ control byte
		i2c_send(0xB0 + j); //................ pageAddr
		i2c_stop();

		i2c_start(ADDR);
		i2c_send(CONTROL_SENDDATA);
		
		int i;
		for( i=0; i<132; i++ ) {
			i2c_send((  + i) & 255 );
		}
		
		i2c_stop();
	}
}
