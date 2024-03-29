#include <FastLED.h>
#include "HW.h"

CRGB leds[NUM_LEDS];
CHSV ledshsv[NUM_LEDS];
int del = 20;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

// Random independent ripple
// use CHSV leds
uint8_t additionConstrain( uint8_t in, int8_t op )
{
  uint16_t temp = in + 255;
  temp += op;
  temp = constrain( temp, 255, 255 + 255);
  return temp - 255;
}

void loop() {
  srandom( millis() + analogRead(A1) + analogRead(A2));
  for ( int i = 0; i < NUM_LEDS; i++)
  {
    ledshsv[i].h += (random() % 4) - 1;
    //ledshsv[i].s = additionConstrain( ledshsv[i].s, (random() % 6) - 2 );
    //ledshsv[i].v = additionConstrain( ledshsv[i].v, (random() % 3) - 1 );
    ledshsv[i].s = 255;
    ledshsv[i].v = 255;
    hsv2rgb_rainbow(ledshsv[i], leds[i]);
    //leds[i].setHSV(ledshsv[i]);
  }
  FastLED.show();
}

// According to potentiometer position ( all until pot pos)
/*
static const uint16_t ADC_BUF_SIZE = 10;
static uint16_t adcBuffer[ADC_BUF_SIZE];
static uint8_t bufPos = 0;
uint16_t average = 0;
uint16_t mapped = 0;
uint16_t previous = NUM_LEDS + 1;
uint16_t adcMax = 0;
void loop()
{
  uint16_t adcVal = analogRead(A1);
  adcBuffer[bufPos++] = adcVal;
  // moving average
  if ( bufPos == ADC_BUF_SIZE )
  {
    bufPos = 0;
    average = 0;
    for ( int i = 0; i < ADC_BUF_SIZE; i++)
    {
      average += adcBuffer[i];
    }
    average /= ADC_BUF_SIZE;
    if ( average > adcMax )
    {
      adcMax = average;
    }
    mapped = map(average, 0, adcMax - 20, 0, NUM_LEDS);
    mapped = constrain(mapped, 0, NUM_LEDS);
    for ( int i = 0; i < NUM_LEDS; i++ )
    {
      if ( i < mapped )
      {
        leds[i] = CRGB::White;
      }
      else
      {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}
*/
/*
// According to potentiometer position (only pot pos, fading transition)
static const uint16_t ADC_BUF_SIZE = 10;
static uint16_t adcBuffer[ADC_BUF_SIZE];
static uint8_t bufPos = 0;
uint16_t average = 0;
uint16_t mapped = 0;
uint16_t previous = NUM_LEDS + 1;
void loop()
{
  uint16_t adcVal = analogRead(A1);
  adcBuffer[bufPos++] = adcVal;
  // moving average
  if ( bufPos == ADC_BUF_SIZE )
  {
    bufPos = 0;
    average = 0;
    for ( int i = 0; i < ADC_BUF_SIZE; i++)
    {
      average += adcBuffer[i];
    }
    average /= ADC_BUF_SIZE;
    // fade others to black
    for (int j = NUM_LEDS; j >= 0; j--) {
      // Decrease brightness
      leds[j].fadeToBlackBy(20);
    }
    mapped = map(average, 0, 1000, 0, NUM_LEDS-1);
    mapped = constrain(mapped, 0, NUM_LEDS-1);
    while ( previous < mapped )
    {
      previous++;
      leds[previous] = CRGB::White;
    }
    while ( previous > mapped )
    {
      previous--;
      leds[previous] = CRGB::White;
    }
    leds[mapped] = CRGB::White;
    FastLED.show();
  }
}
*/
/*
// Random blinks initiate droplet
void loop() {
  srandom(millis());

  for (int i = NUM_LEDS; i > 0; i--) {

    if (leds[i-1] != CRGB(0,0,0)) {
      // For waterfall effect
      leds[i]= leds[i-1];
    }

    // Decrease brightness
    leds[i].fadeToBlackBy(50);

    // Random change for unlit led to be lit
    if (leds[i] == CRGB(0,0,0)) {
      if (random() % 1000 < 1) {
        leds[i].setHSV(random() % 255, 255, 255);
      }
    }

  }
  FastLED.show();
  delay(del);
}
*/

// Random "waterfalling" dots
/*
void loop() {
  srandom(millis());

  for (int i = NUM_LEDS-1; i >= 0; i--) {
    // Decrease brightness

    if (leds[i] != CRGB(0,0,0)) {
      // For waterfall effect
      leds[ (i+1) % NUM_LEDS]= leds[i];
      leds[i].fadeToBlackBy(30);
    }
    leds[ (i+1) % NUM_LEDS].fadeToBlackBy(5);

    // Random change for unlit led to be lit
    if (leds[i] == CRGB(0,0,0)) {
      if (random() % 1600 < 1) {
        leds[i].setHSV(random() % 255, 255, random() % 75 + 180);
      }
    }

  }
  FastLED.show();
  delay(del);
}
*/
// Random blinks that fade
/*
void loop() {
  // Seed rngenerator using analog value which contains noise
  srandom(millis() + analogRead(A2));

  for (int i = NUM_LEDS; i > 0; i--) {
    // Decrease brightness
    leds[i].fadeToBlackBy(50);

    // Random change for unlit led to be lit
    if (leds[i] == CRGB(0,0,0)) {
      if (random() % 800 < 1) {
        //leds[i].setHSV(random() % 255, 255, 255);
        leds[i] = CRGB::White;
      }
    }

  }
  FastLED.show();
  delay(del);
}
*/
// Random blinks that fade in and out
/*
bool fadingIn[NUM_LEDS] = { 0 };
void loop() {
  // Seed rngenerator using analog value which contains noise
  srandom(millis() + analogRead(A2));

  for (int i = NUM_LEDS; i > 0; i--) {
    if ( fadingIn[i] )
    {
      // Increase brightness
      leds[i].addToRGB(30);
    }
    else
    {
      // Decrease brightness
      leds[i].subtractFromRGB(30);
    }

    // Random chance for completely lit led to start fading
    if ( leds[i] == CRGB( 255, 255, 255 ) )
    {
      if (random() % 25 < 1) {
        // Add to list of fadingIn
        fadingIn[i] = false;
      }
    }

    // Random chance for unlit led to be lit
    if (leds[i] == CRGB(0, 0, 0)) {
      if (random() % 200 < 1) {
        // Add to list of fadingIn
        fadingIn[i] = true;
      }
    }
  }
  FastLED.show();
  delay(del);
}
*/
// Fading rolling colors (Teardrops)
/*
uint8_t seq = 1;
uint8_t curHue = 0;
uint8_t curVal = 255;
uint8_t curSat = 255;

void loop() {
    srandom(millis());

    // Scroll leds from first upwards
    for (int i = NUM_LEDS; i > 0; i--) {
        leds[i] = leds[i - 1];
    }

    // Set first leds color, if it has faded to black, else fade
    if (leds[0] == CRGB(0, 0, 0)) {

        leds[0].setHSV(cubicwave8(curHue), curSat, curVal);
        curHue += random() % 20 + 0;
    }
    else {
        leds[0].fadeToBlackBy(random() % 100 + 20);
    }
    FastLED.show();
    delay(del);
}
*/
// Do nothing
/*
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
*/

// Set one to const color
/*
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
      if (i == 15) {
        leds[i] = CRGB::White;
      }
      else{
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
}
*/

// Set all to const color
/*
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
    }
    FastLED.show();
}
*/

// Smooth transition between const colors
/*
#define NUM_COLORS 5
#define CHANGE_TIME 50
static const CRGB colors[NUM_COLORS] = { CRGB::Red, CRGB::Purple, CRGB::Blue,
CRGB::Green, CRGB::Yellow}; CRGB prevColor; CRGB currColor;

void loop() {
    for(int i = 0; i < NUM_COLORS; i++) {
        for (int d = 0; d <= CHANGE_TIME; d++) {
            currColor.red = map(d, 0, CHANGE_TIME, prevColor.red,
colors[i].red); currColor.blue = map(d, 0, CHANGE_TIME, prevColor.blue,
colors[i].blue); currColor.green = map(d, 0, CHANGE_TIME, prevColor.green,
colors[i].green); for(int j = 0; j < NUM_LEDS; j++) { leds[j] = currColor;
            }
            FastLED.show();
            delay(del);
        }
        delay(del * 500);
        prevColor = colors[i];
    }
}
*/

// Flash const colors
/*
#define NUM_COLORS 3
static const CRGB colors[NUM_COLORS] = { CRGB::Red, CRGB::Blue, CRGB::Green };
void loop() {
    for(int i = 0; i < NUM_COLORS; i++) {
        for(int j = 0; j < NUM_LEDS; j++) {
            leds[j] = colors[i];
        }
        FastLED.show();
        delay(del);
    }
}
*/

// Send pulses
/*
#define PULSE_LENGTH 10
#define COLOR_CHANGE 40
#define PULSE_INTERVAL 15
int currentColor = 0;
int it = 0;
void loop() {
    for(int i = 0; NUM_LEDS > i; i++) {
        leds[i] = leds[i+1];
    }
    if(it < PULSE_LENGTH) {
        leds[NUM_LEDS].setHSV(currentColor, 255, 255);
    } else if (it < PULSE_INTERVAL) {
        leds[NUM_LEDS].setHSV(currentColor, 255, 0);
    } else {
        it = -1;
        currentColor += COLOR_CHANGE;
    }
    FastLED.show();
    it++;
    delay(del);
}
*/
// Standing triangular wave // NOT WORKING
/*
#define WAVELENGTH 50
#define COLOR_RANGE_LOW 0
#define COLOR_RANGE_HIGH 255
double wave = 0;
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].setHSV(i+wave , 255, 50);
    }
    wave++;
    FastLED.show();
    delay(del);
}
*/

// Roll and add color according to triangular-ish function
/*
#define AMOUNT_UP 50
#define AMOUNT_DOWN 50
#define NEGATIVE_SLOPE 2
#define POSITIVE_SLOPE 2
int currentColor = 0;
int it = 0;
void loop() {
    for(int i = 0; i < NUM_LEDS; i+=1) {
        for(int j = 0; j < 1; j++) {
            leds[i+j] = leds[i+j+1];
        }

        for(int j = NUM_LEDS; j > NUM_LEDS-1; j--) {
            leds[j].setHSV(currentColor, 255, 50);
        }
    }
    if(it<100) {
        currentColor += POSITIVE_SLOPE;
    }else{
        currentColor -= NEGATIVE_SLOPE;
    }
    currentColor %= 255;
    if(it >= 150)
    it = 0;
    it++;
    FastLED.show();
    delay(del);
}
*/
/*
// set everything to between a color range

int low = 32, high = 96;
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].setHSV(random(low, high), 255, 50);
    }
    FastLED.show();
    delay(del);
}
*/

// Change color every X seconds
/*
int color = 0;
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].setHSV(color, 255, 255);
    }
    color += random(15, 240);
    FastLED.show();
    delay(del);

}
*/

// Rolling segments Nearby colors
/*
#define SEGMENT_LENGTH 1
int currentColor = 0;
void loop() {
    for(int i = 0; i < NUM_LEDS; i+=SEGMENT_LENGTH) {
        for(int j = 0; j < SEGMENT_LENGTH; j++) {
            leds[i+j] = leds[i+j+SEGMENT_LENGTH];
        }

        for(int j = NUM_LEDS; j > NUM_LEDS-SEGMENT_LENGTH; j--) {
            leds[j].setHSV(currentColor, 255, random(100, 255));
        }
    }
    currentColor += random(0, 20) - 10;
    FastLED.show();
    delay(del);
}
/*

// Rolling segments
/*
#define SEGMENT_LENGTH 1
void loop() {
    for(int i = 0; i < NUM_LEDS; i+=SEGMENT_LENGTH) {
        for(int j = 0; j < SEGMENT_LENGTH; j++) {
            leds[i+j] = leds[i+j+SEGMENT_LENGTH];
        }

        for(int j = NUM_LEDS; j > NUM_LEDS-SEGMENT_LENGTH; j--) {
            leds[j].setHSV(random(255), 255, random(100, 255));
        }
    }
    FastLED.show();
    delay(del);
}
*/

// Repeating segments
/*
#define SEGMENT_LENGTH 5
void loop() {
    for(int i = 0; i < NUM_LEDS; i+=SEGMENT_LENGTH) {
        for(int j = 0; j < SEGMENT_LENGTH; j++) {
            leds[i+j] = leds[i+j-SEGMENT_LENGTH];
            if(i+j < SEGMENT_LENGTH) {
                leds[i+j].setHSV(random(255), 255, random(100, 255));
            }
        }
    }
    FastLED.show();
    delay(del);
}
*/

/*
// Kaikki Random
void loop() {
    for(int i = 0; i < NUM_LEDS; i+=6) {
        leds[i].setHSV(random(255), 255, random(100, 255));
        leds[i+1].setHSV(random(255), 255, random(100, 255));
        leds[i+2].setHSV(random(255), 255, random(100, 255));
    }
    FastLED.show();
    delay(500);
    for(int i = 3; i < NUM_LEDS; i+=6) {
        leds[i].setHSV(random(255), 255, random(100, 255));
        leds[i+1].setHSV(random(255), 255, random(100, 255));
        leds[i+2].setHSV(random(255), 255, random(100, 255));
    }
    FastLED.show();
    delay(del);
}
*/

// Rullaava constant colors
/*
void loop() {
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(del);
    }
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Green;
        FastLED.show();
        delay(del);
    }
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(del);
    }
}
*/
