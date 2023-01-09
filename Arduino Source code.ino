
#include <LiquidCrystal.h> //defined the libraries and the variables for the the LCD.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // the lcd is connected to these pins on the arduino board 

int pin2 = 3;  // defined the variables for the dust sensor pins
int pin1 = 2;
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 1s ;// to received inputs in 3000 miliseconds 
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

//defined the variables for the LED Light System
int wLed = A1;
int gLed = A2;
int yLed = A3;
int rLed = A4;
int bLed = A5;

void setup() {
  Serial.begin(9600); // port rate
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(wLed,OUTPUT);
  pinMode(gLed,OUTPUT);
  pinMode(yLed,OUTPUT);
  pinMode(rLed,OUTPUT);
  pinMode(bLed,OUTPUT);
  starttime = millis();//get the current time;
  lcd.begin(16, 2); // LCD display has 16 characters and two line
}

void loop() {
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1; //calculate lowpulseoccupancy.
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2; //lowpulseoccupancy" represents the Lo Pulse Occupancy Time(LPO Time) detected in given 30s.


  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  //calculation part
  {
    //calculate ratio
    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    //calculate concentration
    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

    lcd.setCursor(0, 0); // set the curseor to 0 ,0 on the LCD Display 
    lcd.print("PM10 ");// display to lcd panel "PM10"
    lcd.setCursor(6, 0);
    lcd.print(concentration1,3);//display the calculation value

    //Serial monitor    
    Serial.print("concentration1 = ");
    Serial.print(concentration1);
    Serial.print(" pcs/0.01cf  -  ");

    Serial.print("concentration2 = ");
    Serial.print(concentration2);
    Serial.print(" pcs/0.01cf  -  ");

    
    if (concentration1 < 1000) {   // When the value will be less than 1000 , then the LCD  will display “CLEAN” 
     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i) // The LCD can diplay 16 characters
   {
     lcd.write(' ');
   }
      
     lcd.setCursor(4, 1);
     lcd.print("CLEAN");
     
     digitalWrite(wLed, HIGH);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
  }
    if (concentration1 > 1000 && concentration1 < 10000) { // if value between 1000 and 10000 , then the LCD  will display “Good” 

     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i)
  {
     lcd.write(' ');
  }
      
     lcd.setCursor(4, 1);
     lcd.print("GOOD");
     
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, HIGH);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
    }
    
    if (concentration1 > 10000 && concentration1 < 20000) { // if between 10000 and 20000 , then the LCD  will display “ACCEPTABLE” 

    lcd.setCursor (0, 1);
    for (int i = 0; i < 16; ++i)
  {
    lcd.write(' ');
  }
      
      lcd.setCursor(4, 1);
     lcd.print("ACCEPTABLE");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, HIGH);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, LOW);
    }
      if (concentration1 > 20000 && concentration1 < 50000) { // if between 20000 and 50000 , then the LCD  will display “HEAVY” 
     lcd.setCursor (0, 1);
     for (int i = 0; i < 16; ++i)
  {
     lcd.write(' ');
  }   
     lcd.setCursor(4, 1);
     lcd.print("HEAVY");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, HIGH);
     digitalWrite(bLed, LOW);
  }

    if (concentration1 > 50000 ) { // When the value will be greater than 50000 , then the LCD  will display “HAZARD”
lcd.setCursor (0, 1);
for (int i = 0; i < 16; ++i)
{
  lcd.write(' ');
}
      
     lcd.setCursor(4, 1);
     lcd.print("HAZARD");
     digitalWrite(wLed, LOW);
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     digitalWrite(bLed, HIGH);
     
    } 
      
    lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();
  }
}
