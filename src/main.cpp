#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>

// high level selections
int modeSource1 = 0;
int modeSource2 = 0;
int selectedOutput = 0;
// possible selections
//String modes[5] = {"DC", "SINE", "SQUARE", "TRIANGLE", "SAWTOOTH"};
//String settings[6] = {"Voltage Offset", "Amplitude", "Frequency", "Duty Cycle", "Phase"};


// Tried C++ object oriented, it doesn't work like most other OO languages and its more difficult to use. Gonna use traditional C structs instead.
/*class DCsettings {
  public:
  String name = "DC"; // name of mode
  String settingNames[1] = { "Voltage Offset" } ;
  float voltageOffset = 0.0f;
};

class settings : DCsettings {
  private:
  String settingNames[5] = {"Voltage Offset", "Amplitude", "Frequency", "Duty Cycle", "Phase"};
  float Amplitude;
  float Frequency;
  float DutyCycle;
  float Phase;

  public:
  settings() {}
  settings(String setupName, float amplitude, float freq, float duty, float phase) {
    name = setupName;
    Amplitude = amplitude;
    Frequency = freq;
    DutyCycle = duty;
    Phase = phase;
  }
};*/


typedef struct FunctionGenSettings {
  String name; // set in setup function
  String options[5] = {"Voltage Offset", "Amplitude", "Frequency", "Duty Cycle", "Phase"};
  float data[5] = {0,1,1000,0,0}; // IN ORDER - Voltage Offset, Amplitude, Frequency, Duty Cycle, Phase
  String suffix[5] = {"V","Vpp", "Hz", "%", "deg"};
  int selection = 0;
} settings;

float DCVoltage1 = 0;
float DCVoltage2 = 0;
settings functionData1[4];
settings functionData2[4];
// setup the data for each


// LCD SETUP
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// SIMPLE I/O

int modeSelectButton = 8;
int selectionButton = 9;
int outputSelectButton = 10;
int lowerDigitButton = 6;
int higherDigitButton = 7;

int previousModeButton = 0;
int previousSelectButton = 0;
int previousOutputButton = 0;
int previousLowerDigitButton = 0;
int previousHigherDigitButton = 0;

// More complex IO
// ROTARY ENCODER
int counter = 0;
int clickState = 0;
int previousClickState = 0;
int scrollHigher = 24;
int scrollLower = 22;


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
//void otherOutput();
void loopThruOptions();
int debouncedInput(int input);
void statusLED(String colour);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);

  pinMode(modeSelectButton, INPUT);
  pinMode(selectionButton, INPUT);
  pinMode(outputSelectButton, INPUT);

  // rotary encoder
  pinMode(scrollHigher, INPUT);
  pinMode(scrollLower, INPUT);
  pinMode(lowerDigitButton, INPUT);
  pinMode(higherDigitButton, INPUT);

  previousClickState = digitalRead(scrollLower);

  // STATUS LED
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  // FILL IN ALL DATA FOR DIFFERENT SECTIONS
  functionData1[0].name = functionData2[0].name = "SINE";
  functionData1[1].name = functionData2[1].name = "SQUARE";
  functionData1[2].name = functionData2[2].name = "TRIANGLE";
  functionData1[3].name = functionData2[3].name = "SAWTOOTH";

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //loopThruOptions();
  selectionButtons();
  LCD_Display();

}

void LCD_Display() {
  lcd.setCursor(0,0);
  lcd.print("Output: " + (String)(selectedOutput + 1));
  lcd.setCursor(0,1);
  if(selectedOutput == 0) {
      if(modeSource1 > 0) { // function generator
        lcd.print("Mode: " + functionData1[modeSource1 - 1].name);
        lcd.setCursor(0,2);
        lcd.print("Opt: " + (String)functionData1[modeSource1 - 1].options[functionData1[modeSource1 - 1].selection]);
        lcd.setCursor(0,3);
        if(functionData1[modeSource1 - 1].selection != 4) {
            lcd.print("Value: " + (String)functionData1[modeSource1 - 1].data[functionData1[modeSource1 - 1].selection] + " " + (String)functionData1[modeSource1 - 1].suffix[functionData1[modeSource1 - 1].selection]);
        } else {
            lcd.print("Value: " + (String)functionData1[modeSource1 - 1].data[functionData1[modeSource1 - 1].selection] + " " + ((char) 223));
        }
        
      } else if(modeSource1 == 0) {
        lcd.print("Mode: DC");
        lcd.setCursor(0,2);
        lcd.print("Opt: Voltage");
        lcd.setCursor(0,3);
        lcd.print("Value: " + (String)DCVoltage1 + " V");
      }
  } else if(selectedOutput == 1) {
      if(modeSource2 > 0) { // function generator
        lcd.print("Mode: " + functionData2[modeSource2 - 1].name);
        lcd.setCursor(0,2);
        lcd.print("Opt: " + (String)functionData2[modeSource2 - 1].options[functionData2[modeSource2 - 1].selection]);
        lcd.setCursor(0,3);
        if(functionData1[modeSource1 - 1].selection != 4) {
          lcd.print("Value: " + (String)functionData2[modeSource2 - 1].data[functionData2[modeSource2 - 1].selection] + " " + (String)functionData2[modeSource2 - 1].suffix[functionData2[modeSource2 - 1].selection]);
        } else {
            lcd.print("Value: " + (String)functionData2[modeSource2 - 1].data[functionData2[modeSource2 - 1].selection] + " " + ((char) 223));
        }
      } else if(modeSource2 == 0) {
        lcd.print("Mode: DC");
        lcd.setCursor(0,2);
        lcd.print("Opt: Voltage");
        lcd.setCursor(0,3);
        lcd.print("Value: " + (String)DCVoltage2 + " V");
      }
  }
  
  
}

/*void loopThruOptions() {
    delay(4000);
    if(mode < 4) {
        mode++;
    } else {
        lcd.clear();
        mode = 0;
    } 
}*/

void selectionButtons() {
    int modeButton = digitalRead(modeSelectButton);
    int selectButton = digitalRead(selectionButton);
    int outputButton = digitalRead(outputSelectButton);
    if(selectedOutput == 0) {
        if(modeButton == HIGH && previousModeButton == LOW) {
          delay(50);
          if(modeSource1 < 4) {
            modeSource1++;
          } else {
            modeSource1 = 0;
            lcd.clear(); 
          }
          
        }
        if(modeSource1 != 0) {
          if(selectButton == HIGH && previousSelectButton == LOW) {
            delay(50);
            if(functionData1[modeSource1 - 1].selection < 4) {
              functionData1[modeSource1- 1].selection ++;
              lcd.clear();
            } else {
              functionData1[modeSource1 - 1].selection  = 0;
              lcd.clear();
            }
          }
        } else {
          if(selectButton == HIGH) {
            delay(50);
            statusLED("Red");
          } else {
            statusLED("Off");
          }
        }
    } else if(selectedOutput == 1) {
        if(modeButton == HIGH && previousModeButton == LOW) {
          delay(50);
          if(modeSource2 < 4) {
            modeSource2++;
          } else {
            lcd.clear();
            modeSource2 = 0;
          } 
          
        }
        if(modeSource2 != 0) {
          if(selectButton == HIGH && previousSelectButton == LOW) {
            delay(50);
            if(functionData2[modeSource2 - 1].selection < 4) {
              functionData2[modeSource2- 1].selection ++;
              lcd.clear();
            } else {
              functionData2[modeSource2 - 1].selection  = 0;
              lcd.clear();
            }
          }
        } else {
          if(selectButton == HIGH) {
            delay(50);
            statusLED("Red");
          } else {
            statusLED("Off");
          }
        }
    }
    
    
    if(outputButton == HIGH && previousOutputButton == LOW) {
      delay(50);
      lcd.clear();
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