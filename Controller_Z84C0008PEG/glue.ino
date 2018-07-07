char* RAM;

const int GLUE_CHIPMASK_READ_INSTRUCTION    = CHIPMASK_ACTIVE_M1 | CHIPMASK_ACTIVE_RD | CHIPMASK_ACTIVE_MREQ;
const int GLUE_CHIPMASK_READ_MEMORY         = CHIPMASK_ACTIVE_RD | CHIPMASK_ACTIVE_MREQ;
const int GLUE_CHIPMASK_WRITE_MEMORY        = CHIPMASK_ACTIVE_WR | CHIPMASK_ACTIVE_MREQ;

int glueHasReset;

void glueSetup()
{
  RAM = malloc(65536);

  Serial.write( "RAM: 0x" );
  serialWriteHex( (int)RAM );
  Serial.write( "\n" );
  
  glueHasReset = 0;
}

void glueFigureOutWhatsNext( unsigned char* _pszMessage )
{
  if( glueHasReset == 0 ) {
    Serial.write( "Reset\n" );
    chipReset();
    glueHasReset = 1;
    return;
  }
  
  int chipStatus = chipGetStatusMask();
  Serial.write( "Chip status mask: 0x" );
  serialWriteHex( chipStatus );
  Serial.write( "\n" );

  if( chipStatus == GLUE_CHIPMASK_READ_INSTRUCTION )
  {
    Serial.write("Read instruction\n");
    ioReadFromAddressWriteToData(first);
    chipClockPulse();
  } else if( chipStatus == GLUE_CHIPMASK_READ_MEMORY )
  {
    Serial.write("Read memory\n");
    ioReadFromAddressWriteToData(first);
    chipClockPulse();
  } else if( chipStatus == GLUE_CHIPMASK_WRITE_MEMORY )
  {
    Serial.write("Write memory\n");
    ioWriteFromDataToAddress( RAM );
    chipClockPulse();
  } else
  {
    Serial.write("Unknown\n");
    chipClockPulse();
  }
}

