// Reading 9DOF sensor stick from Sparkfun's
// https://www.sparkfun.com/products/10724
// Used to provide orientation of the chassis and pressure
// on leg sensors built around Sparkfun's
// https://www.sparkfun.com/products/9375
 
// Thanks to Sparkfun for the support
 
#include <Wire.h>
#include <FastLED.h>

#define NUM_LEDS 490
#define DATA_PIN 13
#define COLOR_ORDER GRB
#define LED_BATCH 5

#define GYRO_CLAMP 3000

#define ACCEL_G 260
#define ACCEL_CLAMP 50
 
#define GYROADDR 0x68
#define ACCELADDR 0x53

int prevAbsA = 0;

CRGB leds[NUM_LEDS];
int del = 1;

union XYZBuffer {
  struct {
    short x,y,z;
  } value;
  byte buff[6];
};
 
void changeEndian(union XYZBuffer *xyz) {
  for (int i=0;i<6;i+=2) {
    byte t=xyz->buff[i];
    xyz->buff[i]=xyz->buff[i+1];
    xyz->buff[i+1]=t;
  }
}
 
// Generically useful reading into a union type
void readXYZ(int device,union XYZBuffer *xyz) {    
  Wire.requestFrom(device, 6);      
  long start=millis();
  while (!Wire.available() && (millis()-start)<100);
  if (millis()-start<100) {
    for (int i=0;i<6;i++)
      xyz->buff[i]=Wire.read();
  }
}
 
void setupAccel(int device) {
  // Check ID to see if we are communicating
  Wire.beginTransmission(device);
  Wire.write(0x00); // One Reading
  Wire.endTransmission();
  Wire.requestFrom(device,1);
  while (!Wire.available());  
  byte ch=Wire.read();
  //Serial.print("Accel id is 0x");
  //Serial.println(ch,HEX);
  // Should output E5
 
  // https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
  // Page 16
  Wire.beginTransmission(device);
  Wire.write(0x2d);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(device);
  Wire.write(0x38);
  Wire.write(0x00);
  Wire.endTransmission();
 
}
void readAccel(int device,union XYZBuffer *xyz) {
  Wire.beginTransmission(device);
  Wire.write(0x32); // One Reading
  Wire.endTransmission();
  readXYZ(device,xyz);
}
 
void setupGyro(int device) {
  // Check ID to see if we are communicating
  Wire.beginTransmission(device);
  Wire.write(0x00); // One Reading
  Wire.endTransmission();
  Wire.requestFrom(device,1);
  while (!Wire.available());  
  byte ch=Wire.read();
  //Serial.print("Gyro id is 0x");
  //Serial.println(ch,HEX);  
  // Should output 69
}
void readGyro(int device,union XYZBuffer *xyz) {
  // https://www.sparkfun.com/datasheets/Sensors/Gyro/PS-ITG-3200-00-01.4.pdf
  // page 20
  Wire.beginTransmission(device);
  Wire.write(0x1d);
  Wire.endTransmission();
  readXYZ(device,xyz);
  changeEndian(xyz);  
}

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  setupAccel(ACCELADDR);
  setupGyro(GYROADDR);
  leds[0] = CRGB(0xff,0x0,0x0);
  leds[1] = CRGB(0x00,0xff,0x0);
  leds[2] = CRGB(0x00,0x00,0xff);
  FastLED.show();
  delay(100);
}

int limit(int value, int a_min, int a_max)
{
  return max(min(value, a_max), a_min);
}
 
void loop()
{
  union XYZBuffer gyro,accel;
  readAccel(ACCELADDR,&accel);
  readGyro(GYROADDR,&gyro);
  
  int absA = sqrt(pow(accel.value.x, 2) + pow(accel.value.y, 2) + pow(accel.value.z, 2));
  absA -= ACCEL_G + ACCEL_CLAMP;
  if (absA < 0)
  {
    absA = 0;
  }
  
  int absGYZ = sqrt(pow(gyro.value.y, 2) + pow(gyro.value.z, 2));
  absGYZ -= GYRO_CLAMP;
  if (absGYZ < 0)
  {
    absGYZ = 0;
  }
  int absGX = abs(gyro.value.x) - GYRO_CLAMP * 6;
  if (absGX < 0)
  {
    absGX = 0;
  }
  
  // Scroll leds from first upwards
  for (int i = NUM_LEDS; i > 0; i-=LED_BATCH) {
    for ( int j = 0; j < LED_BATCH; j++)
    {
      leds[i-j] = leds[i-j - LED_BATCH];
    }
  }
  
  // Set first leds according to gyro data
  for (int i = 0; i < LED_BATCH; i++)
  {
    leds[i].setHSV(random(140, 200), random(100, 255), limit(map(absGYZ, 0, 0xffff, 0, 32), 0, 255));
    leds[i].red = limit(map(max(absA, prevAbsA), 0, 0xffff, 0, 255), 0, 255);
    leds[i].green += limit(map(absGX, 0, 0xffff, 0, 32), 0, 255);
    //leds[i] = CRGB(map(abs(gyro.value.x), 0, 0xffff, 0, 128), map(abs(gyro.value.y), 0, 0xffff, 0, 128), map(abs(gyro.value.z), 0, 0xffff, 0, 128));
  }
  FastLED.show();
  prevAbsA = absA;
  //delay(del);
}
