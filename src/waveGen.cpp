#include <Arduino.h>
#include "waves.h"
#include "inputSystem.h"

void setupOutput() {
    pinMode(DAC0, OUTPUT);
    pinMode(DAC1, OUTPUT);

    analogWriteResolution(12);   
}

void generateDC(int output, float voltage, int offset) {
    int pin = DAC0;
    float finalValue = 0;
    if(output) {
        pin = DAC1;
    }
    if(offset) {
        finalValue = voltage/MAX_AMPLITUDE_DC * 4096;
    } else {
        finalValue = voltage/MAX_AMPLITUDE * 4096;
    }

    analogWrite(pin, finalValue);
    delayMicroseconds(100);
}

void generateSineWave(int output, float amplitude, float freq, float phase) {

    // will require testing to figure out how to combine resolution + delayVar to create the frequency given by the input.
    int resolution = 1; // multiplier - 1 = 120, 2 = 60, 3 = 30, 4 = 15
    if(freq > MAX_15) {
        resolution = 4;
        freq = MAX_15;
    }
    else if(freq > MAX_30){
        resolution = 4;
    } else if(freq > MAX_60) {
        resolution = 3;
    } else if(freq > MAX_120) {
        resolution = 2;
    }
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

void applyOffset(int output, int offset) { // gonna apply offset with the op-amp probably.

}




