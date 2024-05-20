// Library
#include <Adafruit_Fingerprint.h> //Adafruit_Fingerprint Sensor Library
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

//library


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
byte accessUID[4] = {0xCA, 0x89, 0xD8, 0x3F};

int getFingerprintIDez();
int RedLed = 5;
int GreenLed = 4;
int pin4 = 6;
int pin3 = 7;



LiquidCrystal_I2C lcd(0x27,20,4);

SoftwareSerial mySerial(2, 3);  //activates serial communication on pin 2 & 3 Fingerprint scanner
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


//setup
void setup()
{
  lcd.init(); 
  lcd.backlight();
  //pinMode(signalPin, OUTPUT);
  //pinMode(signalPin1, OUTPUT);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  //Serial.begin(9600);  // initialize the serial communications:
  pinMode(GreenLed, OUTPUT);  
  pinMode(RedLed, OUTPUT);  
  pinMode(pin4, INPUT);
  pinMode(pin3, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  //pinMode(pinB, INPUT);
  //digitalWrite(pinB, HIGH)
  //Serial.begin(9600);
  
  //pinMode(Buzzer, OUTPUT);  
  finger.begin(57600);  
  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);
  //digitalWrite(Buzzer, LOW);
}
//setup


void loop(){  // run over and over again
  
  getFingerprintID();
  delay(100);


// Button Block
//open
  if (digitalRead(pin4)==HIGH){
    digitalWrite(A0, HIGH);
    
    delay(500);
    for (int i=0; i<=1; i++)
  {
   
    digitalWrite(GreenLed, HIGH);  //red if opened
    //digitalWrite(Buzzer, HIGH);  
    delay(100);
    digitalWrite(GreenLed, LOW); 
    //digitalWrite(Buzzer, LOW);  
    delay(100);
  }
  digitalWrite(A0, LOW);
  
  
    //buzzred();
    //count++;
  }
//close
  if (digitalRead(pin3)==HIGH){
    digitalWrite(A1, HIGH);
    delay(500);
    for (int i=0; i<=1; i++)
    {
      
    digitalWrite(RedLed, HIGH);  //green if secured
    //digitalWrite(Buzzer, HIGH);  
    delay(100);
    digitalWrite(RedLed, LOW); 
    //digitalWrite(Buzzer, LOW);  
    delay(100);
    //buzzgreen();
    //count++;
    }
    digitalWrite(A1, LOW);
  }
//end

//Touchpad True False

  if (digitalRead(A3) == HIGH){
    buzzgreen();
    delay(1000);
  }

  if (digitalRead(A2)==HIGH){
    buzzred();
    //delay(1000);
  }



//RFID Scanner reading and Comparaing
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  if(mfrc522.uid.uidByte[0] == accessUID[0] && mfrc522.uid.uidByte[1] == accessUID[1] && mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3]){
      buzzgreen();
  }else{
    buzzred();
  }
  mfrc522.PICC_HaltA();
}


//Led Buzzer Code Block
uint8_t buzzred(){
  
  digitalWrite(A1, HIGH);
  delay(1000);
  for (int i=0; i<=11; i++)
  {
    if (i == 3) delay(300);
    if (i == 6) delay(300);
    if (i == 9) delay(300);
    digitalWrite(RedLed, HIGH);  
    //digitalWrite(Buzzer, HIGH);  
    delay(100);
    digitalWrite(RedLed, LOW); 
    //digitalWrite(Buzzer, LOW);  
    delay(100);
  }
  digitalWrite(A1, LOW);
  lcd.clear();
}

uint8_t buzzgreen(){
  
  digitalWrite(A0, HIGH);
  delay(1000);
  //digitalWrite(GreenLed, LOW);
  for (int i=0; i<=1; i++)
  {
    
  digitalWrite(GreenLed, HIGH);  
  //digitalWrite(Buzzer, HIGH);  
  delay(100);
  digitalWrite(GreenLed, LOW); 
  //digitalWrite(Buzzer, LOW);  
  delay(100);
  
  }
  
  digitalWrite(A0, LOW);
  delay(3000);
  lcd.clear();
}
//Led Buzzer Code block ends

//Fingerprint Scanner code Block


uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();

  switch (p)
  {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_IMAGEFAIL:
      return p;
    default:
      return p;
  }

  p = finger.image2Tz();
  switch (p)
  {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }

  // match finger with stored ID#
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK)
  {
    digitalWrite(A0, HIGH);
    buzzgreen();
    delay(100);
    digitalWrite(A0, LOW);
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    digitalWrite(A1, HIGH);
    buzzred();
    delay(100);
    digitalWrite(A1, LOW);
    
        
    return p;
  }
  else
  {
    return p;
  }

}

// returns ID#
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

}

//fingerprint Scanner code ends