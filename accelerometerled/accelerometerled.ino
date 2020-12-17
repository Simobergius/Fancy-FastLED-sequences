
// Accelerometer controlled sequences

#include <FastLED.h>
#include <Wire.h>
#define NUM_LEDS 490
#define DATA_PIN 6
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
int del = 1;

//--- Accelerometer Register Addresses
#define Power_Register 0x2D
#define X_Axis_Register_DATAX0 0x32 // Hexadecima address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecima address for the DATAX1 internal register.
#define Y_Axis_Register_DATAY0 0x34 
#define Y_Axis_Register_DATAY1 0x35
#define Z_Axis_Register_DATAZ0 0x36
#define Z_Axis_Register_DATAZ1 0x37
int ADXAddress = 0x53;  //Device address in which is also included the 8th bit for selecting the mode, read in this case.

int Xa;

void setup() {

    Serial.begin(9600);
    Serial.println("Start");
    Wire.begin(); // Initiate the Wire library 
    Serial.println("Wire");
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    Serial.println("LEDS");
    delay(100);

    Wire.beginTransmission(ADXAddress);
    Wire.write(Power_Register); // Power_CTL Register
    // Enable measurement
    Wire.write(8); // Bit D3 High for measuring enable (0000 1000)
    Wire.endTransmission();
    Serial.println("Acc config");
}

void loop() {
    Xa = readAccelerometerData();
    Serial.print("Xa: ");
    Serial.println(Xa);

    // Scroll leds from first upwards
    for (int i = NUM_LEDS; i > 0; i--) {
        leds[i] = leds[i - 1];
    }

    // Set first led according to accelerometer data
    leds[0] = CRGB(map((long)abs(Xa), 0, 512, 0, 256), 0, 0);
    FastLED.show();
    delay(del);
}

int readAccelerometerData() {
    // read X-axis data
    int X0, X1, X_out;
    Wire.beginTransmission(ADXAddress);

    Wire.write(X_Axis_Register_DATAX0);
    Wire.write(X_Axis_Register_DATAX1);

    Wire.endTransmission();

    Wire.requestFrom(ADXAddress, 2);
    if (Wire.available() <= 2) {
        X0 = Wire.read();
        X1 = Wire.read();
        X1 = X1 << 8;
        X_out = X0 + X1;
    }
    return X_out;
}
