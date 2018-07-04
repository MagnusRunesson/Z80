void dataBusWrite( unsigned char _value )
{
  Serial.write( "Setting data bus to value: 0x" );
  serialWriteHex( _value );
  Serial.write( " (" );
  
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PIN_DATA_BASE + i;
    
    pinMode( pin, OUTPUT );
    int b = (_value >> i) & 1;
    if(b) {
      digitalWrite( pin, HIGH );
      Serial.write( "1" );
    } else {
      digitalWrite( pin, LOW );
      Serial.write( "0" );
    }
  }

  Serial.write( "b)\n" );
}

void dataBusWriteMenu( unsigned char* _pszInput )
{
  char* pszData = (char*)&_pszInput[ 2 ];
  Serial.write( "Input string: " );
  Serial.write( pszData );
  Serial.write( "\n" );
  int data = atoi( pszData );
  dataBusWrite( data );
}

int dataBusRead()
{
  int readByte = 0;
  Serial.write( "Data bus read: " );
  
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PIN_DATA_BASE + i;
    pinMode( pin, INPUT );
    if( digitalRead( pin ) == HIGH )
    {
      Serial.write( "1" );
      readByte |= 1 << i;
    } else
    {
      Serial.write( "0" );
    }
  }

  Serial.write( " - (" );
  serialWriteHex( readByte );
  Serial.write( ")\n" );


  return readByte;
}

void dataBusReadMenu( unsigned char* _pszInput )
{
  int data = dataBusRead();
  Serial.write( "menu: 0x" );
  serialWriteHex( data );
  Serial.write( "\n" );
}

