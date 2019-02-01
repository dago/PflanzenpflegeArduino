/*
    This is a demo to test Grove - Sunlight Sensor library

*/

#include <Wire.h>
#include <SeeedOLED.h>

#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

#include "Arduino.h"
#include "SI114X.h"

SI114X SI1145 = SI114X();
int humiditySensor = A1;

// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;
int flowsensor = 2;    //The pin location of the sensor
void rpm ()     //This is the function that the interupt calls
{
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
}

int buttonPin = 3;
int pumpPin = 5;
void buttonPress()
{
     int buttonState = digitalRead(buttonPin);

    // If the button is pressed, activate (close) the relay.
    if (buttonState == 1)   
    {
        digitalWrite(pumpPin, HIGH);
    }
    else   
    {
        digitalWrite(pumpPin, LOW);
    }
}


void setup() {

  //Serial.begin(9600);
  //Serial.println("Beginning Si1145!");

  while (!SI1145.Begin()) {
    //Serial.println("Si1145 is not ready!");
    delay(1000);
  }
  //Serial.println("Si1145 is ready!");

  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  dht.begin();

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  //SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  //SeeedOled.putString("Hello World!"); //Print the String

  pinMode(flowsensor, INPUT); //initializes digital pin 2 as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached

  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPress, CHANGE);
}

uint16_t sichtbar;
uint16_t infrarot;
uint16_t ultraviolett;
int moisture;

void loop() {
  //Serial.print("//--------------------------------------//\r\n");
  //Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
  //Serial.print("IR: "); Serial.println(SI1145.ReadIR());
  //the real UV value must be div 100 from the reg value , datasheet for more information.
  //Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV()/100);
  sichtbar = SI1145.ReadVisible();
  infrarot = SI1145.ReadIR();
  ultraviolett = SI1145.ReadUV();
  //SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  //SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  //SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Sichtbar: "); //Print the String
  SeeedOled.putNumber(sichtbar);
  SeeedOled.putString("  ");
  SeeedOled.setTextXY(1, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Infrarot: "); //Print the String
  SeeedOled.putNumber(infrarot);
  SeeedOled.putString("  ");
  SeeedOled.setTextXY(2, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("UV: "); //Print the String
  SeeedOled.putNumber(ultraviolett);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h))
  {
    SeeedOled.setTextXY(3, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("Read fail DHT");
  }
  else
  {
    SeeedOled.setTextXY(3, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("Air: ");
    SeeedOled.putNumber(t);
    SeeedOled.putString(" *C");
    SeeedOled.putNumber(h);
    SeeedOled.putString(" %");
  }
  SeeedOled.setTextXY(4, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Moisture: ");
  moisture = analogRead(humiditySensor);
  SeeedOled.putNumber(moisture);
  SeeedOled.putString("  ");

  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  // sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  // cli();      //Disable interrupts
  Calc = (NbTopsFan * 60 / 73); //(Pulse frequency x 60) / 73Q, = flow rate in L/hour
  SeeedOled.setTextXY(5, 0);
  SeeedOled.putNumber(Calc); //Prints the number calculated above
  SeeedOled.putString(" L/hour"); //Prints "L/hour" and returns a  new line

  // delay(200);
}



