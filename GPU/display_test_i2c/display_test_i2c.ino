#include <util/twi.h>

#define ADDR  0x78

#define I2C_ERR_NONE 0x00
#define I2C_ERR_TIMEOUT 0x01
#define I2C_ERR_BUS 0x02

typedef unsigned char uint8;
int pixel_x, pixel_y;
int move_x, move_y;
int testfestIndex = 0;

uint8 screen[ 128 * 64 / 8 ];

void i2c_set_error(uint8_t code, uint8_t pos)
{
  Serial.write("I2C error code. Code: " );
  serialWriteInt( code );
  Serial.write(" - Pos: " );
  serialWriteInt( pos );
  Serial.write("\n");
}

void i2c_init()
{
  /*
  TWBR: bit rate register
  TWSR: status register (contains preselector bits)

  prescalar
    0    1
    1   4
    2   16
    3   64

  f = F_CPU/(16+2*TWBR*prescalar)
  
  F_CPU = 16MHz
    TWBR = 152;
    TWSR = 0;
  --> 50KHz

    TWBR = 72;
    TWSR = 0;
  --> 100KHz

    TWBR = 12;
    TWSR = 0;
  --> 400KHz

    F_CPU/(2*100000)-8  --> calculate TWBR value for 100KHz
*/
  TWSR = 0;
  TWBR = F_CPU/(2*600000)-8;
}

uint8_t i2c_wait(uint8_t mask, uint8_t pos)
{
  uint32_t cnt = 6000; /* timout value should be > 280 for 50KHz Bus and 16 Mhz CPU, however the start condition might need longer */
  while( !(TWCR & mask) )
  {
    if ( cnt == 0 )
    {
      i2c_set_error(I2C_ERR_TIMEOUT, pos);
      return 0; /* error */
    }
    cnt--;
  }
  
  return 1; /* all ok */
}

/* sla includes all 8 bits (with r/w bit), assums master transmit */
uint8_t i2c_start(uint8_t sla)
{
  register uint8_t status;
  
  /* send start */
  TWCR = _BV(TWINT) |  _BV(TWSTA)  |  _BV(TWEN);
   
  /* wait */
  if ( i2c_wait(_BV(TWINT), 1) == 0 )
    return 0;
  
  status = TW_STATUS;
 
  /* check status after start */  
  if ( status != TW_START && status != TW_REP_START )
  {
    i2c_set_error(I2C_ERR_BUS, 1);
    return 0;
  }

  /* set slave address */  
  TWDR = sla;
  
  /* enable sla transfer */
  TWCR = _BV(TWINT)  |  _BV(TWEN);

  /* wait */
  if ( i2c_wait(_BV(TWINT), 2) == 0 )
    return 0;

  status = TW_STATUS;
  /* check status after sla */  
  if ( status != TW_MT_SLA_ACK )
  {
    i2c_set_error(I2C_ERR_BUS, 2);
    return 0;
  }

   return 1;
}

void i2c_stop(void)
{
  /* write stop */
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

  /* no error is checked for the stop condition */  
  i2c_wait(_BV(TWSTO), 4);
}

uint8_t i2c_send(uint8_t data)
{
  register uint8_t status;
  TWDR = data;
  TWCR = _BV(TWINT)  |  _BV(TWEN);
  if ( i2c_wait(_BV(TWINT), 3) == 0 )
    return 0;

  status = TW_STATUS;
  if ( status != TW_MT_DATA_ACK )
  {
    i2c_set_error(I2C_ERR_BUS, 3);
    return 0;
  }

  return 1;  
}

/*
uint8_t i2c_read(bool isLastByte) {

  if (isLastByte) TWCR = (1 << TWINT) | (1 << TWEN);
  else TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}
*/

void display_init() {
  i2c_start(ADDR);
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
  i2c_start(ADDR);
  i2c_send(0x80);
  i2c_send(_cmd);
  i2c_stop();
}

void blit()
{
  int page;
  
  for( page=0; page<8; page++ )
  {
    i2c_start(ADDR);
    i2c_send(0x00); //................ control byte
    i2c_send(0xB0 + page); //................ pageAddr
    i2c_stop();

    i2c_start(ADDR);
    i2c_send(0x40);

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
    i2c_send(0x00); //................ control byte
    i2c_send(0xB0 + j); //................ pageAddr
    i2c_stop();

    i2c_start(ADDR);
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
}

void loop() 
{
  clearScreen();
  updatePixel();
  blit();
  //testfest();
}

