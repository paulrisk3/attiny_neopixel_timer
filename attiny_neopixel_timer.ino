#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   PCINT1 // Digital IO pin to a pull-up resistor, executes on high-low transition
#define PIXEL_PIN    3  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 12  // Number of NeoPixels
#define INT_PIN PB1

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

const int millisPerSecond = 1000;
const int timerIncrement = 15 * 60; // seconds added per button-press
const int defaultHours = 8; // number of hours before any button presses
const double maxColorValue = 60; // value between 0-255. Higher is brighter.

int timer = defaultHours * 60 * 60; // timer is in seconds
boolean oldState = HIGH;
unsigned long previousMillis = 0;

void setup() {
//  pinMode(BUTTON_PIN, INPUT_PULLUP);

  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << BUTTON_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            //last line of setup - enable interrupts after setup
  
  strip.begin();
  colorWipe(strip.Color(maxColorValue, 0, maxColorValue), 50);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= millisPerSecond) {
    previousMillis = currentMillis;

   if (timer < 1) {
      greenChase();
    } else {
      setLights();
//      delay(millisPerSecond); // wait one second
      timer--; // decrement timer
    } 
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

void setLights() {
  strip.clear();
  
  // set one red LED for each hour
  int numReds = timer / 3600;
  for (int i=0; i<numReds; i++) {
    strip.setPixelColor(i, maxColorValue, 0, 0);
  }

  // set the dim LED
  double remainder = timer % 3600;
  double brightnessRatio = remainder / 3600;
  strip.setPixelColor(numReds, maxColorValue * brightnessRatio, 0, 0); //if numReds == PIXEL_COUNT, this will cause an error. This shouldn't happen, though, because increaseTimer rolls over.

  strip.show();
}

void increaseTimer() {
  timer += timerIncrement;
  
  if (timer > (PIXEL_COUNT * 3600)) {
    timer = 0; // roll over timer to 0 LEDs if button is pressed too many times
  }
  
//  setLights();
}

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
