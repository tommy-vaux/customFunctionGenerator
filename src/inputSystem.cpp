#include "inputSystem.h"

// FROM HEADER

#define MAX_AMPLITUDE 15
#define MAX_AMPLITUDE_DC 30

// the maximum frequencies that can be achieved by the function generator at given res due to CPU limits (in Hz).
#define MAX_120 1000
#define Max_60 2000
#define Max_30 5000
#define Max_15 10000

// PINS FOR INPUT
#define modePin 44
#define selectPin 50
#define outputPin 40
#define leftArrow 28
#define rightArrow 30
#define rotaryUp 7
#define rotaryDown 8
#define rotaryButton 9

int output = 0;
int multiplier = 1; //1^multiplier, so 1^1 = 1, 1^2 = 10, 1^-1 = 0.1. In this case we won't treat 0 as a valid multiplier.

// for source 1
int mode1 = 0;
int selection1 = 0;


// for source 2
int mode2 = 0;
int selection2 = 0;

int displayMode = 0;
int displaySelection = 0;


// LCD SETUP
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// for the display
String modes[] = {"DC", "SINE", "SQUARE", "TRIANGLE", "SAWTOOTH"};
String arbitaryWaveSettings[] = {"Voltage Offset", "Amplitude", "Frequency", "Phase"};
String dcWaveSettings[] = {"Voltage", "Use Offset"};
String sqWaveSettings[] = {"Voltage Offset", "Amplitude", "Frequency", "Duty Cycle"};

String arbitaryWaveSuffix[] = {"V", "Vpp", "Hz", "deg"};
String dcWaveSuffix[] = {"V", ""};
String sqWaveSuffix[] = {"V", "Vpp", "Hz", "%"};

DefaultSettings waveformData1[4];
DefaultSettings waveformData2[4];
DCSettings dcData1;
DCSettings dcData2;

//END

// note really should have been called IO system

void cleanLCD(int row) {
    lcd.setCursor(0, row);
    lcd.print("                    ");
    lcd.setCursor(0, row);
}

void DisplayOutput() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Output: " + (String)(output + 1));
}

void simpleDisplay() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Output: " + (String)output);
    if(output) {
        lcd.setCursor(0,1);
        lcd.print("Mode: " + (String)mode2);
        lcd.setCursor(0,2);
        lcd.print("Selection: " + (String)selection2);
        lcd.setCursor(0,3);
        lcd.print("No Value");
    } else {
        lcd.setCursor(0,1);
        lcd.print("Mode: " + (String)mode1);
        lcd.setCursor(0,2);
        lcd.print("Selection: " + (String)selection1);
        lcd.setCursor(0,3);
        lcd.print("Multiplier: " + (String)multiplier);
    }
    
}

void displayV2() {
    lcd.noBlink();
    // determine correct options
    if(output) {
        displayMode = mode2;
        displaySelection = selection2;
    } else {
        displayMode = mode1;
        displaySelection = selection1;
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Output: " + (String)(output + 1));
    lcd.setCursor(0,1);
    lcd.print("Mode: " + modes[displayMode]);
    lcd.setCursor(0,2);
    if(displayMode == 0) {
        lcd.print("Opt: " + dcWaveSettings[displaySelection]);
    } else if(displayMode == 2) {
        lcd.print("Opt: " + sqWaveSettings[displaySelection]);
    } else {
        lcd.print("Opt: " + arbitaryWaveSettings[displaySelection]);
    }
    lcd.setCursor(0,3);
    if(displayMode == 0) {
        if(displaySelection == 1) {
            if(output) {
                if(dcData2.data[displaySelection] == 1) {
                    lcd.print("Value: Yes");
                } else {
                    lcd.print("Value: No");
                }
            } else {
                if(dcData1.data[displaySelection] == 1) {
                    lcd.print("Value: Yes");
                } else {
                    lcd.print("Value: No");
                }
            }
        } else {
            if(output) {
                lcd.print("Value: " + (String)dcData2.data[displaySelection] + " " + dcWaveSuffix[displaySelection]);
            } else {
                lcd.print("Value: " + (String)dcData1.data[displaySelection] + " " + dcWaveSuffix[displaySelection]);
            }
        }
        
        
    } else if(displayMode == 2) {
        if(output) {
            lcd.print("Value: " + (String)waveformData2[displayMode - 1].data[displaySelection] + " " + sqWaveSuffix[displaySelection]);
        } else {
            lcd.print("Value: " + (String)waveformData1[displayMode - 1].data[displaySelection] + " " + sqWaveSuffix[displaySelection]);
        }
    } else {
        if(output) {
            lcd.print("Value: " + (String)waveformData2[displayMode - 1].data[displaySelection] + " " + arbitaryWaveSuffix[displaySelection]);
        } else {
            lcd.print("Value: " + (String)waveformData1[displayMode - 1].data[displaySelection] + " " + arbitaryWaveSuffix[displaySelection]);
        }
    }
    lcd.setCursor(7,3); // move cursor to start of float value
    lcd.blink();


}

void setupInputCursor(int mode) {
    if(output) {

    } else {

    }
    if(multiplier < 0) {

    }
    
}

/*void DisplayMode() {
    cleanLCD(1);
    if(output) {
        if(mode2 == 0) {
            lcd.print("Mode: " + dcData2.name);
        } else {
            lcd.print("Mode: " + waveformData2[selection2].name);
        }
        
    } else {
        if(mode1 == 0) {
            lcd.print("Mode: " + dcData1.name);
        } else {
            lcd.print("Mode: " + waveformData1[selection1].name);
        }
    }
}*/

/*void DisplaySelection() {
    cleanLCD(2);
    if(output) {
        if(mode2 == 0) {
            lcd.print("Opt: " + dcWaveSettings[selection2]);
        } else if(mode2 == 2) {
            lcd.print("Opt: " + sqWaveSettings[selection2]);
        } else {
            lcd.print("Opt: " + arbitaryWaveSettings[selection2]);
        }
    } else {
        if(mode1 == 0) {
            lcd.print("Opt: " + dcWaveSettings[selection1]);
        } else if(mode1 == 2) {
            lcd.print("Opt: " + sqWaveSettings[selection1]);
        } else {
            lcd.print("Opt: " + arbitaryWaveSettings[selection1]);
        }
    }
}

void DisplayData() {
    cleanLCD(3);
    if(output) {
        if(mode2 == 0) {
            lcd.print("Value: " + (String)dcData2.data[selection2] + " " + dcWaveSuffix[selection2]);
        } else if (mode2 == 2) {
            lcd.print("Value: " + (String)waveformData2[selection2].data[selection2] + " " + sqWaveSuffix[selection2]);
        } else {
            lcd.print("Value: " + (String)waveformData2[selection2].data[selection2] + " " + arbitaryWaveSuffix[selection2]);
        }
        
    } else {
        if(mode2 == 0) {
            lcd.print("Value: " + (String)dcData1.data[selection1] + " " + dcWaveSuffix[selection1]);
        } else if (mode2 == 2) {
            lcd.print("Value: " + (String)waveformData1[selection1].data[selection1] + " " + sqWaveSuffix[selection1]);
        } else {
            lcd.print("Value: " + (String)waveformData1[selection1].data[selection1] + " " + arbitaryWaveSuffix[selection1]);
        }
    }
}*/


void ChangeMode() { // output mode (Sine, DC, etc.)
    static unsigned long last_change_mode_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_change_mode_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        if(output) {// output 2
            if(mode2 < 4) {
                mode2++;
            } else {
                mode2 = 0;
            }

        } 
        else { // output 1
            if(mode1 < 4) {
                mode1++;
            } else {
                mode1 = 0;
            }
        }

        displayV2();
    }
    last_change_mode_time = interrupt_time;
}

void ChangeSelection() { // control selection (Amplitude, frequency, etc.)
    static unsigned long last_change_sel_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_change_sel_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        if(output) {// output 2
            if((selection2 < 3 && mode2 != 0)  || (selection2 < 1 && mode2 == 0)) {
                selection2++;
            } else {
                selection2 = 0;
            }
        } 
        else { // output 1
            if((selection1 < 3 && mode1 != 0) || (selection1 < 1 && mode1 == 0)) {
                selection1++;
            } else {
                selection1 = 0;
            }
        }

        displayV2();

    }
    last_change_sel_time = interrupt_time;
}

void ChangeOutput() { // selected output (both outputs should still work simultaneously)
    static unsigned long last_change_output_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_change_output_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        output = !output; // either 0 or 1

        displayV2();
    }
    last_change_output_time = interrupt_time;
}

void LeftArrow() { // increase the multiplier
    static unsigned long last_change_output_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_change_output_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        multiplier++;
        // still need to update the display here ---------------------------------------------------------------------------

    }
    last_change_output_time = interrupt_time;
}

void RightArrow() { // reduce the multiplier
    static unsigned long last_change_right_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_change_right_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        multiplier--;
        // still need to update the display here ---------------------------------------------------------------------------

    }
    last_change_right_time = interrupt_time;
}

void RotaryInput() { // for increasing/decreasing values on the display
        
    // PERFORM BUTTON FUNCTIONALITY HERE.
    // still need to update the display here ---------------------------------------------------------------------------
    if(digitalRead(rotaryUp) == digitalRead(rotaryDown)) {
        //increasing
        if(output) {
            if(mode2 == 0) {
                dcData2.data[selection2]++;
            } else {
                waveformData2[mode2].data[selection2]++;
            }
        } else {
            if(mode1 == 0) {
                dcData1.data[selection1]++;
            } else {
                waveformData1[mode1].data[selection1]++;
            }
        }
    } else {
        //decreasing
        if(output) {
            if(mode2 == 0) {
                dcData2.data[selection2]++;
            } else {
                waveformData2[mode2].data[selection2]--;
            }
        } else {
            if(mode1 == 0) {
                dcData1.data[selection1]++;
            } else {
                waveformData1[mode1].data[selection1]--;
            }
        }
    }

    displayV2();
}

void RotaryButton() { // idk what I will use this for, but i might make it an option.
    static unsigned long last_rotary_button_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_rotary_button_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        // still need to update the display here ---------------------------------------------------------------------------
        simpleDisplay();

    }
    last_rotary_button_time = interrupt_time;
}

void setupIO() { // this is defined in the header as it is visible to the main script
    lcd.begin(20,4);

    pinMode(modePin, INPUT);
    pinMode(selectPin, INPUT);
    pinMode(outputPin, INPUT);

    pinMode(rotaryUp, INPUT_PULLUP);
    pinMode(rotaryDown, INPUT_PULLUP);
    pinMode(rotaryButton, INPUT);

    // input systems
    attachInterrupt(digitalPinToInterrupt(modePin),ChangeMode, RISING);
    attachInterrupt(digitalPinToInterrupt(selectPin), ChangeSelection, RISING);
    attachInterrupt(digitalPinToInterrupt(outputPin), ChangeOutput, RISING);

    // rotary encoder setup
    attachInterrupt(digitalPinToInterrupt(rotaryUp), RotaryInput, FALLING);
    attachInterrupt(digitalPinToInterrupt(rotaryButton), RotaryButton, RISING);


    pinMode(leftArrow, INPUT);
    pinMode(rightArrow, INPUT);
    pinMode(rotaryUp, INPUT);
    pinMode(rotaryDown, INPUT);

    displayV2();

}