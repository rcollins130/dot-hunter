/* Dot Hunter Arduino Code
 *  Name: Dot_Hunter.ino
 *  By: Robert Collins
 *  Date: April 28, 2018
 *  
 *  This program drives the LED strip in the Dot Hunter game.
 *  It continuously polls eight digital input pins to determine
 *  the desired location of the target on the strip. A signal
 *  is then sent to the LED strip to display a target at that
 *  location. The display is modulated at a 50% duty cycle to
 *  enable the anti-cheating frequency feature.
 *  
 *  The program also continuously outputs random binary values
 *  between 1 and 144 to eight output pins to provide random
 *  starting values for the location of the next target.
 */

#include <FastLED.h>

#define NUM_LEDS 144
#define DATA_PIN 11
#define LED_TYPE WS2812B
#define DOT_WIDTH 5
#define COLOR White

#define PIN1 9 //LSB
#define PIN2 8
#define PIN3 7
#define PIN4 6
#define PIN5 5
#define PIN6 4
#define PIN7 3
#define PIN8 2 //MSB

#define OPIN1 12 //LSB
#define OPIN2 13
#define OPIN3 A0
#define OPIN4 A1
#define OPIN5 A2
#define OPIN6 A3
#define OPIN7 A4
#define OPIN8 A5 //MSB

#define NUM_PINS 8

int inputPins[NUM_PINS] = {PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8};
int outputPins[NUM_PINS] = {OPIN1, OPIN2, OPIN3, OPIN4, OPIN5, OPIN6, OPIN7, OPIN8};

CRGBArray<NUM_LEDS> Leds;
int loc;

unsigned long start;
const unsigned long DELAY_TIME = 2;

int state = 1;

bool debug = true;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(Leds, NUM_LEDS);

  for(int i=0; i<NUM_PINS; i++){
    pinMode(inputPins[i], INPUT);
  }

  if(debug)
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  start = micros();

  loc = getLoc();
  giveLoc();
  
  if(debug)
    Serial.println(loc);

  switch(state){
    case 1 :
      Leds(0,NUM_LEDS) = CRGB::Black;
      state = 2;
      break;
    case 2 :
      setDot(loc);
      state = 1;
      break;
    default:
      break;
  }
  
  FastLED.show();
  while(micros() < start + DELAY_TIME){};
}

int getLoc(){
  int loc = 0b00000000;
  for(int i=0; i<NUM_PINS; i++){
    bitWrite(loc, i, digitalRead(inputPins[i]));
  }
  return loc;
}

void giveLoc(){
  int loc = random(1,144);
  for(int i=0; i<NUM_PINS; i++){
    if(loc % 2 == 0)
      digitalWrite(outputPins[i], LOW);
    else
      digitalWrite(outputPins[i], HIGH);
    loc = loc / 2;
  }
}

void setDot(int i){
  Leds(0,NUM_LEDS) = CRGB::Black;
  if( i+DOT_WIDTH/2 >= NUM_LEDS){
    Leds( 0, (i+DOT_WIDTH/2)%NUM_LEDS) = CRGB::COLOR;
    Leds( (i-DOT_WIDTH/2), NUM_LEDS-1) = CRGB::COLOR;
  }
  else if( i-DOT_WIDTH/2 < 0){
    Leds( NUM_LEDS+(i-DOT_WIDTH/2), NUM_LEDS-1) = CRGB::COLOR;
    Leds( 0, i+DOT_WIDTH/2) = CRGB::COLOR;
  }
  else{
    Leds( (i-DOT_WIDTH/2), (i+DOT_WIDTH/2)) = CRGB::COLOR;
  }
}


