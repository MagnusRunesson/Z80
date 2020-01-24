//
//  TileBank.cpp
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-09-04.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#include "TileBank.h"

void CTileBank::LoadFromSimulator( unsigned char* _pPixels, int _offset, int _num )
{
	unsigned char* wr = m_pixels + (_offset * 8);
	int i;
	for( i=0; i<_num*8; i++ )
	{
		*wr++ = *_pPixels++;
	}
}
