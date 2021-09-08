#include <Adafruit_NeoPixel.h>

//#define BUTTON_PIN   PCINT1 // Digital IO pin to a pull-up resistor, executes on high-low transition
//#define INT_PIN PB1
#define PIXEL_PIN    3  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 12  // Number of NeoPixels

const int maxColorValue = 48; // value between 0-255. Higher is brighter.
const long millisPerHour = 60000; // 3,600,000 milliseconds in an hour.

boolean oldState = HIGH;
long timer = 300000; // timer is in milliseconds. default value is 8 hours.
unsigned long startingMillis;

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  colorWipe(strip.Color(maxColorValue, 0, maxColorValue), 50);

  startingMillis = millis();
  
//  cli();                            // Disable interrupts during setup
//  PCMSK |= (1 << BUTTON_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
//  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
//  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
//  sei();                            //last line of setup - enable interrupts after setup
}

void loop () {
  unsigned long currentMillis = millis();
  long spentTime = (long) currentMillis - (long) startingMillis;
  long remainingTime = timer - spentTime;

  if (remainingTime > 0) {
    setLights(remainingTime);
//    colorWipe(strip.Color(maxColorValue, 0, 0), 50);
  } else {
    greenChase();
//    colorWipe(strip.Color(0, maxColorValue, 0), 50);
  }
}

//void increaseTimer() {
//  timer += (15 * 60 * 1000); // default increment is 15 minutes.
//  
//  if (timer > (PIXEL_COUNT * 3600000)) { // roll over timer to 0 if button is pressed more than the number of LEDs can indicate.
//    startingMillis = millis();
//    timer = 0;
//  }
//}

void setLights(long remainingTime) {
  strip.clear();
  
  // set one red LED for each hour
  int numReds = remainingTime / millisPerHour;
  for (int i=0; i<numReds; i++) {
    strip.setPixelColor(i, maxColorValue, 0, 0);
  }

  // set the dim LED
  double remainder = remainingTime % millisPerHour;
  double brightness = remainder / millisPerHour;
  strip.setPixelColor(numReds, maxColorValue * brightness, 0, 0); //if numReds == PIXEL_COUNT, this will cause an error. This shouldn't happen, though, because increaseTimer rolls over.
//  strip.setPixelColor(numReds, maxColorValue * .33, 0, 0);

  strip.show();
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

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
