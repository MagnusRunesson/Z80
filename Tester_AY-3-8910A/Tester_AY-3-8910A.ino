/**************************************************************************************************








                                              bits 01234567
There is a problem with one of the pins. I believe 00000100 sooooo one of the data/address pins

The one that enables noise for channel 2. So that would be pin DA5 (zero based, so there is a DA0)












**************************************************************************************************/

#include <stdarg.h>
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

const int N_NOTES = 73;

const unsigned long note_table[N_NOTES] = {
   1911, // MIDI 24, 32.70 Hz
   1804, // MIDI 25, 34.65 Hz
   1703, // MIDI 26, 36.71 Hz
   1607, // MIDI 27, 38.89 Hz
   1517, // MIDI 28, 41.20 Hz
   1432, // MIDI 29, 43.65 Hz
   1351, // MIDI 30, 46.25 Hz
   1276, // MIDI 31, 49.00 Hz
   1204, // MIDI 32, 51.91 Hz
   1136, // MIDI 33, 55.00 Hz
   1073, // MIDI 34, 58.27 Hz
   1012, // MIDI 35, 61.74 Hz
   956, // MIDI 36, 65.41 Hz
   902, // MIDI 37, 69.30 Hz
   851, // MIDI 38, 73.42 Hz
   804, // MIDI 39, 77.78 Hz
   758, // MIDI 40, 82.41 Hz
   716, // MIDI 41, 87.31 Hz
   676, // MIDI 42, 92.50 Hz
   638, // MIDI 43, 98.00 Hz
   602, // MIDI 44, 103.83 Hz
   568, // MIDI 45, 110.00 Hz
   536, // MIDI 46, 116.54 Hz
   506, // MIDI 47, 123.47 Hz
   478, // MIDI 48, 130.81 Hz
   451, // MIDI 49, 138.59 Hz
   426, // MIDI 50, 146.83 Hz
   402, // MIDI 51, 155.56 Hz
   379, // MIDI 52, 164.81 Hz
   358, // MIDI 53, 174.61 Hz
   338, // MIDI 54, 185.00 Hz
   319, // MIDI 55, 196.00 Hz
   301, // MIDI 56, 207.65 Hz
   284, // MIDI 57, 220.00 Hz
   268, // MIDI 58, 233.08 Hz
   253, // MIDI 59, 246.94 Hz
   239, // MIDI 60, 261.63 Hz
   225, // MIDI 61, 277.18 Hz
   213, // MIDI 62, 293.66 Hz
   201, // MIDI 63, 311.13 Hz
   190, // MIDI 64, 329.63 Hz
   179, // MIDI 65, 349.23 Hz
   169, // MIDI 66, 369.99 Hz
   159, // MIDI 67, 392.00 Hz
   150, // MIDI 68, 415.30 Hz
   142, // MIDI 69, 440.00 Hz
   134, // MIDI 70, 466.16 Hz
   127, // MIDI 71, 493.88 Hz
   119, // MIDI 72, 523.25 Hz
   113, // MIDI 73, 554.37 Hz
   106, // MIDI 74, 587.33 Hz
   100, // MIDI 75, 622.25 Hz
   95, // MIDI 76, 659.26 Hz
   89, // MIDI 77, 698.46 Hz
   84, // MIDI 78, 739.99 Hz
   80, // MIDI 79, 783.99 Hz
   75, // MIDI 80, 830.61 Hz
   71, // MIDI 81, 880.00 Hz
   67, // MIDI 82, 932.33 Hz
   63, // MIDI 83, 987.77 Hz
   60, // MIDI 84, 1046.50 Hz
   56, // MIDI 85, 1108.73 Hz
   53, // MIDI 86, 1174.66 Hz
   50, // MIDI 87, 1244.51 Hz
   47, // MIDI 88, 1318.51 Hz
   45, // MIDI 89, 1396.91 Hz
   42, // MIDI 90, 1479.98 Hz
   40, // MIDI 91, 1567.98 Hz
   38, // MIDI 92, 1661.22 Hz
   36, // MIDI 93, 1760.00 Hz
   34, // MIDI 94, 1864.66 Hz
   32, // MIDI 95, 1975.53 Hz
   30, // MIDI 96, 2093.00 Hz
};

const int DIVISOR = 7;

const int AY_REG_TONE_A_LO = 0;
const int AY_REG_TONE_A_HI = 1;
const int AY_REG_TONE_B_LO = 2;
const int AY_REG_TONE_B_HI = 3;
const int AY_REG_TONE_C_LO = 4;
const int AY_REG_TONE_C_HI = 5;
const int AY_REG_NOISE = 6;
const int AY_REG_MIXER = 7;
const int AY_REG_AMP_A = 8;
const int AY_REG_AMP_B = 9;
const int AY_REG_AMP_C = 10;
const int AY_REG_ENVELOPE_A = 11;
const int AY_REG_ENVELOPE_B = 12;
const int AY_REG_ENVELOPE_C = 13;
const int AY_REG_IO_A = 14;
const int AY_REG_IO_B = 15;

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

int dsend = 10;

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
  Serial.print("Setting bus (lsb to msb) ");
  int bitmask = 1;
  for(int i=0; i<8; i++)
  {
    int val = (v & bitmask) ? HIGH : LOW;
    serialWriteInt(val);
    digitalWrite(pinsDA[i], val);
    bitmask <<= 1;
  }
  Serial.print("\n");
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
  //digitalWrite(pinBC1, HIGH);
  //digitalWrite(pinBC2, HIGH);
  delay(dsend);
  //digitalWrite(pinBC1, LOW);
  //digitalWrite(pinBC2, LOW);
  digitalWrite(pinBDIR, LOW);
  //digitalWrite(pinBC1, LOW);
  delay(dsend);
}

void ay_send_data(int v)
{
  ay_set_bus(v);
  //digitalWrite(pinBC1, LOW);
  digitalWrite(pinBC2, HIGH);
  digitalWrite(pinBDIR, HIGH);
  delay(dsend);
  digitalWrite(pinBDIR, LOW);
  digitalWrite(pinBC2, LOW);
  //digitalWrite(pinBC1, LOW);
  delay(dsend);
}

int ay_receive_data()
{
  int i;
  for(i=0; i<8; i++) {
    pinMode(pinsDA[i], INPUT);
  }
  digitalWrite(pinBC2, HIGH);
  digitalWrite(pinBC1, HIGH);
  delay(10);
  int ret = ay_get_bus();
  digitalWrite(pinBC1, LOW);
  digitalWrite(pinBC2, LOW);
  for(i=0; i<8; i++) {
    pinMode(pinsDA[i], OUTPUT);
  }
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
int note;

long lo0, lo1, lo2;

long vol;
long ch0, ch1, ch2, nch0, nch1, nch2;

void set_note(int note_index)
{
  int lo = note_table[note_index] & 0xff;
  int hi = note_table[note_index] >> 8;
  ay_write_register(0, lo);
  ay_write_register(1, hi);
}

int get_mix(int ch0, int ch1, int ch2, int nch0, int nch1, int nch2)
{
  int ret = 0xc0;
  ret += (nch2<<5);
  ret += (nch1<<4);
  ret += (nch0<<3);
  ret += (ch2<<2);
  ret += (ch1<<1);
  ret += ch0;
  plog("mix 0x%02x\n", ret);
  return ret;
}
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

  lo = 255;
  hi = 0;

  note = 0;

  lo0 = 0;
  lo1 = 0;
  lo2 = 0;

  vol = 15;
  
  ay_write_register(0, lo);
  ay_write_register(1, hi);
  ay_write_register(2, 0xb8);
  ay_write_register(3, 0x01);
  ay_write_register(4, 0xb8);
  ay_write_register(5, 0x02);

  ay_write_register(6, 4);

  ay_write_register(7, 0);

  Serial.print("register 7 before: ");
  serialWriteInt(ay_read_register(8));
  Serial.print("\n");
  ch0 = 0;
  ch1 = 1;
  ch2 = 1;
  nch0 = 1;
  nch1 = 1;
  nch2 = 1;
  ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
  Serial.print("register 7 after: ");
  serialWriteInt(ay_read_register(8));
  Serial.print("\n");

  ay_write_register(8, vol);
  ay_write_register(9, 15);
  ay_write_register(10, 15);
}

void inc_free()
{
  lo++;
  if(lo >= 256)
  {
    lo = 0;
    hi++;
    if( hi >= 16) {
      hi = 0;
    }
  }
  Serial.print("Sending hi=");
  serialWriteInt(hi);
  Serial.print(" lo=");
  serialWriteInt(lo);
  Serial.print("\n");
  ay_write_register(0, lo);
  ay_write_register(1, hi);
}

void dec_free()
{
  lo--;
  if(lo < 0)
  {
    lo = 255;
    hi--;
    if( hi < 0) {
      hi = 15;
    }
  }
  Serial.print("Sending hi=");
  serialWriteInt(hi);
  Serial.print(" lo=");
  serialWriteInt(lo);
  Serial.print("\n");
  ay_write_register(0, lo);
  ay_write_register(1, hi);
}

int get_wlo(int lo)
{
  return (lo & 0x1f) << 0;
}

// bits 0-3
int get_wlo0(int lo)
{
  return (lo & 0x0f) << 0;
}

// bits 4-5
int get_wlo1(int lo)
{
  return (lo & 0x03) << 4;
}

// bits 6-7
int get_wlo2(int lo)
{
  return (lo & 0x03) << 6;
}

int get_wlo(int lo0, int lo1, int lo2)
{
  int wlo0 = get_wlo0(lo0);
  int wlo1 = get_wlo1(lo1);
  int wlo2 = get_wlo2(lo2);
  int ret = wlo0 + wlo1 + wlo2;
  plog("wlo0=0x%02x wlo1=0x%02x wlo2=0x%02x ret=0x%02x\n", wlo0, wlo1, wlo2, ret);
  return ret;
}

void wrlo(int lo)
{
  ay_write_register(0, lo);
  serialWriteInt(lo);
  Serial.print("\n");
}

void plog(const char* _pszFormat, ...)
{
  char itoab[ 2048 ];
  va_list argptr;
  va_start(argptr, _pszFormat);
  //vfprintf(stderr, format, argptr);
  //sprintf(itoab, "%lu", _o);
  vsnprintf(itoab, 2048, _pszFormat, argptr);
  va_end(argptr);
  Serial.print(itoab);
}

void loop()
{
  /*
  delay(50);
  serialWriteInt(analogRead(A0));
  Serial.print("\n");
  */


  delay(160);
  while(Serial.available())
  {
    int wlo;
    int cmd = Serial.read();
    switch(cmd)
    {
      case '+':
        lo++;
        wlo = get_wlo(lo);
        wrlo(wlo);
        break;

      case '-':
        lo--;
        wlo = get_wlo(lo);
        wrlo(wlo);
        break;

      case 'q':
        lo0--;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;

      case 'w':
        lo0++;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;

      case 'a':
        lo1--;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;

      case 's':
        lo1++;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;
        
      case 'z':
        lo2--;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;

      case 'x':
        lo2++;
        wrlo(get_wlo(lo0, lo1, lo2));
        break;

      case 'e':
        vol = (vol-1) & 0x0f;
        plog("new volume is %i\n", vol);
        ay_write_register(8, vol);
        break;

      case 'r':
        vol = (vol+1) & 0x0f;
        plog("new volume is %i\n", vol);
        ay_write_register(8, vol);
        break;

      case 'g':
        ch0 = !ch0;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;
        
      case 'f':
        ch1 = !ch1;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;
        
      case 'd':
        ch2 = !ch2;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;

      case 'c':
        nch0 = !nch0;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;
        
      case 'v':
        nch1 = !nch1;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;
        
      case 'b':
        nch2 = !nch2;
        ay_write_register(7, get_mix(ch0, ch1, ch2, nch0, nch1, nch2));
        break;
    }
  }
  /*
  int wlo = 0;
  int i;
  for(i=0; i<8; i++)
  {
    wlo |= ((lo >> i) & 1) << (7-i);
  }
  */
  /*
  ay_write_register(0, wlo);
  serialWriteInt(lo);
  Serial.print("\n");
  */

  //set_note(note);
  /*
  note++;
  if( note >= 73)
  {
    note = 0;
  }
  */
//  ay_write_register(1, hi);

/*
  while(Serial.available())
  {
    int cmd = Serial.read();
    switch(cmd)
    {
      case '+':
        inc_free();
        break;

      case '-':
        dec_free();
        break;
    }

    if(cmd >= '1' && cmd <= '9')
    {
      int n = 30 + (cmd - '1');
      set_note(n);
      Serial.print("playing note ");
      serialWriteInt(n);
      Serial.print("\n");
    }
  }
  */

    /*
    if(cmd >= '1' && cmd <= '9')
    {
      int n = 30 + (cmd - '1');
      set_note(n);
      Serial.print("playing note ");
      serialWriteInt(n);
      Serial.print("\n");
    }
    */
}

