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

void glueFigureOutWhatsNext( bool _bPrint = true , bool _bExecute = true );

void glueFigureOutWhatsNextMenu( unsigned char* _pszMessage )
{
  glueFigureOutWhatsNext( false, true );
  chipPrintStuff( true );
  glueFigureOutWhatsNext( true, false );
}

void glueFigureOutWhatsNext( bool _bPrint = true, bool _bExecute = true )
{
  if( glueHasReset == 0 ) {
    if( _bPrint )
    {
      Serial.write( "Reset\n" );
    }
    if( _bExecute )
    {
      chipReset();
      glueHasReset = 1;
    }
    return;
  }
  
  int chipStatus = chipGetStatusMask();
  if( _bPrint )
  {
    Serial.write( "Chip status mask: 0x" );
    serialWriteHex( chipStatus );
    Serial.write( " - " );
  }

  if( chipStatus == GLUE_CHIPMASK_READ_INSTRUCTION )
  {
    if( _bPrint )
    {
      Serial.write( "Read INSTRUCTION from address 0x" );
      serialWriteHex( addressBusRead());
      Serial.write( "\n" );
    }
    
    if( _bExecute )
    {
      ioReadFromAddressWriteToData(first);
      chipClockPulse();
    }
  } else if( chipStatus == GLUE_CHIPMASK_READ_MEMORY )
  {
    if( _bPrint )
    {
      Serial.write("Read MEMORY from address 0x");
      serialWriteHex( addressBusRead());
      Serial.write( "\n" );
    }
    
    if( _bExecute )
    {
      ioReadFromAddressWriteToData(first);
      chipClockPulse();
    }
  } else if( chipStatus == GLUE_CHIPMASK_WRITE_MEMORY )
  {
    if( _bPrint )
    {
      Serial.write("Write MEMORY to address 0x");
      serialWriteHex( addressBusRead());
      Serial.write( "\n" );
    }
    
    if( _bExecute )
    {
      ioWriteFromDataToAddress( RAM );
      chipClockPulse();
    }
  } else
  {
    if( _bPrint )
    {
      Serial.write("Unknown state. Just do a clock pulse\n");
    }
    
    if( _bExecute )
    {
      chipClockPulse();
    }
  }
}

