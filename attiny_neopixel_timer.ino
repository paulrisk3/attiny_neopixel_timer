#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2 // Digital IO pin to a pull-up resistor, executes on high-low transition
#define PIXEL_PIN    3  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 12  // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

const int millisPerMinute = 1000; // 60,000
const int timerIncrement = 15; // minutes added per button-press
const int defaultHours = 8; // number of hours before any button presses
const double maxColorValue = 60; // value between 0-255. Higher is brighter.
const double colorPerMinute = maxColorValue / 60; // ratio of how much brightness to lose per minute

boolean oldState = HIGH;
int timer = defaultHours * 60; // timer is in minutes

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  colorWipe(strip.Color(maxColorValue, 0, maxColorValue), 50);
}

void loop() {
  if (timer < 1) {
    greenChase();
  } else {
    setLights(timer);
    delay(millisPerMinute); // wait one minute
    timer--; // decrement timer
  }
  
//  boolean newState = digitalRead(BUTTON_PIN); // Get current button state.
//  
//  if((newState == LOW) && (oldState == HIGH)) {
//    delay(20); //button debounce delay
//    newState = digitalRead(BUTTON_PIN); // Check if button is low
//    if(newState == LOW) {      // Yes, still low
//      increaseTimer();
//    }
//  }
//  
//  oldState = newState;
}

void setLights(int timeLeft) {
  strip.clear();
  
  // set one red LED for each hour
  int numReds = timeLeft / 60;
  for (int i=0; i<numReds; i++) {
    strip.setPixelColor(i, maxColorValue, 0, 0);
  }

  // set the dim LED
  double remainder = timeLeft % 60;
  double brightnessRatio = remainder / 60;
  strip.setPixelColor(numReds, maxColorValue * brightnessRatio, 0, 0); //if numReds == PIXEL_COUNT, this will cause an error. This shouldn't happen, though, because increaseTimer rolls over.

  strip.show();
}

//void increaseTimer() {
//  timer += timerIncrement;
//  
//  if (timer > (PIXEL_COUNT * 60)) {
//    timer = 0; // roll over timer to 0 LEDs if button is pressed too many times
//  }
//  
//  setLights();
//}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void greenChase() { // cycle green around the ring.  
  for(int i=0; i<strip.numPixels(); i++) {
    strip.clear();
    
    strip.setPixelColor(i, 0, maxColorValue * .1, 0);
    strip.setPixelColor((i+1)%PIXEL_COUNT, 0, maxColorValue * .33, 0);
    strip.setPixelColor((i+2)%PIXEL_COUNT, 0, maxColorValue * .66, 0);
    strip.setPixelColor((i+3)%PIXEL_COUNT, 0, maxColorValue, 0);
    
    strip.show();
    delay(150);
  }
}
