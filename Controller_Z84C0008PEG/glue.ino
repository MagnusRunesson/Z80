void glueFigureOutWhatsNext( unsigned char* _pszMessage )
{
  int chipStatus = chipGetStatusMask();
  Serial.write( "Chip status mask: 0x" );
  serialWriteHex( chipStatus );
  Serial.write( "\n" );
}

