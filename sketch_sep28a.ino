#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Joystick.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include "DHT.h"
#define XPIN A0
#define YPIN A1
#define SWPIN 30
#define buzzer 31
#define Menu1Screen 20
#define Menu2Screen 21
#define Menu3Screen 22
#define ActivatingAlarmScreen 2
#define ActivatedAlarmScreen 3
#define ChangePasswordScreen 4
#define PasswordChangedScreen 5
#define AlarmOnScreen 6
#define MappingScreen 7
#define HomeDataScreen 8
#define TrigPin 32
#define EchoPin 33
#define ServoPin 2
#define RST_PIN         5          
#define SS_PIN          53  
#define IRQ_PIN         19   
#define sensePin 34
#define Type DHT11


DHT HT(sensePin,Type);
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;
boolean ActiveAlarm = false;
String password = "1111";
String tempPassword;
boolean passChangeMode = false;
boolean passChanged = false;
int screenOffMsg = 0;
boolean AlarmActivated = false;
const byte ROWS = 4; 
const byte COLS = 4; 
char keypressed;
int Screen = 20;
boolean AuthorizedAccess=false;
int xJoystickPosition = 0;
int yJoystickPosition = 0;
long Duration;
int Distance;
int InitDistance;
float floatHumidity;
float floatTempC;
char charHumidity[7];
char charTempC[7]; 
char tempChar[40];
char tempChar1[40];


LiquidCrystal_I2C lcd(0x27, 16, 2);


char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {29, 28, 27, 26}; 
byte colPins[COLS] = {25, 24, 23, 22}; 

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

byte downArrow[] = {
 B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11111,
	B01110,
	B00100
};
byte upArrow[]={
  B00100,
	B01110,
	B11111,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000
};
byte degree[]={
  B00010,
  B00101,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


void setup() {
  myservo.write(90);
  lcd.init();
  lcd.backlight();
  pinMode(XPIN, INPUT_PULLUP);
  pinMode(YPIN, INPUT_PULLUP);
  pinMode(SWPIN, INPUT);
  digitalWrite(SWPIN, HIGH);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  myservo.attach(2);
  SPI.begin();      
  mfrc522.PCD_Init();   
  Serial.begin(9600);
  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, degree);
  HT.begin();
  

}

void loop() {


  if ( Screen == Menu1Screen) {
    AuthorizedAccess=false;
   ScreenTitleChange("Activate Alarm",0,0);
   Arrow('Down');
    delay(500);
    lcd.clear();

    Serial.print(map(analogRead(YPIN), 0, 1023, -1, 2));
    delay(500);
    digitalRead(SWPIN) == LOW ? Screen = ActivatingAlarmScreen : Screen = Menu1Screen;
    ScroolUpDown();
  }




  else if (Screen == Menu2Screen) {

    ScreenTitleChange("Change Password",0,0);
    Arrow('Both');
    delay(500);
    lcd.clear();
    delay(500);
    digitalRead(SWPIN) == LOW ? Screen = ChangePasswordScreen : Screen = Menu2Screen;
    ScroolUpDown();
  }
  else if (Screen ==Menu3Screen){
    ScreenTitleChange("Home Data",0,0);
    Arrow('Up');
    delay(500);
    lcd.clear();
    delay(500);
    digitalRead(SWPIN) == LOW ? Screen = HomeDataScreen : Screen = Menu3Screen;
    ScroolUpDown();
  }
else if(Screen==HomeDataScreen){

floatHumidity=HT.readHumidity();
floatTempC=HT.readTemperature();

dtostrf(floatHumidity,4,1,charHumidity);
dtostrf(floatTempC,4,1,charTempC);
sprintf(tempChar,"Temp: %s C",charTempC);
sprintf(tempChar1,"Humidity: %s%%",charHumidity);
lcd.setCursor(0, 0);
lcd.print(tempChar);
lcd.setCursor(0, 1);
lcd.print(tempChar1);
Serial.print(tempChar1);
delay(800);
lcd.clear();

/*
lcd.print("Temperature: %c",charTempC);
*/

  }
  else if (Screen == ActivatingAlarmScreen) {
    //tone(buzzer, 2000, 100);
    ScreenTitleChange("Alarm Activated",0,0);
    lcd.setCursor(0, 1);
    lcd.print("in:");
    int countdown = 5; 
    myservo.write(180);
    while (countdown != 0) {
      lcd.setCursor(13, 1);
      lcd.print(countdown);
      countdown--;
      tone(buzzer, 700, 100);
      delay(1000);
      if(countdown==0){
         InitDistance = calculateDistance();
         ScreenTitleChange("**MAPPING**",2,0);
         Screen = MappingScreen;
      }
     
    
  }
  }
  else if (Screen == MappingScreen){
    
 RotateServo();
 ScreenTitleChange("***ARMED***",2,0);
 Screen=ActivatedAlarmScreen;
  }
  else if (Screen == ActivatedAlarmScreen)
  {

    
    //Distance=calculateDistance()<InitDistance-10 ? Screen=AlarmOnScreen:Screen=ActivatedAlarmScreen;
    RotateServo();
    
  }
  else if (Screen==AlarmOnScreen){
  ScreenTitleChange("ALARM TRIGGERED",0,0);
  tone(buzzer, 700, 50);
  delay(100);
  
  }
  else if (Screen == ChangePasswordScreen)
  {

    lcd.clear();
    int i = 1;
    tone(buzzer, 2000, 100);
    tempPassword = "";
    lcd.setCursor(0, 0);
    lcd.print("Current Password");
    lcd.setCursor(0, 1);
    lcd.print(">");
    passChangeMode = true;
    passChanged = true;
    while (passChanged) {
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY) {
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
          tempPassword += keypressed;
          lcd.setCursor(i, 1);
          lcd.print("*");
          i++;
          tone(buzzer, 2000, 100);
        }
      }
      if (i > 5 || keypressed == '#') {
        tempPassword = "";
        i = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Current Password");
        lcd.setCursor(0, 1);
        lcd.print(">");
      }
      if ( keypressed == '*') {
        i = 1;
        tone(buzzer, 2000, 100);
        if (password != tempPassword) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong Password!");
          tone(buzzer, 2000, 2000);
          delay(2000);

          return;
        }
        if (password == tempPassword) {
          tempPassword = "";
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Set New Password");
          lcd.setCursor(0, 1);
          lcd.print(">");
          while (passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY) {
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i, 1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i = 1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Set New Password");
              lcd.setCursor(0, 1);
              lcd.print(">");
            }
            if ( keypressed == '*') {
              i = 1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false;
              passChanged = false;
              Screen = PasswordChangedScreen;
            }
          }
        }
      }
    }
  }

  if (Screen == PasswordChangedScreen) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password Changed");
    delay(2000);
    Screen = Menu1Screen;
  }



}

int ScroolUpDown() {
  int tempJoystickPosition = map(analogRead(YPIN), 0, 1023, -1, 2);

  switch (tempJoystickPosition) {
    case 0:
      break;
    case 1:
      Screen + 1 >= 22 ? Screen = 22 : Screen = Screen + 1;
      break;
    case -1:
      Screen - 1 <= 20 ? Screen = 20 : Screen = Screen - 1;
      break;


  }
}

void RotateServo(){
 for (int i = 180; i >=0; i--) { 
    
      
       myservo.write(i);              
       delay(10);
    
    
  }
  
  for (int i = 0; i <=180; i++) { 
    
    
    myservo.write(i);              
                delay(10); 
                        
    }
  
}

int calculateDistance(){ 
  int duration;
  int distance;
  digitalWrite(TrigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH); 
  distance= duration*0.034/2;
  return distance;
}

void ScreenTitleChange(char StringToDisplay[],int Column,int Row){

 lcd.clear();
 lcd.setCursor(Column, Row);
 lcd.print(StringToDisplay);

}

void Arrow(char UpDownBoth[]){
if (UpDownBoth=='Up'){
  lcd.setCursor(15, 0);
    lcd.write(byte(0));
}
else if(UpDownBoth=='Down'){
  lcd.setCursor(15, 1);
    lcd.write(byte(1));
}
else if(UpDownBoth=='Both'){
  lcd.setCursor(15, 0);
    lcd.write(byte(0));
    lcd.setCursor(15, 1);
    lcd.write(byte(1));

}
}

void LookForCard() 
{

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "83 61 9A 16") 
  {
    Serial.println("Authorized access");
    AuthorizedAccess=true;
    Screen = Menu1Screen;
    Serial.println();
   
  }
 
 else   {
    Serial.println(" Access denied");
    Screen = ActivatedAlarmScreen;
    
  }
} 
