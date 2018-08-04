#define ADDR  0x78


void i2c_start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_send(uint8_t data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

}

uint8_t i2c_read(bool isLastByte) {

  if (isLastByte) TWCR = (1 << TWINT) | (1 << TWEN);
  else TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}


void display_init() {
  i2c_start();
  i2c_send(ADDR);
  i2c_send(0x00); //................ control byte
  i2c_send(0xAE); //................ displayOff
  i2c_send(0x8D); //................   *
  i2c_send(0xD5); i2c_send(0x80); // clockFrequency
  i2c_send(0x32); //................ pumpVoltage0123
  i2c_send(0xA1); //................ segmentRemap
  i2c_send(0xC8); //................ flipVertically
  i2c_send(0xDA); i2c_send(0x12); // comConfiguration
  i2c_send(0xA8); i2c_send(0x3F); // multiplex
  i2c_send(0xD3); i2c_send(0x00); // displayOffest
  i2c_send(0x10); i2c_send(0x00); // columnAddr
  i2c_send(0x40); //................ startLine
  i2c_send(0xB0); //................ pageAddr
  i2c_send(0xAF); //................ displayOn
  i2c_stop();
}


void i2c_sendcommand(int _cmd)
{
  i2c_start();
  i2c_send(ADDR);
  i2c_send(0x80);
  i2c_send(_cmd);
  i2c_stop();
}

void testfest( int _anim )
{
  /*
  // COLUMNADDR
  i2c_sendcommand(0x21);
  i2c_sendcommand(0x00);
  i2c_sendcommand(0x7f);

  // PAGEADDR
  i2c_sendcommand(0x22);
  i2c_sendcommand(0x00);
  i2c_sendcommand(0x07);

  // SETSTARTLINE
  i2c_sendcommand(0x40);
  */

  int j;
  for( j=0; j<8; j++ )
  {
    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x00); //................ control byte
    i2c_send(0xB0 + j); //................ pageAddr
    i2c_stop();

    i2c_start();
    i2c_send(ADDR);
    i2c_send(0x40);

    int i;
    for( i=0; i<132; i++ ) {
      i2c_send( (_anim + i) & 255 );
    }

    i2c_stop();
  }
}

void setup() {

  // Initialization
  Serial.begin(57600);
  display_init();

  /*
  // Request RAM data
  i2c_start();
  i2c_send(ADDR);
  i2c_send(0x80); //   control byte
  i2c_send(0xB0); // page address
  i2c_send(0x80); //   control byte
  i2c_send(0x10); // column address high
  i2c_send(0x80); //   control byte
  i2c_send(0x00); // column address low
  i2c_send(0x40); //   control byte
  i2c_send(0xE3); // nop
  i2c_stop();

  // get RAM data
  uint8_t a[10];
  i2c_start();
  i2c_send(ADDR + 1);
  for (int i = 0; i < 9; i++) a[i] = i2c_read(false);
  a[9] = i2c_read(false);
  i2c_stop();

  // print results
  for (int i = 0; i < 10; i++) {
    Serial.print("0x");
    Serial.println(a[i], HEX);
  }
  */
}

int index = 0;

void loop() 
{
  testfest(index);
  index++;
  if( index >= 8 ) {
    index -= 8;
  }
}

