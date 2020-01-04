#include <Arduino.h>
#include "waves.h"

void setupOutput() {
    pinMode(DAC0, OUTPUT);
    pinMode(DAC1, OUTPUT);   
}

void generateDC(int output, float voltage, int offset) {

}

void generateSineWave(int output, float amplitude, float freq, float phase) {

    // will require testing to figure out how to combine resolution + delayVar to create the frequency given by the input.
    int resolution = 1; // multiplier - 1 = 120, 2 = 60, 3 = 30, 4 = 15
    int delayVar = freq;
    // need some way of getting the data settings for this sine wave
    for(int i = 0; i < 128; i+=resolution) {
        if(output) {
            analogWrite(DAC1, sine[i] * amplitude);
        } else {
            analogWrite(DAC0,  sine[i] * amplitude);
        }
        delayMicroseconds(delayVar);
    }
}

void generateSquareWave(int output, float amplitude, float freq, float duty) { // can't use pwm cause we need to modify the amplitude, so table is tiny to maximise performance. Will basically act like PWM

}

void generateTriangleWave(int output, float amplitude, float freq, float phase) {

}

void generateSawtoothWave(int output, float amplitude, float freq, float phase) {

}




