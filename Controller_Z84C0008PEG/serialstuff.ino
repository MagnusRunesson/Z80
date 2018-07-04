const int MAX_INPUTMESSAGE_LENGTH = 128;

unsigned char inputMessage[ MAX_INPUTMESSAGE_LENGTH ];

void serialWriteInt( int _o )
{
  char itoab[ 32 ];
  itoa( _o, itoab, 10 );
  Serial.write( itoab );
}

void serialWriteHex( int _o )
{
  char itoab[ 9 ];
  itoa( _o, itoab, 16 );
  Serial.write( itoab );
}

void serialWriteBool( bool _value )
{
  Serial.write(_value?"True":"False");
}

int inputWriteOffset;

void resetRead()
{
  inputWriteOffset = 0;
}
void serialSetup()
{
  Serial.begin(9600);
  resetRead();
}

unsigned char* serialLoop()
{
  unsigned char* pszRet = NULL;
  //Serial.write( "Is reading: " + isReading ? "Yes\n":"No\n" );
  if( Serial.available())
  {
    /*
    Serial.write("Frame: ");
    char itoaBuffer[ 32 ];
    itoa(frameCount, itoaBuffer, 10 );
    Serial.write( itoaBuffer );
    Serial.write( ": " );
    Serial.write("Serial available\n");
    */

    // Copy to message parameter
    unsigned char byteRead = Serial.read();
    
    if( byteRead == 10 )
    {
      // New line
      inputMessage[ inputWriteOffset ] = 0;
      resetRead();
      pszRet = inputMessage;
    } else {
      inputMessage[ inputWriteOffset++ ] = byteRead;
    }
    /*
    Serial.write( byteRead );
    itoa( byteRead, itoaBuffer, 10 );
    Serial.write( ": '" );
    Serial.write( itoaBuffer );
    Serial.write( "'\n" );
    */

  }
  return pszRet;
}

