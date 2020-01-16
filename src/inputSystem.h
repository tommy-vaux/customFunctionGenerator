#include <Arduino.h>
#include <ClickEncoder.h>
#include <LiquidCrystal.h>
#include <string.h>

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
    float data[2] = {1, 1};

} DCSettings;
