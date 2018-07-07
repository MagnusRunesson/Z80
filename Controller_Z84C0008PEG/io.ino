void ioReadPartyMenu( unsigned char* _pszInput )
{
  ioReadFromAddressWriteToData( first );
  chipClockPulse();
}

void ioReadFromAddressWriteToData( const unsigned char* _pMemory )
{
  int address = addressBusRead();
  Serial.write( "Address: 0x" );
  serialWriteHex( address );
  Serial.write( "\n" );
  
  int data = _pMemory[ address ];
  Serial.write( "Data: 0x" );
  serialWriteHex( data );
  Serial.write( "\n" );

  dataBusWrite( data );
}

void ioWritePartyMenu( unsigned char* _pszInput )
{
  /*
  int maxRead = strlen( _pszInput );
  char address[ 6 ];
  char value[ 4 ];

  // Copy address first
  maxRead -= 2;
  char* pszInput = (char*)&_pszInput[ 2 ];
  char* pszAddress = (char*)&address[ 0 ];
  while(pszInput != ' ')
  {
    *pszAddress++ = *pszInput++;
    maxRead--;
    if( maxRead < 0 )
    {
      // Fail safe!
      return;
    }
  }

  *pszAddress = 0;
  Serial.write("Address: ");
  Serial.write( address );
  Serial.write( "\n" );
  */

  /*
  ioReadFromAddressWriteToData( first );
  chipClockPulse();
  */

  ioWriteFromDataToAddress( RAM );
}

void ioWriteFromDataToAddress( unsigned char* _pMemory )
{
  int address = addressBusRead();
  Serial.write( "Writing to address: 0x" );
  serialWriteHex( address );
  Serial.write( "\n" );
  
  int data = dataBusRead();
  _pMemory[ address ] = data;
  Serial.write( "Data: 0x" );
  serialWriteHex( data );
  Serial.write( "\n" );
}

