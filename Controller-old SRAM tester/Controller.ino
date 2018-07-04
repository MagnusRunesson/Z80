const int PIN_WE = 46;
const int PIN_OE = 47;
const int PIN_CE = 48;

const int MAX_INPUTMESSAGE_LENGTH = 128;

unsigned char inputMessage[ MAX_INPUTMESSAGE_LENGTH ];
int inputWriteOffset;
unsigned int frameCount;

typedef struct menuOption {
  char* menuMessage;
  unsigned char menuKey;
  void(*pfnFunc)(unsigned char* input);
};

void serialWriteInt( int _o )
{
  char itoab[ 32 ];
  itoa( _o, itoab, 10 );
  Serial.write( itoab );
}







void readData( unsigned char* _input )
{
  int readByte = 0;
  Serial.write( "Read byte: " );
  
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = 38 + i;
    pinMode( pin, INPUT );
    if( digitalRead( pin ) == HIGH )
    {
      Serial.write( "1" );
      readByte |= 1 << i;
    } else
    {
      Serial.write( "0" );
    }
  }

  Serial.write( "\nRead byte: " );
  serialWriteInt( readByte );
  Serial.write( "\n" );
}

void setData( unsigned char* _input )
{
  char* pszData = (char*)&_input[ 2 ];
  Serial.write( "New data: " );
  Serial.write( pszData );
  Serial.write( "\n" );
  int data = atoi( pszData );

  // Iterate through every bit and see if it is set or not
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = 38 + i;
    pinMode( pin, OUTPUT );
    
    int mask = 1 << i;
    if((data & mask) == mask)
    {
      Serial.write( "1" );
      digitalWrite( pin, HIGH );
    }
    else
    {
      Serial.write( "0" );
      digitalWrite( pin, LOW );
    }
  }

  Serial.write( "\n" );
}

void setAddress( unsigned char* _input )
{
  char* pszAddress = (char*)&_input[ 2 ];
  Serial.write( "New address: " );
  Serial.write( pszAddress );
  Serial.write( "\n" );
  int address = atoi(pszAddress);

  // Iterate through every bit and see if it is set or not
  int i;
  for( i=0; i<15; i++ )
  {
    int pin = 22 + i;
    pinMode( pin, OUTPUT );
    
    int mask = 1 << i;
    if((address & mask) == mask)
    {
      Serial.write( "1" );
      digitalWrite( pin, HIGH );
    }
    else
    {
      Serial.write( "0" );
      digitalWrite( pin, LOW );
    }
  }

  Serial.write( "\n" );
}

void resetChip( unsigned char* _input )
{
  Serial.write( "Resetting chip\n" );
  
  pinMode( PIN_OE, OUTPUT );
  digitalWrite( PIN_OE, HIGH );
  pinMode( PIN_WE, OUTPUT );
  digitalWrite( PIN_WE, HIGH );
  pinMode( PIN_CE, OUTPUT );
  digitalWrite( PIN_CE, HIGH );

/*
  pinMode( 52, OUTPUT );
  digitalWrite( 52, HIGH );
  delay( 500 );
  digitalWrite( 52, LOW );
  delay( 500 );
  digitalWrite( 52, HIGH );
  delay( 500 );
  digitalWrite( 52, LOW );
  */
}

void setWriteEnable( unsigned char* _input)
{
  int pin = PIN_WE;// 48;
  pinMode( pin, OUTPUT );
  bool active = (_input[ 2 ] == 'a');
  Serial.write( "Write Enable - Active: " );
  Serial.write( active ? "Yes" : "No" );
  Serial.write( "\n" );
  digitalWrite( pin, active ? LOW : HIGH );
}

void setOutputEnable( unsigned char* _input)
{
  int pin = PIN_OE;// 49;
  pinMode( pin, OUTPUT );
  bool active = (_input[ 2 ] == 'a');
  Serial.write( "Output Enable - Active: " );
  Serial.write( active ? "Yes" : "No" );
  Serial.write( "\n" );
  digitalWrite( pin, active ? LOW : HIGH );
}

void setChipEnable( unsigned char* _input)
{
  int pin = PIN_CE;//50;
  pinMode( pin, OUTPUT );
  bool active = (_input[ 2 ] == 'a');
  Serial.write( "Chip Enable - Active: " );
  Serial.write( active ? "Yes" : "No" );
  Serial.write( "\n" );
  digitalWrite( pin, active ? LOW : HIGH );
}


menuOption menu[] {
  {
    "Reset chip",
    '1',
    &resetChip,
  },
  {
    "Read data",
    'r',
    &readData,
  },
  {
    "Set data",
    'd',
    &setData,
  },
  {
    "Set address (a x)",
    'a',
    &setAddress,
  },
  {
    "Output enabled (o a)",
    'o',
    &setOutputEnable,
  },
  {
    "Write enabled (w, a)",
    'w',
    &setWriteEnable,
  },
  {
    "Chip enabled (w, a)",
    'c',
    &setChipEnable,
  },
};

int numMenuOptions = sizeof( menu ) / sizeof( menuOption );

void resetRead()
{
  inputWriteOffset = 0;
}

void setup() {
  frameCount = 0;
  Serial.begin( 9600 );
  resetRead();
  showMenu();
}

void loop() {
  inputCheck();
  frameCount++;
}

void inputCheck()
{
  //Serial.write( "Is reading: " + isReading ? "Yes\n":"No\n" );
  if( Serial.available())
  {
    /*
    Serial.write("Frame: ");
    char itoaBuffer[ 32 ];
    itoa(frameCount, itoaBuffer, 10 );
    Serial.write( itoaBuffer );
    Serial.write( ": " );
    Serial.write("Serial available\n");
    */
    
    // Copy to message parameter
    unsigned char byteRead = Serial.read();
    
    if( byteRead == 10 )
    {
      // New line
      inputMessage[ inputWriteOffset ] = 0;
      runMenu( inputMessage[ 0 ], inputMessage );

      resetRead();
    } else {
      inputMessage[ inputWriteOffset++ ] = byteRead;
    }
    /*
    Serial.write( byteRead );
    itoa( byteRead, itoaBuffer, 10 );
    Serial.write( ": '" );
    Serial.write( itoaBuffer );
    Serial.write( "'\n" );
    */
  }
}

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
    
    showMenu();
  }
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

