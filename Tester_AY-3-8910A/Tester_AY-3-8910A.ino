#include <avr/io.h>


void serialWriteInt( int _o )
{
  char itoab[ 32 ];
  itoa( _o, itoab, 10 );
  Serial.write( itoab );
}

void serialWriteLong( long _o )
{
  char itoab[ 32 ];
  sprintf(itoab, "%lu", _o);
  Serial.write( itoab );
}

void serialWriteHex( int _o )
{
  char itoab[ 9 ];
  itoa( _o, itoab, 16 );
  Serial.write( itoab );
}

void serialWriteBool( bool _value )
{
  Serial.write(_value?"True":"False");
}

void debugDump()
{
 
  Serial.write("hello");
  serialWriteInt(F_CPU);

  if( F_CPU > 15000000) {
    Serial.write("higher");
  }

  int iv = 10;
  unsigned long ulv = 10;
  Serial.print("\n10 size: ");
  serialWriteInt(10);
  Serial.print("\niv size: ");
  serialWriteInt(sizeof(iv));
  Serial.print("\nulf size: ");
  serialWriteInt(sizeof(ulv));  
  Serial.print("\nF_CPU: ");
  serialWriteLong(F_CPU);
  Serial.print("\nsize OCR1A: ");
  serialWriteLong(sizeof(OCR1A));
  Serial.print("\nsize OCR1AL: ");
  serialWriteLong(sizeof(OCR1AL));
  Serial.print("\nsize OCR1AH: ");
  serialWriteLong(sizeof(OCR1AH));
  Serial.print("\nOCR1A=");
  serialWriteLong(OCR1A);

}
const int DIVISOR = 7;

int pinsDA[] = {
  23,
  25,
  27,
  29,
  31,
  33,
  35,
  37
};

int pinBC1 = 47;
int pinBC2 = 49;
int pinBDIR = 51;
int pinRESET = 53;
int pinCLOCK = 11; // Decided by OC1A

void clockSetup()
{
  pinMode(pinCLOCK, OUTPUT);
  digitalWrite(pinCLOCK, LOW);

  // Setup timer 1 to tick at 1 MHz
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12) | (1 << CS10);
  TCCR1C = 0;
  TIMSK1 = 0;
  OCR1A = 8;
}

void ay_reset()
{
  digitalWrite(pinRESET, LOW);
  
}

void ay_set_bus(int v)
{
  int bitmask = 1;
  for(int i=0; i<8; i++)
  {
    digitalWrite(pinsDA[i], (v & bitmask) ? HIGH : LOW);
    bitmask <<= 1;
  }
}

int ay_get_bus()
{
  int ret = 0;
  for(int i=0; i<8; i++)
  {
    int r = (digitalRead(pinsDA[i]) == HIGH) ? 1 : 0;
    ret |= r << i;
  }
  return ret;
}

void ay_send_address(int r)
{
  ay_set_bus(r);
  //digitalWrite(pinBC1, HIGH);
  digitalWrite(pinBDIR, HIGH);
  digitalWrite(pinBC1, HIGH);
  digitalWrite(pinBC2, HIGH);
  //delay(10);
  digitalWrite(pinBC1, LOW);
  digitalWrite(pinBC2, LOW);
  digitalWrite(pinBDIR, LOW);
  //digitalWrite(pinBC1, LOW);
  //delay(10);
}

void ay_send_data(int v)
{
  ay_set_bus(v);
  digitalWrite(pinBC1, LOW);
  digitalWrite(pinBC2, HIGH);
  digitalWrite(pinBDIR, HIGH);
  //delay(10);
  digitalWrite(pinBDIR, LOW);
  digitalWrite(pinBC2, LOW);
  digitalWrite(pinBC1, LOW);
  //delay(10);
}

int ay_receive_data()
{
  digitalWrite(pinBC2, HIGH);
  digitalWrite(pinBC1, HIGH);
  delay(10);
  int ret = ay_get_bus();
  digitalWrite(pinBC1, LOW);
  digitalWrite(pinBC2, LOW);
  return ret;
}

void ay_write_register(int r, int v)
{
  ay_send_address(r);
  ay_send_data(v);
}

int ay_read_register(int r)
{
  ay_send_address(r);
  return ay_receive_data();
}

long hi;
long lo;
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  pinMode(pinRESET, OUTPUT);
  digitalWrite(pinRESET, LOW);

  clockSetup();


  pinMode(pinBC1, OUTPUT);
  digitalWrite(pinBC1, LOW);
  pinMode(pinBC2, OUTPUT);
  digitalWrite(pinBC2, LOW);
  pinMode(pinBDIR, OUTPUT);
  digitalWrite(pinBDIR, LOW);

  int i;
  for(i=0; i<8; i++)
  {
    pinMode(pinsDA[i], OUTPUT);
    digitalWrite(pinsDA[i], LOW);
  }

  delay(100);
  digitalWrite(pinRESET, HIGH);
  delay(10);

  for(i=0; i<16; i++)
  {
    ay_write_register(i, 0x0);
  }

  lo = 0;
  hi = 0;
  
  ay_write_register(0, lo);
  ay_write_register(1, hi);
  ay_write_register(2, 0xb8);
  ay_write_register(3, 0x01);
  ay_write_register(4, 0xb8);
  ay_write_register(5, 0x02);

  ay_write_register(6, 4);

  /*
  ay_write_register(7, 0);
  
  Serial.print("register 7 before: ");
  serialWriteInt(ay_read_register(7));
  Serial.print("\n");
  */
  ay_write_register(7, 0xfe);
  /*
  Serial.print("register 7 after: ");
  serialWriteInt(ay_read_register(7));
  Serial.print("\n");
  */

  ay_write_register(8, 15);
  ay_write_register(9, 15);
  ay_write_register(10, 15);
}

void loop()
{
  /*
  delay(50);
  serialWriteInt(analogRead(A0));
  Serial.print("\n");
  */

  lo++;
  if(lo >= 256)
  {
    lo = 0;
    hi++;
    if( hi >= 16) {
      hi = 0;
    }
  }

  ay_write_register(0, lo);
  ay_write_register(1, hi);
}

