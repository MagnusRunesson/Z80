//
//  main.cpp
//  png2c
//
//  Created by Magnus Runesson on 01/12/15.
//  Copyright (c) 2015 Pokewhat. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_Image.h>

typedef struct
{
	unsigned short w;
	unsigned short h;
	unsigned short* pixels;
	unsigned char* alpha;
} Image;

Image myImage
{
	10, 10, NULL, NULL
};

void writeHeader( FILE* f, char* _symbolNameBase, SDL_Surface* image )
{
	fprintf( f, "\n" );
}

void writePixels( FILE* f, char* _symbolNameBase, SDL_Surface* image, int* _pTotalOutputSize )
{
	fprintf( f, "#include <avr/pgmspace.h>\n" );
	fprintf( f, "\n" );
	
	fprintf( f, "extern \"C\" const unsigned char PROGMEM %s_tilebank[];\n", _symbolNameBase );
	fprintf( f, "const unsigned char PROGMEM %s_tilebank[] =\n{\n", _symbolNameBase );

	unsigned char* pixels = (unsigned char*)image->pixels;
	int x, y;
	for( y=0; y<image->h; y+=8 )
	{
		fprintf( f, "\t" );
		for( x=0; x<image->w; x++ )
		{
			unsigned char outval = 0;
			
			int bitsy = 0;
			for( bitsy=0; bitsy<8; bitsy++)
			{
				int rofs = (((y+bitsy)*image->w)+x)*4;
				unsigned char b = pixels[ rofs+0 ];
				unsigned char g = pixels[ rofs+1 ];
				unsigned char r = pixels[ rofs+2 ];
				
				if(r > 127)
				{
					outval |= 1<<bitsy;
				}
			}
			
			fprintf( f, "0x%02x,", outval );
		}
		fprintf( f, "\n" );
	}
	
	*_pTotalOutputSize += image->w*(image->h/8);
	
	fprintf( f, "};\n\n" );
}

void writeHeaderFile( FILE* f, char* _symbolNameBase, SDL_Surface* _image )
{
	fprintf( f, "#ifndef %s_data_h\n", _symbolNameBase );
	fprintf( f, "#define %s_data_h\n", _symbolNameBase );
	fprintf( f, "\n" );
	fprintf( f, "#include <avr/pgmspace.h>\n" );
	fprintf( f, "\n" );
	fprintf( f, "extern \"C\" const unsigned char PROGMEM %s_tilebank[];\n", _symbolNameBase );
	fprintf( f, "\n" );
	fprintf( f, "#endif // %s_data_h\n", _symbolNameBase );
}

SDL_Surface* LoadImage( char* _fileName )
{
	SDL_Surface* image = IMG_Load( _fileName );
	bool isAlpha = SDL_ISPIXELFORMAT_ALPHA( image->format->format );
	bool isIndexed = SDL_ISPIXELFORMAT_INDEXED( image->format->format );
	
	return image;
}

FILE* openOutfileC( char* _baseOutFileName )
{
	char outname_c[ 2048 ];
	sprintf( outname_c, "%s.cpp", _baseOutFileName );
	FILE* f = fopen( outname_c, "w" );
	
	return f;
}

FILE* openOutfileH( char* _baseOutFileName )
{
	char outname_c[ 2048 ];
	sprintf( outname_c, "%s.h", _baseOutFileName );
	FILE* f = fopen( outname_c, "w" );
	
	return f;
}

int main( int _numargs, char** _apszArgh )
{
	if( _numargs != 4 )
	{
		printf("Usage error: Program need 3 arguments:\n");
		printf("  png2sh1106_tilebank <in_file.png> <out_file_base> <symbol_name>\n");
		return -1;
	}

	char* pszInFileName = _apszArgh[ 1 ];
	char* pszOutFilenameBase = _apszArgh[ 2 ];
	char* pszSymbolNameBase = _apszArgh[ 3 ];
	
	//
	SDL_Surface* image = LoadImage( pszInFileName );

	//
	// Write cpp file
	//
	FILE* f = openOutfileC( pszOutFilenameBase );

	int totalOutputSize = 0;
	writeHeader( f, pszSymbolNameBase, image );
	writePixels( f, pszSymbolNameBase, image, &totalOutputSize );
	
	fclose( f );
	
	
	//
	// Write h file
	//
	f = openOutfileH( pszOutFilenameBase );
	writeHeaderFile( f, pszSymbolNameBase, image );
	fclose( f );

	printf("Total output size: %i\n", totalOutputSize );
	return 0;
}
