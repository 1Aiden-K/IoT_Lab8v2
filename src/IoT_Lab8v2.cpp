/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/aiden/Desktop/IoT-Engineering/VisualStudioCodeProjects/IoT_Lab8v2/src/IoT_Lab8v2.ino"
/*
 * Project IoT_Lab8
 * Description:
 * Author:
 * Date:
 */

#include "oled-wing-adafruit.h"
#include <Wire.h>
#include <blynk.h>

void setup();
void loop();
#line 12 "c:/Users/aiden/Desktop/IoT-Engineering/VisualStudioCodeProjects/IoT_Lab8v2/src/IoT_Lab8v2.ino"
SYSTEM_THREAD(ENABLED);

const uint8_t VCNL4040_ADDR = 0x60; //7-bit unshifted I2C address of VCNL4040

//Used to select between upper and lower byte of command register
#define LOWER true
#define UPPER false

//VCNL4040 Command Codes
#define VCNL4040_ALS_CONF 0x00
#define VCNL4040_ALS_THDH 0x01
#define VCNL4040_ALS_THDL 0x02
#define VCNL4040_PS_CONF1 0x03 //Lower
#define VCNL4040_PS_CONF2 0x03 //Upper
#define VCNL4040_PS_CONF3 0x04 //Lower
#define VCNL4040_PS_MS 0x04 //Upper
#define VCNL4040_PS_CANC 0x05
#define VCNL4040_PS_THDL 0x06
#define VCNL4040_PS_THDH 0x07
#define VCNL4040_PS_DATA 0x08
#define VCNL4040_ALS_DATA 0x09
#define VCNL4040_WHITE_DATA 0x0A
#define VCNL4040_INT_FLAG 0x0B //Upper
#define VCNL4040_ID 0x0C


bool oledVeiw = true;
bool oledOn = true;

OledWingAdafruit display;

#define RED_LIGHT D8
#define YELLOW_LIGHT D7
#define GREEN_LIGHT D6

void setup()
{
  Wire.begin(); 

  Wire.beginTransmission(VCNL4040_ADDR);

  Wire.write(VCNL4040_ALS_CONF);

  Wire.endTransmission();

  Blynk.begin("h9VMR0qLvSa2MKcs1Xy7SXPzGGI8xFhZ", IPAddress(167, 172, 234, 162), 8080);

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(YELLOW_LIGHT, OUTPUT);
  pinMode(GREEN_LIGHT, OUTPUT);

  display.setup();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  Serial.begin(9600);
}

void loop()
{
  display.loop();
  Blynk.run();
  display.clearDisplay();

  Wire.beginTransmission(VCNL4040_ADDR);

  Wire.write(VCNL4040_PS_DATA);

  Wire.endTransmission();

  //Wire.requestFrom(0x60, 2);

  unsigned int proximity = Wire.read();
  Serial.println(proximity);

  Wire.beginTransmission(VCNL4040_ADDR);

  Wire.write(VCNL4040_ALS_DATA);

  Wire.endTransmission();

  //Wire.requestFrom(0x60, 2);

  unsigned int ambient = Wire.read();
  Serial.println(ambient);

  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);

  if (display.pressedA())
  {
    oledVeiw = true;
    oledOn = true;
  }
  else if (display.pressedB())
  {
    oledVeiw = false;
    oledOn = true;
  }
  else if (display.pressedC())
  {
    if (oledOn)
    {
      oledOn = false;
    }
    else
    {
      oledOn = true;
    }
  }

  if (oledOn)
  {
    if (oledVeiw)
    {
      display.println("Proximity Level:");
      display.println(proximity);

      if (proximity < 25)
      {
        digitalWrite(GREEN_LIGHT, HIGH);
      }
      else if (proximity < 250)
      {
        digitalWrite(YELLOW_LIGHT, HIGH);
      }
      else
      {
        digitalWrite(RED_LIGHT, HIGH);
      }
    }
    else
    {
      display.println("Ambient Light Level:");
      display.println(ambient);

      if (ambient < 1000)
      {
        digitalWrite(RED_LIGHT, HIGH);
      }
      else if (ambient < 2400)
      {
        digitalWrite(YELLOW_LIGHT, HIGH);
      }
      else
      {
        digitalWrite(GREEN_LIGHT, HIGH);
      }
    }
  }
  else
  {
    Blynk.virtualWrite(V0, proximity);
    Blynk.virtualWrite(V1, ambient);
  }

  display.setCursor(0, 0);
  display.display();
}