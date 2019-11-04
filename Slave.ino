//Alec Mai, Navidur Rahman
//Slave Arduino
//This arduino receives input from the Master arduino and generates questions as well as
//evaluating whether or not the answer is correct

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Defines tones used for the piezo
#define R_C4  262
#define R_G3  196
#define R_A3  220
#define R_B3  247

#define W_C4  142
#define W_G3  100
#define W_A3  120
#define W_B3  117

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);  

int r1, r2, realAns, inpAns, type;  //variables for the random question generator

//pins for the rgb-LED
int redPin = 10;
int bluePin = 6;
int greenPin = 9;

int key = 0;    //"key" holds the user inputted number
int input = 0;    //”input” holds the interpreted input from the user
int enterState = 0; //”enterState” holds whether or not the user has hit an enter key
int dipState = 0; //”dipState” holds the state of the dip switch
bool easy = true; //”easy” holds whether or not the questions will be easy or hard
byte x = 0;   //used to receive input
String sign = " + ";
int right = 0;
int total = 0;
int percRight = 0;

//the songs used by the piezo
int melodyR[] = {R_C4, R_G3, R_G3, R_A3, R_G3, 0, R_B3, R_C4};
int melodyW[] = {W_C4, W_G3, W_G3, W_A3, W_G3, 0, W_B3, W_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

void setup() {
  pinMode(redPin, OUTPUT);    //sets “redPin” to output
  pinMode(greenPin, OUTPUT);  //sets “greenPin” to output
  pinMode(bluePin, OUTPUT);   //sets “bluePin” to output
  Wire.begin(8);      //sets up I2C
  Wire.onReceive(receiveEvent); //sets what to do when it recives something from I2C
  lcd.begin(16,2);       // set up the LCD's number of columns and rows:
  lcd.backlight();
  lcd.print("GNS");
  Serial.begin(9600);     //beings the console
  randomSeed(analogRead(0));  //sets up a random number generator
}

void loop() {
  lcd.setCursor(0, 0);      // set the cursor to column 0, line 1

//randomly generates questions

  if(easy){
    r1 = random(1, 10);
    r2 = random(1, 10);
  }
  else{
    r1 = random(11, 100);
    r2 = random(11, 100);
  }

  type = random(0, 3);
  if(type == 0){
    int or1 = r1;
    int or2 = r2;
    r1 = max(or1, or2);
    r2 = min(or1, or2);
    sign = " - ";
    realAns = r1 - r2;      //determines the answer to the question
  }
  else if(type == 1){
    sign = " + ";
    realAns = r1 + r2;      //determines the answer to the question
  }
  else{
    sign = " X ";
    if(r1 == 18 or r1 == 37){
      r1++;
    }
    realAns = r1 * r2;      //determines the answer to the question
  }

 
  inpAns;     //resets the input thing
 
//displays the question on the LCD
  lcd.print(r1);
  lcd.print(sign);
  lcd.print(r2);
  lcd.println("?");
 
//prints the question to the console
  Serial.print(r1);
  Serial.print(sign);
  Serial.print(r2);
  Serial.println("?");
 
  while(true){
//waits for the user to fully input
    while(enterState == 0){
      Serial.print("");
      }
    inpAns = input;
    if(input == 666){
      lcd.clear();
      lcd.setCursor(0, 0);
      if(total == 0){
        percRight = 0;
      }
      else{
        percRight = 100*right/total;
      }
      lcd.print("U got ");
      lcd.print(percRight);
      lcd.print("% right");
      lcd.setCursor(0, 1);
      lcd.print("Congratulations!");
      break;
    }

    enterState = 0;   //resets the enter key

//displays the user’s input
    lcd.setCursor(0, 1);
    lcd.println(inpAns);

//displays whether or not the user got the question correct
    if(inpAns == realAns){
      right++;
      total++;
      setColor(0, 255, 0);
      playNoiseR();
      setColor(0, 0, 0);
     
      lcd.clear();
      input = 0;
      inpAns = 0;
      break;
    }
    else{
      input = 0;
      total++;
      setColor(255, 0, 0);
      playNoiseW();
      setColor(0, 0, 0);

      input = 0;
      inpAns = 0;
    }
   
  }
  delay(2500);
}

//what to do when the Arduino receives input
void receiveEvent(int bytes){
//reads the input
  key = Wire.read();
  enterState = Wire.read();
  dipState = Wire.read();
 
//only changes the input if it is not an enter key
  if(enterState == 0){
    input *= 10;
    input += (key - 48);
  }
 
//changes the difficulty
  if(dipState == 0){
    easy = true;
  }
  else{
    easy = false;
  }
}

//sets the color of the rgb LED
void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

//plays the song for a correct answer
void playNoiseR(){
  for(int i = 0; i < 8; i++){
    int length = 1000/noteDurations[i];
    tone(8, melodyR[i], length);
   
    int pause = length * 1.3;
    delay(pause);
    noTone(8);
  }
}

//plays the song for a wrong answer
void playNoiseW(){
  for(int i = 0; i < 8; i++){
    int length = 1000/noteDurations[i];
    tone(8, melodyW[i], length);
   
    int pause = length * 1.3;
    delay(pause);
    noTone(8);
  }
}
