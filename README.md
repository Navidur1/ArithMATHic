# ArithMATHic
### TEJ4M FSE
##### Navidur Rahman, Alec Mai

A game designed for children learning math that gives users randomly generated arithmetic questions. Built using Arduino, LiquidCrystal library, and I2C communication. 

The program will display a randomly generated arthmetic problem on the LCD screen, which the user must input the answer to using the keypad. There are 3 modes to choose from, Addition, Subtraction, and Multiplication. The difficulty of the questions can be adjusted to easy (1-digit integers) or hard (2-digit integers). When finished doing questions the user can exit the question section and the screen will display the percentage of questions answered correctly.

The project required the use of I2C communication between 2 different Arduino systems due to all the ports being taken up. This was done with the wire library and involves a slave and master Arduino.

Attached are pictures of the project including the 2 Arduinos, LCD screen, and keypad.

Libraries to download: Wire and LiquidCrystal_I2C.
https://www.arduino.cc/en/reference/libraries
