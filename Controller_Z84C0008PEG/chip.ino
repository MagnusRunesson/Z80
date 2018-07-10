void chipSetup()
{
  int i;

  //
  // Pins that are input (from Z80 to Arduino)
  //
  for( i=0; i<16; i++ ) {
    int pin = PIN_ADDRESS[ i ];
    pinMode(pin, INPUT);
  }
  pinMode(PIN_HALT, INPUT);
  pinMode(PIN_MREQ, INPUT);
  pinMode(PIN_IOREQ, INPUT);
  pinMode(PIN_RFSH, INPUT);
  pinMode(PIN_M1, INPUT);
  pinMode(PIN_BUSACK, INPUT);
  pinMode(PIN_WR, INPUT);
  pinMode(PIN_RD, INPUT);

  //
  // Pins that are output (from Arduino to Z80)
  //
  pinMode(PIN_CLK, OUTPUT);
  digitalWrite(PIN_CLK, LOW);
  pinMode(PIN_WAIT, OUTPUT);
  digitalWrite(PIN_WAIT, HIGH);
  pinMode(PIN_INT, OUTPUT);
  digitalWrite(PIN_INT, HIGH);
  pinMode(PIN_NMI, OUTPUT);
  digitalWrite(PIN_NMI, HIGH);
  pinMode(PIN_RESET, OUTPUT);
  digitalWrite(PIN_RESET, HIGH);
  pinMode(PIN_BUSREQ, OUTPUT);
  digitalWrite(PIN_BUSREQ, HIGH);

  //
  // The only pins left are the data pins and they can go both ways, so I do not initialize them beforehand
  //
}

void chipPrintPin( int _pin, bool _bCompact = false )
{
  if( digitalRead( _pin ) == HIGH )
  {
    Serial.write( "H" );
    if( !_bCompact )
    {
      Serial.write( "igh" );
    }
  } else {
    Serial.write( "L" );
    if( !_bCompact )
    {
      Serial.write( "ow" );
    }
  }
}

const int CHIPMASK_ACTIVE_M1         = 1<<0;
const int CHIPMASK_ACTIVE_WR         = 1<<1;
const int CHIPMASK_ACTIVE_RD         = 1<<2;
const int CHIPMASK_ACTIVE_HALT       = 1<<3;
const int CHIPMASK_ACTIVE_MREQ       = 1<<4;
const int CHIPMASK_ACTIVE_IOREQ      = 1<<5;
const int CHIPMASK_ACTIVE_RFSH       = 1<<6;
const int CHIPMASK_ACTIVE_BUSACK     = 1<<7;

int chipGetStatusMask()
{
  int ret = 0;
  if( digitalRead( PIN_M1 ) == LOW ) ret |= CHIPMASK_ACTIVE_M1;
  if( digitalRead( PIN_WR ) == LOW ) ret |= CHIPMASK_ACTIVE_WR;
  if( digitalRead( PIN_RD ) == LOW ) ret |= CHIPMASK_ACTIVE_RD;
  if( digitalRead( PIN_HALT ) == LOW ) ret |= CHIPMASK_ACTIVE_HALT;
  if( digitalRead( PIN_MREQ ) == LOW ) ret |= CHIPMASK_ACTIVE_MREQ;
  if( digitalRead( PIN_IOREQ ) == LOW ) ret |= CHIPMASK_ACTIVE_IOREQ;
  if( digitalRead( PIN_RFSH ) == LOW ) ret |= CHIPMASK_ACTIVE_RFSH;
  if( digitalRead( PIN_BUSACK ) == LOW ) ret |= CHIPMASK_ACTIVE_BUSACK;
  return ret;
}

void chipPrintStuff( bool _bCompact )
{
  Serial.write( "     M1: " ); chipPrintPin( PIN_M1, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "     WR: " ); chipPrintPin( PIN_WR, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "     RD: " ); chipPrintPin( PIN_RD, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "   HALT: " ); chipPrintPin( PIN_HALT, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "   MREQ: " ); chipPrintPin( PIN_MREQ, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "  IOREQ: " ); chipPrintPin( PIN_IOREQ, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( "   RFSH: " ); chipPrintPin( PIN_RFSH, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( " BUSACK: " ); chipPrintPin( PIN_BUSACK, _bCompact ); if( !_bCompact ) { Serial.write( "\n" ); }
  Serial.write( " " );
  
  addressBusPrint( true, !_bCompact );
}

void chipClockPulse()
{
  digitalWrite(PIN_CLK, HIGH);
  delay(1);
  digitalWrite(PIN_CLK, LOW);
  delay(1);
}

void chipPrintStuffMenu( unsigned char* _ )
{
  chipPrintStuff( false );
}

void chipClockPulseMenu( unsigned char* _ )
{
   chipClockPulse();
}

void chipResetMenu( unsigned char* _ )
{
  chipReset();
}

void chipReset()
{
  digitalWrite(PIN_RESET, LOW);
  chipClockPulse();
  chipClockPulse();
  chipClockPulse();
  chipClockPulse();
  digitalWrite(PIN_RESET, HIGH);
}

