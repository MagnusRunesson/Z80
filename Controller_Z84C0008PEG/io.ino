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

