const int PIN_OUT_A = 26;
const int PIN_OUT_B = 27;
const int PIN_IN = 53;

void setup() {
  Serial.begin( 57600 );

  int signalWrite_A = HIGH;
  int signalWrite_B = HIGH;
  int expectedSignalRead = HIGH;

  // Write to pin
  pinMode( PIN_OUT_A, OUTPUT );
  digitalWrite( PIN_OUT_A, signalWrite_A );
  pinMode( PIN_OUT_B, OUTPUT );
  digitalWrite( PIN_OUT_B, signalWrite_B );

  // Debug write
  Serial.write( "Wrote " );
  printHighLow( signalWrite_A );
  Serial.write( " to A\n" );

  Serial.write( "Wrote " );
  printHighLow( signalWrite_B );
  Serial.write( " to B\n" );

  // Wait for .. something?
  delay( 100 );

  // Read from pin
  pinMode( PIN_IN, INPUT );
  int d = digitalRead( PIN_IN );
  Serial.write( "Read " );
  printHighLow( d );
  Serial.write( " from input\n" );

  if( d == expectedSignalRead )
  {
    Serial.write( "Test success!\n" );
  }
  else
  {
    Serial.write( "Test fail!\n" );
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printHighLow( int _value )
{
  if( _value == HIGH )
  {
    Serial.write( "HIGH" );
  }
  else
  {
    Serial.write( "LOW" );
  }
}

