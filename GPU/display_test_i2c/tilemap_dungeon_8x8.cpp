//
// Data file generated from ../Art/dungeon/tilemap_dungeon_8x8.tmx
//

#include "Engine/Types.h"
#include "Engine/Graphics/TileMap.h"

const uint16 tilemap_dungeon_8x8_tiles[] =
{
	8,1,2,2,3,4,2,5,1,2,2,22,4,4,5,8,
	23,17,10,26,19,18,28,21,17,20,26,26,19,11,29,8,
	38,25,20,6,35,7,19,13,25,20,11,6,7,11,29,38,
	2,15,11,29,1,15,28,14,15,20,11,14,15,27,13,8,
	10,28,10,21,17,11,12,12,20,19,10,10,28,28,29,8,
	34,35,36,37,25,19,11,11,28,6,34,34,34,34,37,8,
	8,8,8,38,9,19,18,18,12,21,38,8,8,8,30,8,
	8,31,8,8,33,34,7,26,6,37,8,8,23,8,8,8,
};

extern "C" const CTileMap tilemap_dungeon_8x8;
const CTileMap tilemap_dungeon_8x8 =
{
	16,8,
	0,
	(uint16*)&tilemap_dungeon_8x8_tiles,
};

