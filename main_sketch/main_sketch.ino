
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Ports 
int redLED = 2;
int buzzPort = 3;
int tempSensor = 1;
float lowerBound = 67;
float higherBound = 70;

LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display


//void findTemps(float &tempC, float &tempF);
//void printInfo(float tempC, float tempF);

void setup() {
  Serial.begin(9600);

  setUpLight();

  Wire.begin(); 
  pinMode(redLED, OUTPUT);
  pinMode(buzzPort, OUTPUT);
  pinMode(tempSensor, INPUT);


  
}

void loop() {
     float temperatureC,temperatureF;
     
     findTemps(temperatureC, temperatureF);
     printInfo(temperatureC, temperatureF);
     
     bool needToAlarm = inRange(temperatureF);
     
     soundAlarm(needToAlarm);
     
     delay(1000); //waiting a second

}

////////////////////////////////////////////////
// Function to calculate the tempature values //
////////////////////////////////////////////////

void findTemps(float &tempC, float &tempF){
     int reading = analogRead(tempSensor);  

     // converting that reading to voltage, for 3.3v arduino use 3.3
     float voltage = reading * 5.0;
     voltage /= 1024.0; 
     
     // print out the voltage
     Serial.print(voltage);
     Serial.println(" volts");
     
     // now print out the temperature
     tempC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                                   //to degrees ((voltage - 500mV) times 100)
     
     // now convert to Fahrenheit
     tempF = (tempC * 9.0 / 5.0) + 32.0;
     
}

/////////////////////////////////////////////////////////////
// Function to setup the initial message on the LCD screen //
/////////////////////////////////////////////////////////////

void setUpLight(){
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Hello!!!"); // Print a message to the LCD.
  lcd.setCursor(1,1);
  lcd.print("I'm Tim");
  delay(1000);
  lcd.init();
}


/////////////////////////////////////////////////////
// Function to check if tempature is out of range  //
/////////////////////////////////////////////////////
bool inRange(float tempF){
  bool needToAlarm = false;
  if(tempF < lowerBound || tempF > higherBound){
    needToAlarm = true;
  }
 return needToAlarm;
}


/////////////////////////////////////////////////////////////////////
// Function to sound alarm and light of tempature is out of range  //
/////////////////////////////////////////////////////////////////////
void soundAlarm(bool readyAlarm){
  if (readyAlarm) {
    digitalWrite(redLED, HIGH);
    buzzAlarm();
    }
  else {digitalWrite(redLED, LOW);};
}

//////////////////////////////////////
// Function to print the tempature  //
//////////////////////////////////////
void printInfo(float tempC, float tempF){
     // Print on Serial Monitor
     Serial.print(" ");
     Serial.print(tempC); 
     Serial.println(" degrees C");
     Serial.print(tempF); 
     Serial.println(" degrees F");
     Serial.print(" ");

     // Print on LCD
      lcd.init(); // initialize the lcd
      lcd.backlight();
      lcd.setCursor(1,0);
      lcd.print("Temp C:"); // Print a message to the LCD.
      lcd.setCursor(8,0);
      lcd.print(tempC);
      lcd.setCursor(1,1);
      lcd.print("Temp F:");
      lcd.setCursor(8,1);
      lcd.print(tempF);
}

void buzzAlarm(){
  tone(buzzPort, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzPort);     // Stop sound...
  delay(1000); 
}
