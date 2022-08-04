#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* Logic Variables */
int btnState1;
int btnState2; 
int prevBtnState;
int btnCounter = 20;                //temperature in Celcius (Default is 20C)
int value;                          //val read from temp sensor

/* Output & Input Pins */
int L = 13;                         //onboard led
int relayPin = 4; 
const int tempPin = 1;
const int btn1 = 3; 
const int btn2 = 2;
LiquidCrystal_I2C lcd(0x27,16,2);

/* Timing */
unsigned long startMillis;
unsigned long prevMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  //temperature reading delay 
const unsigned long btnDelay = 500; //button input delay 

void setup()
{
  Serial.begin(9600); //starting the serial monitor
  
/* Initiate inputs/outputs */
  pinMode(L, OUTPUT);
  pinMode(relayPin, OUTPUT);        
  pinMode(tempPin, INPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  
/* Initial starting LCD screen */
  lcd.init();                         
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Set to:");
  lcd.print(btnCounter);
  lcd.print("C");

 digitalWrite(relayPin,LOW);          //Relay is initially off
}

void loop(){
  startMillis = millis();
  
  btnState1 = digitalRead(btn1);
  btnState2 = digitalRead(btn2);
 
  if (btnState1 != prevBtnState) {   
    if (btnState1 == HIGH) {          //if btnstate1 changed, add to the temperature counter
      btnCounter++;
      Serial.print("desired temp:");
      Serial.println(btnCounter);
      
      lcd.setCursor(0,0);
      lcd.print("                 "); //clearing first row
      lcd.setCursor(0,0);
      lcd.print("Set to:");
      lcd.print(btnCounter);
      lcd.print("C");
    }
    while(millis() < currentMillis + btnDelay){
    }
  }
 
  if (btnState2 != prevBtnState) { 
    if (btnState2 == HIGH) {           //if btnstate2 changed, subtract from the temperature counter
      btnCounter--;
      Serial.print("desired temp:");
      Serial.println(btnCounter);
      
      
      lcd.setCursor(0,0);
      lcd.print("                 ");  //clearing first row
      lcd.setCursor(0,0);
      lcd.print("Set to:");
      lcd.println(btnCounter);
      lcd.print("C");
    }
    while(millis() < currentMillis + btnDelay){
    }
  }
  
  if (currentMillis - startMillis >= period)  {
    value = analogRead(tempPin);        //Calculating temperature based on voltage measured by the temperature sensor
    double millivolts = (value / 1024.0) * 5000; 
    double celsius = millivolts / 10;
    
    lcd.setCursor(0,1);
    lcd.print("Current:");
    lcd.print(celsius); 
    lcd.print("C"); 
    Serial.print("celsius:");
    Serial.println(celsius);            //read the value from the sensor
    
    if(celsius >= btnCounter) { 
      digitalWrite(L, HIGH);
      digitalWrite(relayPin, LOW);  
      Serial.print("Cooling on");
    }
    
    if (celsius < btnCounter) {
      digitalWrite(L, LOW);
      digitalWrite(relayPin, HIGH);
      Serial.print("Cooling off");
    }
  startMillis = currentMillis;           //saving start time
  }

}