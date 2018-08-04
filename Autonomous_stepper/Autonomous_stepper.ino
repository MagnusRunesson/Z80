 typedef struct menuOption {
  char* menuMessage;
  unsigned char menuKey;
  void(*pfnFunc)(unsigned char* input);
};

const int DELAY_STEP = 1;
const int DELAY_LOOP = 100;

const int PIN_CLOCK = 12;

const int PIN_DATA[] =
{
  9,8,7,6,5,4,3,2,
};

void steppa( unsigned char* _input )
{
  if( _input != NULL )
  {
    Serial.write( "Step\n" );
  }
  
  digitalWrite( PIN_CLOCK, HIGH );
  delay( DELAY_STEP );
  digitalWrite( PIN_CLOCK, LOW );
  delay( DELAY_STEP );
}

menuOption menu[] {
  {
    "Step",
    '1',
    &steppa,
  }
};

void setup() {
  serialSetup();
  showMenu();
  pinMode( PIN_CLOCK, OUTPUT );
  digitalWrite( PIN_CLOCK, LOW );

  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PIN_DATA[ i ];
    pinMode( pin, INPUT );
  }
}

void dataBusRead()
{
  int data = 0;
  int i;
  for( i=0; i<8; i++ )
  {
    int pin = PIN_DATA[ i ];
    if( digitalRead( pin ) == HIGH )
    {
      data |= (1 << i);
    }
  }

  Serial.write( "Data buss: 0x" );
  serialWriteHex( data );
  Serial.write( "\n" );
}

void loop() {
  //*
  unsigned char* pszMessage = serialLoop();
  if(pszMessage != NULL)
  {
    steppa( "Hej" );
    delay( 100 );
    dataBusRead();
    //runMenu( pszMessage[ 0 ], pszMessage );
    //chipPrintStuff( true );
    //showMenu();
  }
  /*/
  delay( DELAY_LOOP );
  steppa( NULL );
  dataBusRead();
  /**/
}

