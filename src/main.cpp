#include "inputSystem.h"
#include "waveGen.h"
// ------------------------------------------------------------------------ // SETUP
void setup() {
  
  setupIO();

  Serial.begin(9600);
}

void loop() {
        RotaryInput();
        
        
        if(getMode(0) == 0) { // if we are in DC mode, generate DC
            DCSettings dcData1 = getDCSettings(0);
            generateDC(0, dcData1.data[0], dcData1.data[1]);
            if(dcData1.data[1] == true) {
                applyOffset(0, DC_OFFSET);
            }
        } else {
            DefaultSettings * waveformData1 = getWaveformSettings(0);
            if(getMode(0) == 1) {
                generateSineWave(0, waveformData1[0].data[1], waveformData1[0].data[2], waveformData1[0].data[3]);
                applyOffset(0, waveformData1[0].data[1]);
            }
        }

        if(getMode(1) == 0) { // if we are in DC mode, generate DC
            DCSettings dcData2 = getDCSettings(1);
            generateDC(1, dcData2.data[0], dcData2.data[1]);
            if(dcData2.data[1] == true) {
                applyOffset(1, DC_OFFSET);
            }
        } else {
            DefaultSettings * waveformData2 = getWaveformSettings(1);
            if(getMode(1) == 1) {
                generateSineWave(0, waveformData2[0].data[1], waveformData2[0].data[2], waveformData2[0].data[3]);
                applyOffset(0, waveformData2[0].data[1]);
            }
        }
        
        
}