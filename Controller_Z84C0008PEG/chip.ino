void chipSetup()
{
  int i;

  //
  // Pins that are input (from Z80 to Arduino)
  //
  for( i=0; i<16; i++ ) {
    pinMode(PIN_ADDRESS_BASE + i, INPUT);
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
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, LOW);

  //
  // The only pins left are the data pins and they can go both ways, so I do not initialize them beforehand
  //
}

void chipPrintPin( int _pin )
{
  bool high = digitalRead( _pin ) == HIGH;
  Serial.write(high?"High":"Low");
}

void chipPrintStuff()
{
  Serial.write("    M1: "); chipPrintPin( PIN_M1 ); Serial.write("\n");
  Serial.write("    WR: "); chipPrintPin( PIN_WR ); Serial.write("\n");
  Serial.write("    RD: "); chipPrintPin( PIN_RD ); Serial.write("\n");
  Serial.write("  HALT: "); chipPrintPin( PIN_HALT ); Serial.write("\n");
  Serial.write("  MREQ: "); chipPrintPin( PIN_MREQ ); Serial.write("\n");
  Serial.write(" IOREQ: "); chipPrintPin( PIN_IOREQ ); Serial.write("\n");
  Serial.write("  RFSH: "); chipPrintPin( PIN_RFSH ); Serial.write("\n");
  Serial.write("BUSACK: "); chipPrintPin( PIN_BUSACK ); Serial.write("\n");
}

void chipClockPulse()
{
  digitalWrite(PIN_CLK, HIGH);
  delay(100);
  digitalWrite(PIN_CLK, LOW);
  delay(100);
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

