#include <util/twi.h>

#define I2C_ERR_NONE 0x00
#define I2C_ERR_TIMEOUT 0x01
#define I2C_ERR_BUS 0x02

void i2c_set_error(uint8_t code, uint8_t pos)
{
  Serial.write("I2C error code. Code: " );
  serialWriteInt( code );
  Serial.write(" - Pos: " );
  serialWriteInt( pos );
  Serial.write("\n");
}

void i2c_init()
{
  /*
  TWBR: bit rate register
  TWSR: status register (contains preselector bits)

  prescalar
    0    1
    1   4
    2   16
    3   64

  f = F_CPU/(16+2*TWBR*prescalar)
  
  F_CPU = 16MHz
    TWBR = 152;
    TWSR = 0;
  --> 50KHz

    TWBR = 72;
    TWSR = 0;
  --> 100KHz

    TWBR = 12;
    TWSR = 0;
  --> 400KHz

    F_CPU/(2*100000)-8  --> calculate TWBR value for 100KHz
*/
  TWSR = 0;
  TWBR = F_CPU/(2*600000)-8;
}

uint8_t i2c_wait(uint8_t mask, uint8_t pos)
{
  uint32_t cnt = 6000; /* timout value should be > 280 for 50KHz Bus and 16 Mhz CPU, however the start condition might need longer */
  while( !(TWCR & mask) )
  {
    if ( cnt == 0 )
    {
      i2c_set_error(I2C_ERR_TIMEOUT, pos);
      return 0; /* error */
    }
    cnt--;
    waitCycles++;
  }
  
  return 1; /* all ok */
}

/* sla includes all 8 bits (with r/w bit), assums master transmit */
uint8_t i2c_start(uint8_t sla)
{
  register uint8_t status;
  
  /* send start */
  TWCR = _BV(TWINT) |  _BV(TWSTA)  |  _BV(TWEN);
   
  /* wait */
  if ( i2c_wait(_BV(TWINT), 1) == 0 )
    return 0;
  
  status = TW_STATUS;
 
  /* check status after start */  
  if ( status != TW_START && status != TW_REP_START )
  {
    i2c_set_error(I2C_ERR_BUS, 1);
    return 0;
  }

  /* set slave address */  
  TWDR = sla;
  
  /* enable sla transfer */
  TWCR = _BV(TWINT)  |  _BV(TWEN);

  /* wait */
  if ( i2c_wait(_BV(TWINT), 2) == 0 )
    return 0;

  status = TW_STATUS;
  /* check status after sla */  
  if ( status != TW_MT_SLA_ACK )
  {
    i2c_set_error(I2C_ERR_BUS, 2);
    return 0;
  }

   return 1;
}

void i2c_stop(void)
{
  /* write stop */
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

  /* no error is checked for the stop condition */  
  i2c_wait(_BV(TWSTO), 4);
}

uint8_t i2c_send(uint8_t data)
{
  register uint8_t status;
  TWDR = data;
  TWCR = _BV(TWINT)  |  _BV(TWEN);
  if ( i2c_wait(_BV(TWINT), 3) == 0 )
    return 0;

  status = TW_STATUS;
  if ( status != TW_MT_DATA_ACK )
  {
    i2c_set_error(I2C_ERR_BUS, 3);
    return 0;
  }

  return 1;  
}

/*
uint8_t i2c_read(bool isLastByte) {

  if (isLastByte) TWCR = (1 << TWINT) | (1 << TWEN);
  else TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  return TWDR;
}
*/


