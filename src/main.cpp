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
        }

        if(getMode(1) == 0) {
            DCSettings dcData2 = getDCSettings(1);
            generateDC(1, dcData2.data[0], dcData2.data[1]);
        }
        
        
}