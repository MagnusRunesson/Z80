//
//  TileBank.h
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-09-04.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#ifndef TileBank_h
#define TileBank_h

class CTileBank
{
public:
	void LoadFromSimulator( unsigned char* _pPixels, int _offset, int _num );
	
	unsigned char m_pixels[ 2048 ]; // 2048 bytes is 256 tiles that are 8x8 pixels (one pixel is 8 byte, so one tile is 8 bytes)
};

extern CTileBank tileBank;

#endif /* TileBank_h */
