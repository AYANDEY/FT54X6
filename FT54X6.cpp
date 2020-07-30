#include "FT54X6.h" 


bool tch_ = false;


FT54X6::FT54X6(uint8_t INT_PIN)
{
    int_pin=INT_PIN;
}
void touch_isr()
{
tch_=true;
}
byte FT54X6::begin()
{
    Wire.begin();
    Wire.beginTransmission(FT54X6_ADDRESS);
    Wire.write((byte)FT54X6_MODE);
    Wire.write((byte)FT54X6_MODE_WORKING);
    byte error = Wire.endTransmission();

    #ifdef SERIAL_DEBUG
    if (error==0){Serial.println("SENT_OK");
    }else{
      Serial.println("Fail");
    }
    #endif
    pinMode(int_pin,INPUT);
    attachInterrupt(digitalPinToInterrupt(int_pin),touch_isr,FALLING);
    return error;
}


bool FT54X6::touched()
{
  if (tch_)
  {
      tch_=false;
      return true;
  }else
  {
    return false;
  }
}


uint8_t FT54X6::readRegister8(uint8_t reg) {
  uint8_t x;
  // use i2c
  Wire.beginTransmission(FT54X6_ADDRESS);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom((byte)FT54X6_ADDRESS, (byte)1);
  while (Wire.available()>0){
  x = Wire.read();
  }

#ifdef I2C_DEBUG
  Serial.print("$");
  Serial.print(reg, HEX);
  Serial.print(": 0x");
  Serial.println(x, HEX);
#endif

  return x;
}

void FT54X6::writeRegister8(uint8_t reg, uint8_t val) {
  // use i2c
  Wire.beginTransmission(FT54X6_ADDRESS);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

TouchPoint FT54X6::getTouchLoc(uint8_t num)
{
byte TOUCH_P = FT54X6_TOUCH1_XH+(num-1)*(byte)6;
//TOUCH_PXH=TOUCH_P //TOUCH_PXL=TOUCH_P+1 //TOUCH_PYH=TOUCH_P+2//TOUCH_PYL=TOUCH_P+3
uint16_t Touch_P_x=0;
byte x_ =readRegister8(TOUCH_P)&B00001111;
Touch_P_x=Touch_P_x|x_;
Touch_P_x=(Touch_P_x<<8)|readRegister8(TOUCH_P+(byte)1);

uint16_t Touch_P_Y=0;
byte Y_ =readRegister8(TOUCH_P+(byte)2)&B00001111;
Touch_P_Y=Touch_P_Y|Y_;
Touch_P_Y=(Touch_P_Y<<8)|readRegister8(TOUCH_P+(byte)3);

uint8_t mode = readRegister8(TOUCH_P)>>6;
uint8_t pressure =  readRegister8(TOUCH_P+(byte)4);
TouchPoint tp;
tp.M =mode;
tp.P=pressure;
tp.X=Touch_P_x;
tp.Y=Touch_P_Y;

return tp;
}

uint8_t FT54X6::getNoOfTouches()
{
  return readRegister8(0x02);
}
