// IO board: https://io.adafruit.com/xur5/dashboards/melody
// Melody Xu

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#define PHOTOCELL_PIN A0

// store photocell state
int currentP = 0;
int lastP = -1;

// set up the 'analog' feed
AdafruitIO_Feed *analog = io.feed("analog");

#define BUTTON_PIN 5

// store states for button
bool currentB = false;
bool lastB = false;

// set up the 'digital' feed
AdafruitIO_Feed *digital = io.feed("digital");

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  // io.run(); is required for all sketches.
  io.run();
  
  // reading the button state and updating state
  if(digitalRead(BUTTON_PIN) == LOW)
    currentB = true;
  else
    currentB = false;
    
  // return if the button value hasn't changed
  if(currentB == lastB)
    return;

  // grab the current state of the photocell
  currentP = analogRead(PHOTOCELL_PIN);

   // return if the Photoresistor value hasn't changed
  if(currentP == lastP)
    return;

  // save the current state to the analog feed on adafruit io
  Serial.print("sending data-> ");
  Serial.println(currentP);
  analog->save(currentP);
  
  // store last photoresistor value
  lastP = currentP;

  // save the current state to the 'digital' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(currentB);
  digital->save(currentB);

  // store last button state
  lastB = currentB;
  
  // wait .2 sec
  delay(200);
  
}

