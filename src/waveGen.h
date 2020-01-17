void setupOutput();
void generateDC(int output, float voltage, int offset);
void generateSineWave(int output, float amplitude, float freq, float phase);
void generateSquareWave(int output, float amplitude, float freq, float duty);
void generateTriangleWave(int output, float amplitude, float freq, float phase);
void generateSawtoothWave(int output, float amplitude, float freq, float phase);
void applyOffset(int output, int offset);

#define DC_OFFSET -15