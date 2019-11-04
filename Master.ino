//Alec Mai, Navidur Rahman
//Master Arduino
//This Arduino gets input from the user and passes it to the Slave arduino

#include <Wire.h>
#include <Keypad.h>

int key = 0;    //"key" holds the user inputted number
int dipState = 0; //”dipState” holds the state of the dip switch (used for difficulty)
int switchState = 0;  //”switchState” holds the state of the push switch (used to allow input)
int enterState = 0; //”enterState” holds whether or not the user has hit an enter key
int customKey;  //”customKey” holds what the user has inputted from the keypad

int motorPin = 3; //what pin the motor is connected to

//used to get input from the keypad
const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}};
byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 2, 13};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup() {
  Wire.begin();       //Start the I2C Bus as Master
  Serial.begin(9600);     //Start the console
 
  pinMode(11, INPUT_PULLUP);  //tells pin 11 to listen for input from a switch
  pinMode(9, INPUT_PULLUP); //tells pin 9 to listen for input from a switch
  pinMode(motorPin, OUTPUT);  //tells motor pin to output
}

void loop() {
  dipState = digitalRead(11);   //reads the state of the dip switch
  switchState = digitalRead(9); //reads the state of the push switch
   
//waits for input from the keypad
  while(true){
    customKey = customKeypad.getKey();
   
   
    if(customKey){
      break;
    }
  }
 
//turns on the motor
  analogWrite(motorPin, 255);
 
//determines whether or not the key was a number or an enter key
  if (isDigit(customKey)){
    key += customKey;
  }
  else{
    enterState = 1;
  }

//passes the inputs to the slave Arduino through I2C
  if(switchState == 0){
    Wire.beginTransmission(8);
   
    Wire.write(key);
    Wire.write(enterState);
    Wire.write(dipState);
   
    Wire.endTransmission();
  }

//resets some key variables
  key = 0;
  enterState = 0;
}
