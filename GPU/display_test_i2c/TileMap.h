//
//  TileMap.h
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-09-04.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#ifndef TileMap_h
#define TileMap_h

class CTileMap
{
public:
	CTileMap();
	
	void Blit();
	
	int m_width;
	int m_height;
	
	int m_offsetX;
	int m_offsetY;
	
	unsigned char m_map[ 2048 ];
};

extern CTileMap tileMap;

#endif /* TileMap_h */
