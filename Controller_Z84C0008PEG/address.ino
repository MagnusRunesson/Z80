int addressBusRead()
{
  int ret = 0;
  int i;
  
  for( i=0; i<16; i++ )
  {
    if( digitalRead( PIN_ADDRESS_BASE + i ) == HIGH )
    {
      ret += 1<<i;
    }
  }

  return ret;
}

void addressBusPrint()
{
  int address = addressBusRead();
  Serial.write( "Address bus: 0x" );
  serialWriteHex( address );
  Serial.write( " - (" );
  int i;
  for( i=16; i>0; i-- )
  {
    int b = (address >> (i-1)) & 1;
    if(b)
    {
      Serial.write( "1" );
    } else {
       Serial.write( "0" );
    }
  }

  Serial.write( ")\n" );
}

void addressBusPrintMenu( unsigned char* _ )
{
  addressBusPrint();
}

