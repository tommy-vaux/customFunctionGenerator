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

void ChangeMode();
void ChangeSelection();
void ChangeOutput();
void LeftArrow();
void RightArrow();
void RotaryInput();
void RotaryButton();
