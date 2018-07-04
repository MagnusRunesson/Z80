
int numMenuOptions = sizeof( menu ) / sizeof( menuOption );

void runMenu( unsigned char _menuChoice, unsigned char* _pszMessage)
{
  /*
  Serial.write("Input: ");
  Serial.write(_menuChoice);
  Serial.write("\n");
  */

  bool ranMenu = false;
  int i;
  for( i=0; i<numMenuOptions; i++ )
  {
    if( _menuChoice == menu[ i ].menuKey )
    {
      Serial.write( "Identified command " );
      Serial.write( _menuChoice );
      Serial.write( "\n" );
      menu[ i ].pfnFunc( _pszMessage );
      ranMenu  = true;
      break;
    }
  }

  if( ranMenu == false )
  {
    Serial.write( "Unknown command: " );
    Serial.write( _menuChoice );
    Serial.write( " (" );
    Serial.write( (char*)_pszMessage );
    Serial.write( ")\n");
  }

  chipPrintStuff();
  showMenu();
}

void showMenu()
{
  Serial.write( "\n" );
  int i;
  for( i=0; i<numMenuOptions; i++ )
  {
    menuOption* p = &menu[ i ];
    Serial.write( p->menuKey );
    Serial.write( ": " );
    Serial.write( p->menuMessage );
    Serial.write( "\n" );
  }
  Serial.write( "------------------------------------------\n" );
}

