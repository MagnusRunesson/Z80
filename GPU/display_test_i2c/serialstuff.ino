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

