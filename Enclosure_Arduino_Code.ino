#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#include <avr/sleep.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celsius = 0;
float T_error=0;

#include <Servo.h>
int servoPin = 3;
int RelayPin=A2;
int ExhaustFan= 10;
int CircFan=9;
int Temp=0;

int angle=0;
int dangle=0;

//LED Indicators
int LED_MAN=A1;
int LED_VENT=11;
int LED_POW=13;
int LED_Array=A3;

//LED Array
const int LEDPB=5;
int ledState=LOW;
int LastButtonState_LED;
int currentButtonState_LED;
// Indexing For LED Tggle
int stateNow_LED = 0;
int stateBefore_LED = 0;
int i_LED = 0;
int j_LED = 0;
int k_LED=0;




// Indexing For Manual Control
int stateNow_MAN = 0;
int stateBefore_MAN = 0;
int i_MAN = 0;
int j_MAN = 0;
int k_MAN=0;
int time_now=0;
int count=0;

// Indexing For Heater Control
int stateNow_HOT = 0;
int stateBefore_HOT = 0;
int i_HOT = 0;
int j_HOT = 0;
int k_HOT=0;
int HeaterState=0;
int ManualPB =8;
int HeaterPB = 12;


//initiate Power Button
int PowerPin = 2;

// Power Button Setup
bool POWbuttonPressed = false;
bool POWState = LOW;
int RESET=0;


int Pot=0;

int E=0;

float kp=10;

float H_fact=1.1;
int E_=0;
Servo Servo1;

const int  HotPB = 6;
const int  ColdPB = 7;

int Tset = 20; 
int buttonStateplus = 0;
int buttonStateminus = 0;  
int lastButtonStateplus = 0;     
int lastButtonStateminus = 0; 


int Brightness=1;





void setup() {
  // put your setup code here, to run once:
  pinMode(PowerPin, INPUT_PULLUP);
  pinMode(LED_POW, OUTPUT);
  attachInterrupt(0, POWbuttonISR, FALLING);
  pinMode(LED_VENT,OUTPUT);



sensors.begin();

Servo1.attach(servoPin);

pinMode(RelayPin,OUTPUT);
pinMode(ExhaustFan,OUTPUT);
pinMode(CircFan,OUTPUT);
pinMode(LED_Array,OUTPUT);
//pinMode(ManualLED,OUTPUT);

pinMode(ManualPB,INPUT);
pinMode(HeaterPB,INPUT);

pinMode(HotPB, INPUT);
pinMode(ColdPB, INPUT); 

Serial.begin(9600);

  lcd.begin(20,4); 
  lcd.setCursor(0,0);
  lcd.print("T set");
  lcd.setCursor(0,2);
  lcd.print("T");
  lcd.setCursor(2,3);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(13,0);
  lcd.print("Venting"); 
  lcd.setCursor(13,2);
  lcd.print("Heater");


//digitalWrite(ManualLED, LOW);
//digitalWrite(CircFan, HIGH);

POWbuttonPressed = true;
}

void POWbuttonISR() { POWbuttonPressed = true; }

void loop() {
digitalWrite(CircFan, HIGH);

  
// POWER ON/OFF CONTROL
  if (POWbuttonPressed) {
    delay(10);     // debounce button press
    while (!digitalRead(PowerPin)) {}    // wait for it to be released
    delay(10);     // debounce button release
    digitalWrite(LED_POW, LOW);    // turn off LED
    POWState = LOW;

    Serial.println("Going to sleep...");
    Serial.flush();

    //TURN EVERYTHING OFF
    digitalWrite(CircFan, LOW);
    digitalWrite(LED_POW, LOW);
    digitalWrite(LED_VENT, LOW);
    digitalWrite(RelayPin, LOW);
    digitalWrite(LED_Array, LOW);
    digitalWrite(ExhaustFan, LOW);
    digitalWrite(LED_MAN,LOW);
    RESET=0;
    
    gotoSleep();    // function to put the processor to sleep; a button press will wake it up

    //TURN EVERYTHING ON
    digitalWrite(LED_POW, HIGH);
    delay(10);
    //LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
    lcd.begin(20,4); 
    lcd.setCursor(0,0);
    lcd.print("T set");
    lcd.setCursor(0,2);
    lcd.print("T");
    lcd.setCursor(2,3);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(13,0);
    lcd.print("Venting"); 
    lcd.setCursor(13,2);
    lcd.print("Heater");




    
    delay(10);     // debounce button press
    while (!digitalRead(PowerPin)) {}    // wait for it to be released
    delay(10);     // debounce button release
    POWbuttonPressed = false;
   Serial.flush();
  }










  
// Read Temperature
    //Only read every 5 seconds to avoid slowing program
if ((count>125)or(millis()<500)){
  lcd.setCursor(4,3);
  lcd.print(".");
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Temp=Celsius;
  count=0;
  lcd.setCursor(4,3);
  lcd.print(" ");
}



// Setpoint Temperature Controll
buttonStateplus = digitalRead(HotPB);
buttonStateminus = digitalRead(ColdPB);

  if (buttonStateplus != lastButtonStateplus) {
    // if the state has changed, increment the counter
    if (buttonStateplus == HIGH) {
      Tset=Tset+1;
      delay(10);
    }}

  if (buttonStateminus != lastButtonStateminus){
    if (buttonStateminus == HIGH) {
      Tset=Tset-1;
      delay(10);    
    }}


  //Display Temperature Readounts in Main Menu    
  if (k_MAN==0){

      lcd.setCursor(0,1);
      lcd.print(Tset);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0,3);
      lcd.print(Temp);
  
      lcd.print((char)223);
      lcd.print("C"); 


  }
 
  lastButtonStateplus = buttonStateplus;
  lastButtonStateminus = buttonStateminus;


// LED Array Toggle
  stateNow_LED = digitalRead(LEDPB);
  
    if (stateNow_LED != stateBefore_LED) {
    if (stateNow_LED==HIGH and i_LED==0) {
      //digitalWrite (ManualLED, HIGH);
      k_LED=1;
      j_LED=1;
      //digitalWrite(LED_Array, HIGH);
    }
    else if (stateNow_LED==LOW and j_LED==1) {
      i_LED=1;
    }
    else if (stateNow_LED==HIGH and i_LED==1) {
      //digitalWrite (ManualLED, LOW);
      j_LED=0;
      k_LED=0;
      //digitalWrite(LED_Array, LOW);
    
    }
    else if (stateNow_LED==LOW and j_LED==0) {
      i_LED=0;
    }
  }
  stateBefore_LED=stateNow_LED;

  Serial.print(k_LED);
  if (i_LED==1){
    digitalWrite(LED_Array, HIGH);
  }
  else{
    digitalWrite(LED_Array, LOW);
  }
  
  
// Manual Operation  
  stateNow_MAN = digitalRead(ManualPB);
  stateNow_HOT = digitalRead(HeaterPB);
  
  if (stateNow_MAN != stateBefore_MAN) {
    if (stateNow_MAN==HIGH and i_MAN==0) {
      //digitalWrite (ManualLED, HIGH);
      k_MAN=1;
      j_MAN=1;
      //digitalWrite(LED_MAN,HIGH);
    }
    else if (stateNow_MAN==LOW and j_MAN==1) {
      i_MAN=1;
    }
    else if (stateNow_MAN==HIGH and i_MAN==1) {
      //digitalWrite (ManualLED, LOW);
      j_MAN=0;
      k_MAN=0;
      //digitalWrite(LED_MAN,LOW);
      
      //Reinitiate Main Menu
      digitalWrite (RelayPin, LOW); 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T set");
      lcd.setCursor(0,2);
      lcd.print("T");
      lcd.setCursor(2,3);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(13,0);
      lcd.print("Venting"); 
      lcd.setCursor(13,2);
      lcd.print("Heater");
    }
    else if (stateNow_MAN==LOW and j_MAN==0) {
      i_MAN=0;
    }
  }

  if (k_MAN==1){ // if in manual opperation
    digitalWrite(LED_MAN,HIGH);
    if (stateNow_MAN==HIGH and i_MAN==1){
      k_MAN=0;
      
              
    }
      //Initiate Manual Controll Menu
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Manual");
      lcd.setCursor(13,2);
      lcd.print("Heater");
      lcd.setCursor(13,0);
      lcd.print("Venting");
      lcd.setCursor(0,2);
      lcd.print("T");
      lcd.setCursor(0,3);
      lcd.print(Temp);
      lcd.print((char)223);
      lcd.print("C");  


      // Manual Opperation Velting Controll
      E=analogRead(Pot);
      E=map(E,1020,3,0,100);
      E_=E;
      E=100-E;
      
      dangle=0.000110*pow(H_fact*E_,3) - 0.016965*pow(H_fact*E_,2) + 1.372789*(H_fact*E_);
      angle=dangle+83;  //dangle+83
      Servo1.write(angle);

      lcd.setCursor(13,1);
      lcd.print(E);
      lcd.print("%");


      // Manual Opperation Heater Controll Toggle
      if (HeaterState==1){
        lcd.setCursor(13,3);
        lcd.print("ON "); 
      }
      else{
        lcd.setCursor(13,3);
        lcd.print("OFF ");     }   


            
      if (stateNow_HOT != stateBefore_HOT) {
      if (stateNow_HOT==HIGH and i_HOT==0) {
        digitalWrite (RelayPin, HIGH);
        k_HOT=1;
        j_HOT=1;
        HeaterState=1;
        }
      else if (stateNow_HOT==LOW and j_HOT==1) {
        i_HOT=1;
        }
      else if (stateNow_HOT==HIGH and i_HOT==1) {
        digitalWrite (RelayPin, LOW);
        j_HOT=0;
        k_HOT=0;
        HeaterState=0;
    }
    else if (stateNow_HOT==LOW and j_HOT==0) {
      i_HOT=0;
    }
  }
  stateBefore_HOT=stateNow_HOT;
  }




  
  //Automatic Control
  else{
    digitalWrite(LED_MAN,LOW);
  T_error=Tset-Celsius;
    if (T_error>0.5){ // if the actual temperature is 0.5 degrees less than the setpoint activate the heater
      digitalWrite (RelayPin, HIGH);
      HeaterState=1;
    }
    else{
      digitalWrite (RelayPin, LOW);
      HeaterState=0;     
    }
    if (T_error<0){//if the temperature is too hot
      E=-kp*T_error;
      E=constrain(E,0,100);
      E_=100-E;
      dangle=0.000110*pow(H_fact*E_,3) - 0.016965*pow(H_fact*E_,2) + 1.372789*(H_fact*E_);
      angle=dangle+83;
      Servo1.write(angle);
      lcd.setCursor(13,1);
      lcd.print(E);
      lcd.print("%   ");

    }
    else{
      E=0;
      lcd.setCursor(13,1);
      lcd.print(E);
      lcd.print("%   ");      
    }
    if (HeaterState==1){
    lcd.setCursor(13,3);
    lcd.print("ON ");}
    else {
    lcd.setCursor(13,3);
    lcd.print("OFF");}
    }
  
      if (E>10){
       digitalWrite (ExhaustFan, HIGH);
        }
      else {
      digitalWrite (ExhaustFan, LOW);       
      }
  stateBefore_MAN=stateNow_MAN;
  delay(20);


//LED_VENT
Brightness=map(E,0,100,0,155);
analogWrite(LED_VENT,Brightness);
Serial.print(Brightness);
//Serial.print("    ");



count=count+1;
//    lcd.setCursor(6,3);
//    lcd.print(count);


  
}

void gotoSleep()
{
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  noInterrupts();           // timed sequence follows
  sleep_enable();
  interrupts();             // guarantees next instruction executed
  sleep_cpu();              // nighty-night!
  sleep_disable();          // awake again -- cancel sleep as a precaution
}

    
