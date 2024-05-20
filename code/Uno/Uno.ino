
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include<UltraDistSensor.h>
#include <Servo.h>

#define Password_Length 2
#define echoPin 12                         // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
#define trigPin 13                       // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
long duration, distance;



//int signalPin = 12;
int green = 14;
int red = 15;

Servo servo;


char Data[Password_Length]; 
char Master[Password_Length] = "*"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27,20,4); 
UltraDistSensor mysensor;
float reading; 

void setup(){
  lcd.init(); 
  lcd.backlight();
  Serial.begin(9600);
  //pinMode(signalPin, OUTPUT);
  //digitalWrite(Alert, LOW);
  
  mysensor.attach(12,13);//Trigger pin , Echo pin
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, OUTPUT);  
  pinMode(A3, OUTPUT);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(10);


}

void loop(){
  
  lcd.setCursor(0,0);
  lcd.print("----Welcome!----");
  //digitalWrite(A0, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
   distance = duration / 58.2;
   if (distance <= 20) {
      digitalWrite(A0, HIGH);
      Serial.println(distance);
   }
  Serial.print(distance);
  //String disp = String(distance);

  //Serial.print(distance);
  
  delay(1000);


// switch end

  customKey = customKeypad.getKey();
  if (customKey){
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      lcd.print("Correct");
      digitalWrite(A2, HIGH);
      servo.write(90);
      
      //digitalWrite(signalPin, HIGH); 
      delay(5000);
      digitalWrite(A2, LOW);
      //servo.write(0);
      //digitalWrite(signalPin, LOW);
      }
    else{
      
      digitalWrite(A3, HIGH);
      delay(100);
      //digitalWrite(Alert, HIGH);
      lcd.print("Incorrect");
      delay(5000);
      //digitalWrite(Alert, LOW);
      digitalWrite(A3, LOW);
      //lcd.print("Incorrect");
      }
    delay(1000);
    lcd.clear();
    clearData();  
  }
  if (digitalRead(A1)){
    lcd.clear();
    lcd.print("Open");
    Serial.println("Access Granted");
    delay(1000);
    servo.write(90);
    lcd.clear();
    lcd.print("----Welcome!----");
  }
  if (digitalRead(11) == HIGH){
    lcd.clear();
    lcd.print("Closed");
    Serial.println("Access Denied");
    servo.write(0);
    delay(1000);
    lcd.clear();
    lcd.print("----Welcome!----");
  }
  reading1();


}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}


//Switch check
uint8_t reading1(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  if (distance>20) {
    delay(5000);
    //reading=mysensor.distanceInCm();
    if (distance>20 || distance==0){
      digitalWrite(A0, LOW);
    }
  }
}