//Libraries 
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Shifter.h>

#define SER_Pin 8 //SER_IN 14 on 74HC595
#define RCLK_Pin 9 //L_CLOCK 12 on 74HC595
#define SRCLK_Pin 10 //CLOCK 11 on 74HC595
#define NUM_REGISTERS 3 //how many registers are in the chain
//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS); 
 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address

const int ButtonPinUp = 2;
int ButtonPinUpState = 0;
const int ButtonPinDn = 3;
int ButtonPinDnState = 0;
const int MuteButtonPin = 5;
int MuteButtonPinState = 0;
const int AuxButtonPin = 4;
int AuxButtonPinState = 0;
const int fbutton = 6;
int RelaySource = 1;
int RelaySourceDigi = 1;
int RelayRec = 1;
int RecDigi = 1;
int Mute = 0;
int Mode = 0;
int Aux = 1;
int state = 0;
int old = 0;
int ButtonPoll = 0;


byte opt[8] = {
  0b00000,
  0b01110,
  0b11011,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
  };

byte coax[8] = {
0b00100,
  0b01110,
  0b11111,
  0b10001,
  0b10001,
  0b01110,
  0b00100,
  0b00100
};

byte mut[8] = {
  0b00001,
  0b00011,
  0b00111,
  0b11111,
  0b11111,
  0b00111,
  0b00011,
  0b00001
};


byte outp[8] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B11111,
  B00110,
  B00100,
  B00000
};

byte upp[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte inpp[8] {
  0b10000,
  0b10000,
  0b10100,
  0b10110,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};


byte ann[8] {

 0b11111,
 0b10001,
  0b10101,
  0b10101,
  0b10001,
  0b10101,
  0b10101,
  0b11111
};


byte dnn[8] = {
  0b11111,
  0b10011,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10011,
  0b11111
};

byte too[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000,
  0b00000
};

void setup() {      

  lcd.begin(20,4);
  lcd.backlight();
  lcd.createChar(1, outp);
  lcd.createChar(2, coax);
  lcd.createChar(3, mut);
  lcd.createChar(4, opt);
  lcd.createChar(5, upp);
  lcd.createChar(6, inpp);
  lcd.createChar(7, ann);
  lcd.createChar(8, dnn);
  lcd.createChar(9, too);
  pinMode(fbutton, INPUT);
  pinMode(ButtonPinUp, INPUT); 
  pinMode(MuteButtonPin, INPUT);
  pinMode(ButtonPinDn, INPUT);
  pinMode(AuxButtonPin, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
 
  lcd.setCursor(3,0);
  lcd.print("Digital/Analog");
  lcd.setCursor(3,1);
  lcd.print("Audio Selector");
  lcd.setCursor(4,2);
  lcd.print("Built-In DAC");
  lcd.setCursor(2,3);
  lcd.print("V2.0f (23052020)");
  delay(4000);
  lcd.clear();
}

void loop() {


 // shifter.clear(); //set all pins on the shift register chain to LOW
  shifter.write(); //send changes to the chain and display them
  //check button states
  ButtonPinUpState = digitalRead(ButtonPinUp);
  ButtonPinDnState = digitalRead(ButtonPinDn);
  MuteButtonPinState = digitalRead(MuteButtonPin);
  AuxButtonPinState = digitalRead(AuxButtonPin);
  ButtonPoll = digitalRead(fbutton);

if (AuxButtonPinState == HIGH && RelaySource == 1 && state == 1) {
    if (Aux == 0) {
      lcd.clear();   
      Aux = 1;
      digitalWrite(13, LOW);
      }                            
    else
    {
      lcd.clear();
      Aux = 0 ;
      digitalWrite(13, HIGH);  
    }
  }


if(ButtonPoll == HIGH) {
    state = old + 1;
  }

switch (state) {
   case 1:
   lcd.clear();
   lcd.setCursor(1,0);
   lcd.write(5);
   old = state; 
   break;
   case 2:
   lcd.clear();
   lcd.setCursor(1,1);
   lcd.write(5);
   old = state;
   break;
   case 3:
   lcd.clear();
   lcd.setCursor(1,2);
    lcd.write(5);  
   old = state;
   break;
   case 4:
   lcd.clear();
   lcd.setCursor(1,3);
   lcd.write(5);
   old = state;
   break;
   default:
   lcd.clear();
   old = 0;
   break;
} 

if(ButtonPinUpState == HIGH && state == 1) {
    lcd.clear();
    RelaySource = RelaySource + 1;
  }
  if(ButtonPinDnState == HIGH && state == 1) {
    RelaySource = RelaySource - 1;
  }
  if(RelaySource > 8) {  //put a max of 3 for relay number
    RelaySource = 8;
  }
  if(RelaySource < 1) {  //put a min of 1 for relay number
    RelaySource = 1;
  }
  
if(ButtonPinUpState == HIGH && state == 3) {
    lcd.clear();
    RelaySourceDigi = RelaySourceDigi + 1;
  }
  if(ButtonPinDnState == HIGH && state == 3) {
    RelaySourceDigi = RelaySourceDigi - 1;
  }
  
  if(RelaySourceDigi > 8) {  //put a max of 3 for relay number
    RelaySourceDigi = 8;
  }
  if(RelaySourceDigi < 1) {  //put a min of 1 for relay number
    RelaySourceDigi = 1;
  }
 
if(ButtonPinUpState == HIGH && state == 2) {
    lcd.clear();
    RelayRec = RelayRec + 1;
  }
  if(ButtonPinDnState == HIGH && state == 2) {
    RelayRec = RelayRec - 1;
  }
  if(RelayRec > 5) {  //put a max of 3 for relay number
    RelayRec = 5;
  }
  if(RelayRec < 1) {  //put a min of 1 for relay number
    RelayRec = 1;
  }   

  
if(ButtonPinUpState == HIGH && state == 4) {
    lcd.clear();
    RecDigi = RecDigi + 1;
  }
  if(ButtonPinDnState == HIGH && state == 4) {
    RecDigi = RecDigi - 1;
  }
  if(RecDigi > 4) {  //put a max of 3 for relay number
    RecDigi = 4;
  }
  if(RecDigi < 1) {  //put a min of 1 for relay number
    RecDigi = 1;

  }

    
  if (MuteButtonPinState == HIGH) {
    lcd.clear();
    if (Mute == 0) {     
      Mute = 1;
      digitalWrite(12, HIGH);
      }                            
    else
    {
      Mute = 0 ;
      digitalWrite(12, LOW);  
    }
  }
  

  //check relay number and activate outputs according to number

if(RelaySource == 1 && Aux == 1) {
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("Line 1");
  shifter.setPin(0, HIGH);
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  }
  else
  if(RelaySource == 1 && Aux == 0) {
  shifter.setPin(0, HIGH);
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("D");
  lcd.write(9);
  lcd.print("A:");
  }

if(RelaySource == 2) {
  shifter.setPin(0, LOW);
  shifter.setPin(1, HIGH);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);  
  lcd.print("Line 2");
  }
   
   
if(RelaySource == 3) {
  shifter.setPin(0, LOW);
  shifter.setPin(1, LOW);
  shifter.setPin(2, HIGH);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("CD 1");
  }
  
  
  if(RelaySource == 4) {
 
  shifter.setPin(0, LOW);
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, HIGH);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("CD 2");
  }
   
  if(RelaySource == 5) {
  shifter.setPin(0, LOW); 
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, HIGH);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("Tape 1");
  }
   
  if(RelaySource == 6) {
  shifter.setPin(0, LOW);
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, HIGH);
  shifter.setPin(6, LOW);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("Tape 2/DCC");;
  }
 
  if(RelaySource == 7) {
  shifter.setPin(0, LOW);
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, HIGH);
  shifter.setPin(7, LOW);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("Tape 3/DAT");
  }
 
  if(RelaySource == 8) {
  shifter.setPin(0, LOW); //set pin 1 in the chain(second pin) HIGH
  shifter.setPin(1, LOW);
  shifter.setPin(2, LOW);
  shifter.setPin(3, LOW);
  shifter.setPin(4, LOW);
  shifter.setPin(5, LOW);
  shifter.setPin(6, LOW);
  shifter.setPin(7, HIGH);
  shifter.write();
  lcd.setCursor(0,0);
  lcd.write(7);
  lcd.setCursor(2,0);
  lcd.print("MD");
  }

if(RelayRec == 1) {
  lcd.setCursor(0,1);
  lcd.write(6);
  lcd.setCursor(2,1);
  lcd.print("Source");
  shifter.setPin(8, HIGH); 
  shifter.setPin(9, LOW);
  shifter.setPin(10, LOW); 
  shifter.setPin(11, LOW);
  shifter.write();
  }

if(RelayRec == 2) {
  lcd.setCursor(0,1);
  lcd.write(6);
  lcd.setCursor(2,1);
  lcd.print("OFF");
  shifter.setPin(8, LOW); 
  shifter.setPin(9, LOW);
  shifter.setPin(10, LOW); 
  shifter.setPin(11, LOW);
  shifter.write();
  }
  
if(RelayRec == 3) {
  lcd.setCursor(0,1);
  lcd.write(6);
  lcd.setCursor(2,1);
  lcd.print("Tape 1");
  lcd.write(9);
  lcd.print("2,3");
  shifter.setPin(8, LOW); 
  shifter.setPin(9, HIGH);
  shifter.setPin(10, LOW); 
  shifter.setPin(11, LOW);
  shifter.write();
  }
  
if(RelayRec == 4) {
  lcd.setCursor(0,1);
  lcd.write(6);
  lcd.setCursor(2,1);
  lcd.print("Tape 2");
  lcd.write(9);
  lcd.print("1,3");
  shifter.setPin(8, LOW); 
  shifter.setPin(9, LOW);
  shifter.setPin(10, HIGH); 
  shifter.setPin(11, LOW);
  shifter.write();
  }

if(RelayRec == 5) {
  lcd.setCursor(0,1);
  lcd.write(6);
  lcd.setCursor(2,1);
  lcd.print("Tape 3");
  lcd.write(9);
  lcd.print("1,2");
  shifter.setPin(8, LOW); 
  shifter.setPin(9, LOW);
  shifter.setPin(10, LOW); 
  shifter.setPin(11, HIGH);
  shifter.write();
  }

if(RelaySourceDigi == 1 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Optical 1");
  }
  if (RelaySourceDigi == 1) {
  shifter.setPin(16, HIGH);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Sony 5-CD [");
  lcd.write(4);
  lcd.print("]");
  }

if(RelaySourceDigi == 2 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Optical 2");
  }
  if (RelaySourceDigi == 2) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, HIGH);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Sony MD [");
  lcd.write(4);
  lcd.print("]");
  }

if(RelaySourceDigi == 3 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Optical 3");
  }
  if (RelaySourceDigi == 3) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, HIGH);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Optical 3");
  
  }


 if(RelaySourceDigi == 4 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Optical 4");
  }
  if (RelaySourceDigi == 4) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, HIGH);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Optical 4");
  }

  if(RelaySourceDigi == 5 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Coaxial 1");
  }
  if (RelaySourceDigi == 5) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, HIGH);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Philips CD-R [");
  lcd.write(2);
  lcd.print("]");
  }

 if(RelaySourceDigi == 6 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Coaxial 2");
  }
  if (RelaySourceDigi == 6) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, HIGH);
  shifter.setPin(22, LOW);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Coaxial 2");
  }

 if(RelaySourceDigi == 7 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Coaxial 3");
  }
  if (RelaySourceDigi == 7) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, HIGH);
  shifter.setPin(23, LOW);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Coaxial 3");
  }

  if(RelaySourceDigi == 8 && RelaySource == 1 && Aux ==0) {
  lcd.setCursor(7,0);
  lcd.print("Coaxial 4");
  }
  if (RelaySourceDigi == 8) {
  shifter.setPin(16, LOW);
  shifter.setPin(17, LOW);
  shifter.setPin(18, LOW);
  shifter.setPin(19, LOW);
  shifter.setPin(20, LOW);
  shifter.setPin(21, LOW);
  shifter.setPin(22, LOW);
  shifter.setPin(23, HIGH);
  shifter.write();
  lcd.setCursor(0,2);
  lcd.write(8);
  lcd.setCursor(2,2);
  lcd.print("Coaxial 4");
  }

  if(RecDigi == 1) {
  lcd.setCursor(0,3);
  lcd.write(6);
  lcd.setCursor(2,3);
  lcd.print("Optical Out 1");
  shifter.setPin(12, HIGH); 
  shifter.setPin(13, LOW);
  shifter.setPin(14, LOW); 
  shifter.setPin(15, LOW);
  shifter.write();
  }

if(RecDigi == 2) {
  lcd.setCursor(0,3);
  lcd.write(6);
  lcd.setCursor(2,3);
  lcd.print("Optical Out 2");
  shifter.setPin(12, LOW); 
  shifter.setPin(13, HIGH);
  shifter.setPin(14, LOW); 
  shifter.setPin(15, LOW);
  shifter.write();
  }

if(RecDigi == 3) {
  lcd.setCursor(0,3);
  lcd.write(6);
  lcd.setCursor(2,3);
  lcd.print("Coaxial Out 1");
  shifter.setPin(12, LOW); 
  shifter.setPin(13, LOW);
  shifter.setPin(14, HIGH); 
  shifter.setPin(15, LOW);
  shifter.write();
  }

if(RecDigi == 4) {
  lcd.setCursor(0,3);
  lcd.write(6);
  lcd.setCursor(2,3);
  lcd.print("Coaxial Out 2");
  shifter.setPin(12, LOW); 
  shifter.setPin(13, LOW);
  shifter.setPin(14, LOW); 
  shifter.setPin(15, HIGH);
  shifter.write();
  }

  
 delay(150);
   
}
