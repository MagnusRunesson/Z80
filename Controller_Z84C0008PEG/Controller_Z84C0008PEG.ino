#include "first.h"

/*

  Arduino       Z80
  22-37         A0-A15
  38-45         D0-D7
  46            WR
  47            RD
  48            CLK
  49            INT
  8             NMI
  50            HALT
  51            MREQ
  9             IOREQ
  2             RFSH
  3             M1
  4             RESET
  5             BUSRQ
  6             WAIT
  7             BUSAK

 */
//const int PIN_ADDRESS_BASE    = 22;
const int PIN_ADDRESS[] = {
  37,
  23,
  24,
  25,
  26,
  27,
  28,
  29,
  30,
  12,
  32,
  33,
  34,
  35,
  36,
  22,
};

const int PIN_DATA_BASE       = 38;
const int PIN_WR              = 46;
const int PIN_RD              = 47;
const int PIN_CLK             = 48;
const int PIN_INT             = 49;
const int PIN_NMI             = 8;
const int PIN_HALT            = 50;
const int PIN_MREQ            = 51;
const int PIN_IOREQ           = 9;
const int PIN_RFSH            = 2;
const int PIN_M1              = 3;
const int PIN_RESET           = 4;
const int PIN_BUSREQ          = 5;
const int PIN_WAIT            = 6;
const int PIN_BUSACK          = 7;

typedef struct menuOption {
  char* menuMessage;
  unsigned char menuKey;
  void(*pfnFunc)(unsigned char* input);
};

void menuFunc( unsigned char* _input )
{
  
}

menuOption menu[] {
  {
    "Print chip information",
    '1',
    &chipPrintStuffMenu,
  },
  {
    "Chip reset",
    'r',
    &chipResetMenu,
  },
  {
    "Chip clock pulse",
    'c',
    &chipClockPulseMenu,
  },
  {
    "Address bus read",
    'a',
    &addressBusPrintMenu,
  },
  {
    "Data bus write",
    'd',
    &dataBusWriteMenu,
  },
  {
    "Data bus read",
    'f',
    &dataBusReadMenu,
  },
  {
    "Copy requested data from ROM to data bus",
    'o',
    &ioReadPartyMenu,
  },
  {
    "Copy from data to address on bus",
    'p',
    &ioWritePartyMenu,
  },
  {
    "Figure out what is next",
    'g',
    &glueFigureOutWhatsNextMenu,
  },
  {
    "IO Write party",
    'w',
    &ioWritePartyMenu,
  }
};

void setup() {
  serialSetup();
  chipSetup();
  glueSetup();
  showMenu();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char* pszMessage = serialLoop();
  if(pszMessage != NULL)
  {
    runMenu( pszMessage[ 0 ], pszMessage );
    //chipPrintStuff( true );
    //showMenu();
  }
}

void resetPins() {
  
}

