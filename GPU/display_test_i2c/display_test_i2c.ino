#define ADDR  0x78

typedef unsigned char uint8;
int pixel_x, pixel_y;
int move_x, move_y;

uint8 screen[ 128 * 64 / 8 ];

void i2c_start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_send(uint8_t data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

}

uint8_t i2c_read(bool isLastByte) {

  if (isLastByte) TWCR = (1 << TWINT) | (1 << TWEN);
  else TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}


void display_init() {
  i2c_start();
  i2c_send(ADDR);
  i2c_send(0x00); //................ control byte
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
  i2c_start();
  i2c_send(ADDR);
  i2c_send(0x80);
  i2c_send(_cmd);
  i2c_stop();
}

void blit()
{
  int page;
  
  for( page=0; page<8; page++ )
  {
    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x00); //................ control byte
    i2c_send(0xB0 + page); //................ pageAddr
    i2c_stop();

    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x40);

    // Left side off screen
    i2c_send( 0 );
    i2c_send( 0 );

    uint8* screenBase = ((uint8*)screen) + (page * 128);

    int x;
    for( x=0; x<128 / 8; x++ )
    {
      uint8 r[ 8 ];
      int row;
      for( row=0; row<8; row++ )
      {
        r[ row ] = screenBase[( 128 * row ) / 8 ];
      }

      int column;
      for( column=0; column<8; column++ )
      {
        uint8 c = 0;
        
        for( row=0; row<8; row++ )
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
  }
}

int testfestIndex = 0;

void testfest()
{
  testfestIndex++;
  if( testfestIndex >= 8 ) {
    testfestIndex -= 8;
  }
  
  int j;
  for( j=0; j<8; j++ )
  {
    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x00); //................ control byte
    i2c_send(0xB0 + j); //................ pageAddr
    i2c_stop();

    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x40);

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

void setup() {

  // Initialization
  Serial.begin(57600);
  display_init();

  pixel_x = 12;
  pixel_y = 7;
  move_x = 1;
  move_y = 1;
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

void loop() 
{
  clearScreen();
  updatePixel();
  blit();
  //testfest();
}

