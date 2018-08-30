
#include "sh1106_shared.h"

typedef unsigned char uint8;
int pixel_x, pixel_y;
int move_x, move_y;
int testfestIndex = 0;

int waitCycles;

int frame;

uint8 screen[ 128 * 64 / 8 ];

void display_init() {
  i2c_start(ADDR);
  i2c_send(CONTROL_SENDCOMMAND); //................ control byte
  i2c_send(0xAE); //................ displayOff
  i2c_send(0x8D); //................   *
  i2c_send(0xD5); i2c_send(0x80); // clockFrequency
  i2c_send(0x32); //................ pumpVoltage0123
  i2c_send(0xA1); //................ segmentRemap
  i2c_send(0xC8); //................ flipVertically
  i2c_send(0xDA); i2c_send(0x12); // comConfiguration
  i2c_send(0xA8); i2c_send(0x3F); // multiplex
  i2c_send(0xD3); i2c_send(0x00); // displayOffest
  i2c_send(0x10); i2c_send(0x00); // columnAddr
  i2c_send(0x40); //................ startLine
  i2c_send(0xB0); //................ pageAddr
  i2c_send(0xAF); //................ displayOn
  i2c_stop();
}


void i2c_sendcommand(int _cmd)
{
  i2c_start(ADDR);
  i2c_send(CONTROL_SENDCOMMAND);
  i2c_send(_cmd);
  i2c_stop();
}

void blit()
{
  int page;
  
  for( page=0; page<8; page++ )
  {
    i2c_start(ADDR);
    i2c_send(CONTROL_SENDCOMMAND); //................ control byte
    i2c_send(0xB0 + page); //................ pageAddr
    i2c_stop();

    i2c_start(ADDR);
    i2c_send(CONTROL_SENDDATA);

    // Left side off screen
    i2c_send( 0 );
    i2c_send( 0 );

    uint8* screenBase = ((uint8*)screen) + (page * 128);

    int x;
    const int columnWidth = 8;  // Read/write this many bits at a time. must match the datatype that is written (8 for unsigned byte, 32 for unsigned int)
    const int numRowsPerPage = 8; // This can't be changed because it is decided by the SH1106 chip that each page contain 8 rows (each bit in a byte is one row, so yeah it's pretty hard wired)
    for( x=0; x<128 / columnWidth; x++ )
    {
      uint8 r[ numRowsPerPage ]; // This data type must be able to contain the columnWidth number of bits
      int row;
      for( row=0; row<numRowsPerPage; row++ )
      {
        r[ row ] = screenBase[( 128 * row ) / columnWidth ];
      }

      int column;
      for( column=0; column<columnWidth; column++ )
      {
        uint8 c = 0;
        
        for( row=0; row<numRowsPerPage; row++ )
        {
          c += ((r[ row ] >> column) & 1) << row;
        }

        i2c_send( c );
      }
      
      screenBase++;
    }

    // Right side off screen
    i2c_send( 0 );
    i2c_send( 0 );
    
    i2c_stop();
  }
}

void blit_image( const unsigned char* _pImage )
{
  int page;

/*
  i2c_sendcommand(0x40 + frame); //................ startLine
  frame++;
  if( frame >= 64 )
  {
    frame -= 64;
  }
  delay(20);
*/



  for( page=0; page<8; page++ )
  {
    i2c_start(ADDR);
    i2c_send(CONTROL_SENDCOMMAND); //................ control byte
    i2c_send(0xB0 + page); //................ pageAddr
    i2c_stop();

    i2c_start(ADDR);
    i2c_send(CONTROL_SENDDATA);

    // Left side off screen
    i2c_send( 0 );
    i2c_send( 0 );

    int x;
    for( x=0; x<128; x++ )
    {
      unsigned char v = pgm_read_byte_near( _pImage + (page*128) + x );
      //unsigned char v = _pImage[ (page*128) + x ];
      i2c_send( v );
    }

    // Right side off screen
    i2c_send( 0 );
    i2c_send( 0 );
    
    i2c_stop();
  }
}


void testfest()
{
  testfestIndex++;
  if( testfestIndex >= 256 ) {
    testfestIndex -= 256;
  }
  
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
      i2c_send(( testfestIndex + i) & 255 );
    }

    i2c_stop();
  }
}

void clearScreen()
{
  int i;
  for( i=0; i<128*64/8; i++ )
  {
    uint8 v = 0;
    if( i < 128 )
    {
      v = 0xff;
    }
    screen[ i ] = v;
  }
}

void setPixel( int _x, int _y, int _val )
{
  int wrofs = (_y * 128) + _x;  // Get bit index in screen array
  wrofs >>= 3;  // Get byte index in screen array
  int wrbit = _x & 7;

  if( _val == 0 )
  {
    int wrmask = ~(1 << wrbit);
    screen[ wrofs ] &= wrmask;
  } else if( _val == 1 )
  {
    int wrmask = 1 << wrbit;
    screen[ wrofs ] |= wrmask;
  }
  else if( _val == 2 )
  {
    int wrmask = 1 << wrbit;
    screen[ wrofs ] ^= wrmask;
  }
}

void updatePixel()
{
  setPixel( pixel_x, pixel_y, 2 );
  
  if( move_x > 0 )
  {
    pixel_x++;
    if( pixel_x >= 127 )
    {
      move_x = -1;
    }
  } else
  {
    pixel_x--;
    if( pixel_x <= 0 )
    {
      move_x = 1;
    }
  }

  if( move_y > 0 )
  {
    pixel_y++;
    if( pixel_y >= 63 )
    {
      move_y = -1;
    }
  } else
  {
    pixel_y--;
    if( pixel_y <= 0 )
    {
      move_y = 1;
    }
  }
}

void setup() {

  // Initialization
  Serial.begin(57600);
  i2c_init();
  display_init();

  pixel_x = 12;
  pixel_y = 7;
  move_x = 1;
  move_y = 1;

  frame = 0;
}

#include "test0.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "test4.h"
#include "tilemap_dungeon_8x8.h"

void loop() 
{
  waitCycles = 0;
  //clearScreen();
  //updatePixel();
  blit_image( tilemap_dungeon_8x8_pixels );
  //testfest();

  /*
  Serial.write("Spent ");
  serialWriteInt(waitCycles);
  Serial.write(" cycles waiting for i2c\n");
  */
}

