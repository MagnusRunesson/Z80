const int PIN_OUT = 22;
const int PIN_IN = 52;

void setup() {
  Serial.begin( 57600 );

  int signalWrite = LOW;
  int expectedSignalRead = HIGH;

  // Write to pin
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, signalWrite );

  // Debug write
  if( signalWrite == LOW )
  {
    Serial.write( "Wrote LOW\n" );
  }
  else
  {
    Serial.write( "Wrote HIGH\n" );
  }

  // Wait for .. something?
  delay( 100 );

  // Read from pin
  pinMode( PIN_IN, INPUT );
  int d = digitalRead( PIN_IN );
  if( d == HIGH )
  {
    Serial.write( "HIGH\n" );
  }
  else
  {
    Serial.write( "LOW\n" );
  }

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
