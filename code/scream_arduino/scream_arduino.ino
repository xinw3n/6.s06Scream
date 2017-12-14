#include <FastLED.h> 
// How many leds are in the strip?
#define NUM_LEDS 26
// Data pin that led data will be written out over
#define DATA_PIN 11
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 13
// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// Read microphone value from this pin 
#define MIC_PIN A0; 
// update every 200 ms 
const int sampleWindow = 400; 
unsigned int sample;
int maxScore; 

String inString = "";    // string to hold input
int user = 0; 

uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;

 unsigned long startMillis; 


void setup() {
  // set up LED strip
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  maxScore = 0;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("connected");
  startMillis= millis();  // Start of sample window
}

void loop() {
  
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int screamMax = 0;
  unsigned int signalMin = 12;
  if (Serial.available() > 0) {
      int inChar = Serial.read();
      if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
      }
      if (inChar == '\n') {
      Serial.print("Value:");
      user = inString.toInt(); 
      Serial.println(user);
      
      // clear the string for new input:
      inString = "";
      }
      //sample = analogRead(MIC_PIN);
      
      if (user > signalMin){// discard ambient noise
        //maxScore = _max(maxScore, int(analogRead(A0))); 
        Serial.println("display");
        maxScore = max(maxScore, user);
        Serial.print(maxScore); 
        if (maxScore > 300){
          thishue++;        
          fill_rainbow(leds, NUM_LEDS, thishue, deltahue);  
          FastLED.delay(200);
          FastLED.clear(); 
        }
        else{
          Serial.print("regular"); 
          
          int lit = min(NUM_LEDS, int(maxScore/10)); 
          Serial.print(lit); 
          for(int i = 0; i < lit; i++) {
            leds[i] = CRGB::Red;    // turn on white 
            FastLED.show();
            delay(50);
          }
          FastLED.clear(); 
        }
      
      }
   }
}
