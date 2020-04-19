#include "inputSystem.h"

// PINS FOR INPUT
#define modePin 44
#define selectPin 50
#define outputPin 40
#define leftArrow 9
#define rightArrow 10
#define rotaryA 7
#define rotaryB 6
#define rotaryButton 8

int output = 0;
int multiplier = 0; //1^multiplier, so 10^1 = 1, 10^0 = 1, 1^-1 = 0.1. In this case we won't treat 0 as a valid multiplier.
int rotaryState;
int lastRotaryState;

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

// GETTERS FOR OTHER FUNCTIONS IN OTHER SCRIPTS
int getOutput() {
    return output;
}

int getMode(int output) {
    if(output) {
        return mode2;
    } else {
        return mode1;
    }
}

DCSettings getDCSettings(int output) {
    if(output) {
        return dcData2;
    } else {
        return dcData1;
    }
}

DefaultSettings * getWaveformSettings(int output) {
    if(output) {
        return waveformData2;
    } else {
        return waveformData1;
    }
}



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
    //lcd.setCursor(7,3); // move cursor to start of float value
    setupInputCursor();
    lcd.blink();


}

float getDisplayData() { // returns the value currently shown in the display. needed for the input cursor/multiplier to work properly.
    if(output) {
        if(mode2 == 0) {
            return dcData2.data[selection2];
        } else {
            return waveformData2[mode2].data[selection2];
        }
    } else {
        if(mode1 == 0) {
            return dcData1.data[selection1];
        } else {
            return waveformData1[mode1].data[selection1];
        }
    }
}

void setupInputCursor() { // weird glitch - it is one off (over the period) with voltage offset on sine & square, but its fine on sawtooth and triangle. Data also seems to be applying in wrong places.
    float data = getDisplayData();
    short position = ((String)data).length() - 4; // figures out the position along the data the cursor should be at to correctly be at the first positive multiplier
    short startPos = 7; // this is the position where the cursor starts, (ie the first data position on the display, the largest multiple of the current number)
    short finalCursorPos = startPos;
    if(multiplier < 0) {
        if(data < 0) {
            finalCursorPos = startPos + position - multiplier + 2;
        } else {
            finalCursorPos = startPos + position - multiplier + 1;
        }
        
    } else {
        if(data < 0) {
            finalCursorPos = startPos + position - multiplier + 1;
        } else {
            finalCursorPos = startPos + position - multiplier;
        }
        
    }
    lcd.setCursor(finalCursorPos, 3);
    
}

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
                selection2 = 0;
            }

        } 
        else { // output 1
            if(mode1 < 4) {
                mode1++;
            } else {
                mode1 = 0;
                selection1 = 0;
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
        updateMultiplier();
        displayV2();

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
        updateMultiplier();
        displayV2();

    }
    last_change_right_time = interrupt_time;
}

void updateMultiplier() { // applies min/max values to the multiplier
    if(output) { // need cause I need to know which one is selected
        if(mode2 == 0) {
            if(multiplier > MAX_MULTIPLIER_SMALL) {
                multiplier = MAX_MULTIPLIER_SMALL;
            } else if(multiplier < MIN_MULTIPLIER) {
                multiplier = MIN_MULTIPLIER;
            }
        } else {

        }
    } else {
        if(mode1 == 0) {
            if(multiplier > MAX_MULTIPLIER_SMALL) {
                multiplier = MAX_MULTIPLIER_SMALL;
            } else if(multiplier < MIN_MULTIPLIER) {
                multiplier = MIN_MULTIPLIER;
            }
        } else {
            
        }
    }

}

void RotaryInput() { // for increasing/decreasing values on the display. This needs to be run in loop unfortunately.
    rotaryState = digitalRead(rotaryA);
    if(rotaryState != lastRotaryState) {
        updateMultiplier();
        if(digitalRead(rotaryB) != rotaryState) {
            //increasing
            if(output) {
                if(mode2 == 0) {
                    if(selection2 == 0){
                        dcData2.data[selection2] += pow(10,multiplier);
                    } else {
                        dcData2.data[selection2] = !dcData2.data[selection2];
                    }
                    // force the value back down to the maximum if it goes over, or back to the minimum if it goes under
                    if(dcData2.data[1] == true && dcData2.data[0] > MAX_AMPLITUDE_DC) {
                        dcData2.data[0] = MAX_AMPLITUDE_DC;
                    } else if(dcData2.data[0] > MAX_AMPLITUDE && dcData2.data[1] == false) {
                        dcData2.data[0] = MAX_AMPLITUDE;
                    } else if(dcData2.data[0] < 0) {
                        dcData2.data[0] = 0;
                    }
                } else {
                    //if(waveformData2[mode2].data[selection2])
                    waveformData2[mode2 - 1].data[selection2] += pow(10,multiplier);
                    checkWaveformData();
                }
            } else {
                if(mode1 == 0) {
                    if(selection1 == 0){
                        dcData1.data[selection1] += pow(10,multiplier);
                    } else {
                        dcData1.data[selection1] = !dcData1.data[selection1];
                    }
                    // force the value back down to the maximum if it goes over, or back to the minimum if it goes under
                    if(dcData1.data[1] == true && dcData1.data[0] > MAX_AMPLITUDE_DC) {
                        dcData1.data[0] = MAX_AMPLITUDE_DC;
                    } else if(dcData1.data[0] > MAX_AMPLITUDE && dcData1.data[1] == false) {
                        dcData1.data[0] = MAX_AMPLITUDE;
                    } else if(dcData1.data[0] < 0) {
                        dcData1.data[0] = 0;
                    }
                } else {
                    waveformData1[mode1 - 1].data[selection1] += pow(10,multiplier);
                    checkWaveformData();
                }
            }
        } else {
            //decreasing
            if(output) {
                if(mode2 == 0) {
                    if(selection2 == 0){
                        dcData2.data[selection2] -= pow(10,multiplier);
                    } else {
                        dcData2.data[selection2] = !dcData2.data[selection2];
                    }
                    // force the value back down to the maximum if it goes over, or back to the minimum if it goes under
                    if(dcData2.data[1] == true && dcData2.data[0] > MAX_AMPLITUDE_DC) {
                        dcData2.data[0] = MAX_AMPLITUDE_DC;
                    } else if(dcData2.data[0] > MAX_AMPLITUDE && dcData2.data[1] == false) {
                        dcData2.data[0] = MAX_AMPLITUDE;
                    } else if(dcData2.data[0] < 0) {
                        dcData2.data[0] = 0;
                    }
                } else {
                    waveformData2[mode2 - 1].data[selection2] -= pow(10,multiplier);
                    checkWaveformData();
                }
            } else {
                if(mode1 == 0) {
                    if(selection1 == 0){
                        dcData1.data[selection1] -= pow(10,multiplier);
                    } else {
                        dcData1.data[selection1] = !dcData1.data[selection1];
                    }
                    // force the value back down to the maximum if it goes over, or back to the minimum if it goes under
                    if(dcData1.data[1] == true && dcData1.data[0] > MAX_AMPLITUDE_DC) {
                        dcData1.data[0] = MAX_AMPLITUDE_DC;
                    } else if(dcData1.data[0] > MAX_AMPLITUDE && dcData1.data[1] == false) {
                        dcData1.data[0] = MAX_AMPLITUDE;
                    } else if(dcData1.data[0] < 0) {
                        dcData1.data[0] = 0;
                    }
                } else {
                    waveformData1[mode1 - 1].data[selection1] -= pow(10,multiplier);
                    checkWaveformData();
                }
            }
        }
        displayV2(); // display will only be updated if a change in rotary encoder occurs.
    }
    lastRotaryState = rotaryState;
}

void checkWaveformData() {
    if(output) {
        
        // check offset
        if(waveformData2[mode2 - 1].data[0] > MAX_OFFSET) {
            waveformData2[mode2 - 1].data[0] = MAX_OFFSET;
        } else if(waveformData2[mode2 - 1].data[0] < -MAX_OFFSET) {
            waveformData2[mode2 - 1].data[0] = -MAX_OFFSET;
        }
        // check amplitude
        if(waveformData2[mode2 - 1].data[1] > MAX_AMPLITUDE) {
            waveformData2[mode2 - 1].data[1] = MAX_AMPLITUDE;
        } else if(waveformData2[mode2 - 1].data[1] < 0) {
            waveformData2[mode2 - 1].data[1] = 0;
        }
        // check frequency is greater than zero - the hardware itself will automatically limit the frequency.
        if(waveformData2[mode2 - 1].data[2] < 0){
            waveformData2[mode2 - 1].data[2] = 0;
        }

        if(mode2 != 2) { // NOT a square wave, so must be phase
            if(waveformData2[mode2 - 1].data[3] > MAX_PHASE || waveformData2[mode2].data[3] < MIN_PHASE){
                waveformData2[mode2 - 1].data[3] = 0;
            }
        } else {
            if(waveformData2[mode2 - 1].data[3] > MAX_DUTY) {
                waveformData2[mode2 - 1].data[3] = 100;
            } else if(waveformData2[mode2 - 1].data[3] < 0) {
                waveformData2[mode2 - 1].data[3] = 0;
            }
        }
    } else {
        // check offset
        if(waveformData1[mode1 - 1].data[0] > MAX_OFFSET) {
            waveformData1[mode1 - 1].data[0] = MAX_OFFSET;
        } else if(waveformData1[mode1 - 1].data[0] < -MAX_OFFSET) {
            waveformData1[mode1 - 1].data[0] = -MAX_OFFSET;
        }
        // check amplitude
        if(waveformData1[mode1 - 1].data[1] > MAX_AMPLITUDE) {
            waveformData1[mode1 - 1].data[1] = MAX_AMPLITUDE;
        } else if(waveformData1[mode1 - 1].data[1] < 0) {
            waveformData1[mode1 - 1].data[1] = 0;
        }
        // check frequency is greater than zero - the hardware itself will automatically limit the frequency.
        if(waveformData1[mode1 - 1].data[2] < 0){
            waveformData1[mode1 - 1].data[2] = 0;
        }

        if(mode1 != 2) { // NOT a square wave, so must be phase
            if(waveformData1[mode1 - 1].data[3] > MAX_PHASE || waveformData1[mode1 - 1].data[3] < MIN_PHASE){
                waveformData1[mode1 - 1].data[3] = 0;
            }
        } else {
            if(waveformData1[mode1 - 1].data[3] > MAX_DUTY) {
                waveformData1[mode1 - 1].data[3] = 100;
            } else if(waveformData1[mode1 - 1].data[3] < 0) {
                waveformData1[mode1 - 1].data[3] = 0;
            }
        }
    }
}

void RotaryButton() { // idk what I will use this for, but i might make it an option.
    static unsigned long last_rotary_button_time = 0;
    unsigned long interrupt_time = millis();
    if(interrupt_time - last_rotary_button_time > 200) {
        
        // PERFORM BUTTON FUNCTIONALITY HERE.
        // still need to update the display here ---------------------------------------------------------------------------
        displayV2();
    }
    last_rotary_button_time = interrupt_time;
}

void setupIO() { // this is defined in the header as it is visible to the main script
    lcd.begin(20,4);

    //pinMode(modePin, INPUT);
    //pinMode(selectPin, INPUT);
    //pinMode(outputPin, INPUT);

    pinMode(rotaryA, INPUT);
    pinMode(rotaryB, INPUT);

    // input systems
    attachInterrupt(digitalPinToInterrupt(modePin),ChangeMode, RISING);
    attachInterrupt(digitalPinToInterrupt(selectPin), ChangeSelection, RISING);
    attachInterrupt(digitalPinToInterrupt(outputPin), ChangeOutput, RISING);
    attachInterrupt(digitalPinToInterrupt(leftArrow), LeftArrow, RISING);
    attachInterrupt(digitalPinToInterrupt(rightArrow), RightArrow, RISING);

    // rotary encoder setup
    //attachInterrupt(digitalPinToInterrupt(rotaryA), RotaryInput, FALLING);
    attachInterrupt(digitalPinToInterrupt(rotaryButton), RotaryButton, RISING); // not tied to anything atm

    displayV2();

}