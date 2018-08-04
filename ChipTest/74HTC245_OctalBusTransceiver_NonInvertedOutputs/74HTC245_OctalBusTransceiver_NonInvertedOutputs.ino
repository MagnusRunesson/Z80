const int PIN_DIR = 35;
const int PIN_OE = 34;
const int PIN_A = 36;
const int PIN_B = 37;

void setup() {
  Serial.begin( 57600 );

  // Dir - LOW: A=B    HIGH: B=A
  int signalWrite_DIR = HIGH;
  int signalWrite_OE = LOW;
  int signalWrite_A = HIGH;
  int expectedSignalRead_B = HIGH;

  // Write to pin
  pinMode( PIN_DIR, OUTPUT );
  digitalWrite( PIN_DIR, signalWrite_DIR );
  pinMode( PIN_OE, OUTPUT );
  digitalWrite( PIN_OE, signalWrite_OE );
  pinMode( PIN_A, OUTPUT );
  digitalWrite( PIN_A, signalWrite_A );

  // Debug write
  Serial.write( "Wrote " );
  printHighLow( signalWrite_DIR );
  Serial.write( " to DIR\n" );

  Serial.write( "Wrote " );
  printHighLow( signalWrite_OE );
  Serial.write( " to OE\n" );

  Serial.write( "Wrote " );
  printHighLow( signalWrite_A );
  Serial.write( " to A\n" );

  // Wait for .. something?
  delay( 100 );

  // Read from pin
  pinMode( PIN_B, INPUT );
  int d = digitalRead( PIN_B );
  Serial.write( "Read " );
  printHighLow( d );
  Serial.write( " from B\n" );

  if( d == expectedSignalRead_B )
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

