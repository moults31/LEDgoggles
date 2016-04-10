// Low power NeoPixel goggles code based on Adafruit's
// Kaleidoscope Eyes project and modified by Zac Moulton
// to take pushbutton input and add 3 new modes.
 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif
 
#define PIN 0
#define BUTTONPIN 3
#define RED 0xFF0000

enum Modes{
  SPARKS,
  WHEELS,
  OFF,
  WHITE,
  RAINBOW,
  NUMMODES
};
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);
 
uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = RED, //Start red
         offset_wide = RED; // For Rainbows
uint32_t prevTime;
 
void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
  prevTime = millis();
  pinMode(BUTTONPIN,INPUT);
}
 
void loop() {
  uint8_t  i;
  uint32_t t;
 
  switch(mode) {
 
   case SPARKS: // Random sparks - just one LED on at a time!
    i = random(32);
    pixels.setPixelColor(i, color);
    pixels.setBrightness(85);
    pixels.show();
    delay(10);
    pixels.setPixelColor(i, 0);
    break;
 
   case WHEELS: // Spinny wheels (8 LEDs on at a time)
    for(i=0; i<16; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
      pixels.setPixelColor(   i, c); // First eye
      pixels.setPixelColor(31-i, c); // Second eye (flipped)
    }
    pixels.setBrightness(85);
    pixels.show();
    offset++;
    delay(50);
    break;

    case OFF: //off
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    pixels.show();
    break;

    case WHITE: //all white
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0xFFFFFF);
    pixels.setBrightness(85);
    pixels.show();
    break;

    case RAINBOW: //rainbow
    for(i=0; i<16; i++){
      pixels.setPixelColor(i, offset_wide<<i);
      pixels.setPixelColor(31-i, offset_wide<<i);
    }
    pixels.setBrightness(85);
    pixels.show();
    if( offset_wide % 2 ) 
      offset_wide |= (0x800000);
    offset_wide >>= 1;
    delay(50);
    break;
  }
 
  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    color >>= 8;                 // Next color R->G->B
    if(!color) color = 0xFF0000; // Reset to red
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    prevTime = t;
  }

  if(digitalRead(BUTTONPIN) == LOW){
    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
    pixels.show();
    if(mode < NUMMODES){
      mode = 0;
    }
    else{
      mode++;
    }
    while(digitalRead(BUTTONPIN) == LOW);
  }
}
