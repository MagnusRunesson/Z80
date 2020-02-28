/*
 * EEPROM   Arduino
 * 1        23
 * 2        25
 * 3        27
 * 4        29
 * 5        31
 * 6        33
 * 7        35
 * 8        37
 * 9        39
 * 10       41
 * 11       43
 * 12       45
 * 13       47
 * 14       Gnd
 * 
 * 15       46
 * 16       44
 * 17       42
 * 18       40
 * 19       38
 * 20       36
 * 21       34
 * 22       32
 * 23       30
 * 24       28
 * 25       26
 * 26       24
 * 27       22
 * 28       +5V
 */
 
const int PINS_ADDRESS[ 15 ] = {
  /* A0  - EEPROM 10 */   41,
  /* A1  - EEPROM 9  */   39,
  /* A2  - EEPROM 8  */   37,
  /* A3  - EEPROM 7  */   35,
  /* A4  - EEPROM 6  */   33,
  /* A5  - EEPROM 5  */   31,
  /* A6  - EEPROM 4  */   29,
  /* A7  - EEPROM 3  */   27,
  /* A8  - EEPROM 25 */   26,
  /* A9  - EEPROM 24 */   28,
  /* A10 - EEPROM 21 */   34,
  /* A11 - EEPROM 23 */   30,
  /* A12 - EEPROM 2  */   25,
  /* A13 - EEPROM 26 */   24,
  /* A14 - EEPROM 1  */   23
};

const int PINS_DATA[ 8 ] = {
  /* D0  - EEPROM 11 */   43,
  /* D1  - EEPROM 12 */   45,
  /* D2  - EEPROM 13 */   47,
  /* D3  - EEPROM 15 */   46,
  /* D4  - EEPROM 16 */   44,
  /* D5  - EEPROM 17 */   42,
  /* D6  - EEPROM 18 */   40,
  /* D7  - EEPROM 19 */   38,
};

const int PIN_WE        = 22; /* EEPROM 27 */
const int PIN_OE        = 32; /* EEPROM 22 */
const int PIN_CE        = 36; /* EEPROM 20 */

// These delays are copied from the AT28C256 datasheet
const int DELAY_tWC       = 12;   // Write Cycle Time
const int DELAY_tDS       = 52;   // Data setup time
const int DELAY_tDH       = 100;  // Data hold time

void addressSetBus( int _address, bool _bPrint )
{
  if( _bPrint )
  {
    Serial.write( "Setting address bus to 0x" );
    serialWriteHex( _address );
    Serial.write( " - " );
    serialWriteInt( _address );
    Serial.write( " - 0b" );
  }
  
  int i;
  for( i=0; i<15; i++ )
  {
    int pin = PINS_ADDRESS[ i ];
    int bit = 1 << i;
    
    pinMode( pin, OUTPUT );
    if( _address & bit ) {
      if( _bPrint )
      {
        Serial.write( "0" );
      }
      digitalWrite( pin, HIGH );
    } else {
      if( _bPrint )
      {
        Serial.write( "1" );
      }
      digitalWrite( pin, LOW );
    }
  }
  
  if( _bPrint )
  {
    Serial.write( "\n" );
  }
}

void dataSetBus( int _value, bool _bPrint )
{
  if( _bPrint )
  {
    Serial.write( "Setting data bus to 0x" );
    serialWriteHex( _value );
    Serial.write( " - " );
    serialWriteInt( _value );
    Serial.write( " - 0b" );
  }
    
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PINS_DATA[ i ];
    int bit = 1 << i;
    
    pinMode( pin, OUTPUT );
    if( _value & bit )
    {
      if( _bPrint )
      {
        Serial.write( "0" );
      }
      digitalWrite( pin, HIGH );
    } else {
      if( _bPrint )
      {
        Serial.write( "1" );
      }
      digitalWrite( pin, LOW );
    }
  }

  if( _bPrint )
  {
    Serial.write( "\n" );
  }
}

int dataReadBus( bool _bPrint )
{
  if( _bPrint )
  {
    Serial.write( "Read from data bus - 0b" );
  }

  int retValue = 0;
  
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PINS_DATA[ i ];
    int bit = 1 << i;
    
    pinMode( pin, INPUT );
    int value = (digitalRead( pin ) == HIGH) ? 1 : 0;
    if( _bPrint )
    {
      serialWriteInt( value );
    }

    retValue |= (value << i);
  }

  if( _bPrint )
  {
    Serial.write( " - 0x" );
    serialWriteHex( retValue );
    Serial.write( " - " );
    serialWriteInt( retValue );
    Serial.write( "\n" );
  }

  return retValue;
}

int chipRead( int _address )
{
  addressSetBus( _address, false );

  digitalWrite( PIN_OE, LOW );
  digitalWrite( PIN_CE, LOW );

  delay( DELAY_tDS );
  
  int ret = dataReadBus( false );
  
  digitalWrite( PIN_CE, HIGH );
  digitalWrite( PIN_OE, HIGH );
  
  return ret;
}

void chipWrite( int _address, int _value )
{
  addressSetBus( _address, false );
  dataSetBus( _value, false );

  digitalWrite( PIN_WE, LOW );
  digitalWrite( PIN_CE, LOW );
  
  delay( DELAY_tWC );
  
  digitalWrite( PIN_CE, HIGH );
  digitalWrite( PIN_WE, HIGH );

  delay(DELAY_tDH);
}

void flashTest()
{
  int i;
  for( i=0; i<128; i++ )
  {
    int val = chipRead( i );
    Serial.write( "Address 0x" );
    serialWriteHex( i );
    Serial.write( ": 0x" );
    serialWriteHex( val );
    Serial.write( "\n" );
    chipWrite( i, i );
  }
}

void flashNOP( int _num )
{
  int i;
  for( i=0; i<_num; i++ )
  {
    int val = chipRead( i );
    Serial.write( "Address 0x" );
    serialWriteHex( i );
    Serial.write( ": 0x" );
    serialWriteHex( val );
    Serial.write( "\n" );
    chipWrite( i, 0 );
    while( chipRead( i ) != 0 )
    {
      Serial.write("Waiting for write\n");
      delay( 2 );
    }
  }
}

#include "rom_image.h"

void flashROMImage()
{
  int i;
  Serial.write( "rom image size: " );
  serialWriteInt( rom_image_size );
  Serial.write( " bytes\n" );

  for( i=0; i<rom_image_size; i++ )
  {
    /*
    if(( i % 10 ) == 9 )
    {
      serialWriteInt( i );
      Serial.write( " bytes written\n" );
    }*/
    int val = chipRead( i );
    Serial.write( "Address 0x" );
    serialWriteHex( i );
    Serial.write( " was: 0x" );
    serialWriteHex( val );
    chipWrite( i, rom_image[ i ]);
    while( chipRead( i ) != rom_image[ i ])
    {
      delay(5);
    Serial.write( " waiting!! " );
    }
    
    Serial.write( "  - writing: 0x" );
    serialWriteHex( rom_image[ i ]);
    Serial.write( "\n" );
  }

  serialWriteInt( i );
  Serial.write( " bytes written - Verify start\n" );

  for( i=0; i<rom_image_size; i++ )
  {
    if(( i % 10 ) == 9 )
    {
      serialWriteInt( i );
      Serial.write( " bytes verified\n" );
    }
    
    int d = chipRead( i );
    if( d != rom_image[ i ])
    {
      Serial.write( "ERROR at address 0x" );
      serialWriteHex( i );
      Serial.write( " - expected " );
      serialWriteInt( rom_image[ i ]);
      Serial.write( ", read " );
      serialWriteInt( d );
      Serial.write( "\n" );
    }
  }
}

void flashRead( int _length )
{
  int i;
  Serial.write( "Reading " );
  serialWriteInt( _length );
  Serial.write( " bytes\n" );

  for( i=0; i<_length; i++ )
  {
    int val = chipRead( i );
    Serial.write( "Address 0x" );
    serialWriteHex( i );
    Serial.write( ": 0x" );
    serialWriteHex( val );
    Serial.write( "\n" );
  }
}

void setup() {
  Serial.begin(57600);
  pinMode( PIN_CE, OUTPUT ); digitalWrite( PIN_CE, HIGH );
  pinMode( PIN_WE, OUTPUT ); digitalWrite( PIN_WE, HIGH );
  pinMode( PIN_OE, OUTPUT ); digitalWrite( PIN_OE, HIGH );


  flashROMImage();
  //flashNOP( 256 );
  
  delay( 300 );
  
  flashRead( 100 );
  
  //chipWrite( 0, 32 );
  //chipRead( 0 );
}

void loop() {
  // put your main code here, to run repeatedly:

}
