#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>

int mode = 0;
int selection = 0;
int selectedOutput = 0;
String modes[5] = {"DC", "SINE", "SQUARE", "TRIANGLE", "SAWTOOTH"};
String settings[6] = {"Voltage Offset", "Amplitude", "Frequency", "Duty Cycle", "Phase"};
// LCD SETUP
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// SIMPLE I/O
int output1LED = 6;
int output2LED = 7;

int modeSelectButton = 8;
int selectionButton = 9;
int outputSelectButton = 10;

int previousModeButton = 0;
int previousSelectButton = 0;
int previousOutputButton = 0;

/* SETTINGS

- MODE 1 - DC Power Supply - DC Output
 - Voltage Offset (Voltage)
 - Current limit (if possible)

- Mode 2 - Function Generator
  - Waveform - SINE, SQUARE, TRIANGLE, SAWTOOTH, etc.
  - Voltage Offset - DC Voltage offset
  - Amplitude
  - Frequency
  - 

  NEW FOR DUE AND BIGGER LCD
  - Can choose output on LCD
  - current output setting shown aswell
*/

void LCD_Display();
void selectionButtons();
void otherOutput();
int debouncedInput(int input);
void statusLED(String colour);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);

  pinMode(modeSelectButton, INPUT);
  pinMode(selectionButton, INPUT);
  pinMode(outputSelectButton, INPUT);

  // SELECTED OUTPUT
  pinMode(output1LED, OUTPUT);
  pinMode(output2LED, OUTPUT);

  // STATUS LED
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  selectionButtons();

  LCD_Display();
  otherOutput();

}

void LCD_Display() {
  lcd.setCursor(0,0);
  lcd.print("Output: " + (String)selectedOutput);
  lcd.setCursor(0,1);
  lcd.print("Mode: " + modes[mode]);
  lcd.setCursor(0,2);
  lcd.print("Setting: " + settings[selection]);
  lcd.setCursor(0,3);
  lcd.print("Value: 10000 Hz");
}

void otherOutput() {
  digitalWrite(output1LED, !selectedOutput);
  digitalWrite(output2LED, selectedOutput);
  

}

void selectionButtons() {
    int modeButton = digitalRead(modeSelectButton);
    int selectButton = digitalRead(selectionButton);
    int outputButton = digitalRead(outputSelectButton);
    if(modeButton == HIGH && previousModeButton == LOW) {
      delay(50);
      if(mode < 4) {
        mode++;
      } else {
        lcd.clear();
        mode = 0;
      } 
    }
    if(mode != 0) {
      if(selectButton == HIGH && previousSelectButton == LOW) {
        delay(50);
        if(selection < 4) {
          selection++;
          if((selection == 1) || (selection == 4)) {
            lcd.clear();
          }
        } else {
          selection = 0;
          lcd.clear();
        }
      }
    } else {
      selection = 0;
      if(selectButton == HIGH) {
        delay(50);
        statusLED("Red");
      } else {
        statusLED("Off");
      }
    }
    
    if(outputButton == HIGH && previousOutputButton == LOW) {
      delay(50);
      selectedOutput = !selectedOutput;
    }
    delay(50);
    previousModeButton = modeButton;
    previousSelectButton = selectButton;
    previousOutputButton = outputButton;
}

void statusLED(String colour) {
  if(colour == "Red") {
    digitalWrite(A0,HIGH);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
  } else if(colour == "Green") {
    digitalWrite(A0,LOW);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  } else if(colour == "Blue") {
    digitalWrite(A0,LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  } else if(colour == "Off") {
    digitalWrite(A0,LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
  }
}