#include <FT54X6.h>//using i2c bus SDA SCL and Interrupt pin
FT54X6 TchPad(PA11);//Tested for "Blue Pill" Board (STM32F103C8)
void setup()
{
    Serial.begin(115200);
    Serial.println(TchPad.begin());
}
void loop()
{
    if (TchPad.touched()){
  byte n = TchPad.getNoOfTouches();
  for (uint8_t i=1;i<n+1;i++)
  {

    TouchPoint TP= TchPad.getTouchLoc(i);///ith Touch_point
    Serial.print("TPoint:"+String(i)+" M:" + String(TP.M)+" P:"+String(TP.P)+" Pos:("+String(TP.X)+','+String(TP.Y)+") ");
    /*TP.M=Touch_mode: 0==> Press Down
                        1==> Lift Up
                        2=>> Contact
                        3==>: No event
    TP.P==> Touch_pressure
    TP.X==> position x 
    TP.Y==> position y*/

  }
  Serial.println("");   
 }

}