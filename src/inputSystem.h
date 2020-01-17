#include <Arduino.h>
#include <ClickEncoder.h>
#include <LiquidCrystal.h>
#include <string.h>
// need these here so they can be pulled by wavegen.
#define MAX_AMPLITUDE 15
#define MAX_AMPLITUDE_DC 30

#define MAX_OFFSET 5
#define MAX_PHASE 359
#define MIN_PHASE -359
#define MAX_DUTY 100

#define MIN_MULTIPLIER -2
#define MAX_MULTIPLIER_SMALL 1 // for smaller value inputs, like voltage, offset, magnitude
#define MAX_MULTIPLIER_PHASE 3 // for phase only
#define MAX_MULTIPLER_FREQUNECY_NORMAL 4 // only frequency reaches values above 1000. Square output should be able to go much higher than this tho is PWM is used.

// the maximum frequencies that can be achieved by the function generator at given res due to CPU limits (in Hz).
#define MAX_120 1000
#define MAX_60 2000
#define MAX_30 5000
#define MAX_15 10000 // this is basically our maximum frequency

// FUNCTIONS
void setupIO();

// other functions
void cleanLCD(int row);
void DisplayOutput();
void DisplayMode();
void DisplaySelection();
void DisplayData();
void setupInputCursor();
void updateMultiplier();
void checkWaveformData();

void ChangeMode();
void ChangeSelection();
void ChangeOutput();
void LeftArrow();
void RightArrow();
void RotaryInput();
void RotaryButton();

// for storing user data
typedef struct ArbitarySettings {
    float data[4] = {0, 1, 1000, 0};

} DefaultSettings;

typedef struct _DCSettings {
    float data[2] = {1, 0};

} DCSettings;

// getters for other areas
int getOutput();
DefaultSettings * getWaveformSettings(int output);
DCSettings getDCSettings(int output);
int getMode(int output);