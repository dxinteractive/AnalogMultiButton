#include <AnalogMultiButton.h>

/*  An example for DFRobot 1602 LCD Keypad Shield
 *  this shield has 5 keys SELECT, UP, RIGHT, DOWN and LEFT
 *  product page : https://www.dfrobot.com/product-51.html
 *  
 */

// define the pin you want to use
const int BUTTONS_PIN = A0;

// set how many buttons you have connected
const int BUTTONS_TOTAL = 5;

// find out what the value of analogRead is when you press each of your buttons and put them in this array
// you can find this out by putting Serial.println(analogRead(BUTTONS_PIN)); in your loop() and opening the serial monitor to see the values
// make sure they are in order of smallest to largest
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 99, 255, 408, 640};
// you can also define constants for each of your buttons, which makes your code easier to read
// define these in the same order as the numbers in your BUTTONS_VALUES array, so whichever button has the smallest analogRead() number should come first

const int BUTTON_RIGHT  = 0;
const int BUTTON_UP     = 1;
const int BUTTON_DOWN   = 2;
const int BUTTON_LEFT   = 3;
const int BUTTON_SELECT = 4;

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton buttons(BUTTONS_PIN, BUTTONS_TOTAL, BUTTONS_VALUES);

// pass a fourth parameter to set the debounce time in milliseconds
// this defaults to 20 and can be increased if you're working with particularly bouncy buttons

void setup() {
  // begin serial so we can see which buttons are being pressed through the serial monitor
  Serial.begin(9600);
}

void loop() {
  // Serial.println(analogRead(BUTTONS_PIN));
  // update the AnalogMultiButton object every loop
  buttons.update();

  // check if BUTTON_UP is pressed
  if(buttons.onPress(BUTTON_UP)){
     Serial.println("Button Up is pressed");
  }

  // check if BUTTON_DOWN is pressed
  if(buttons.onPress(BUTTON_DOWN)){
    Serial.println("Button Down is pressed");
  }

  // check if BUTTON_RIGHT is pressed
  if(buttons.onPress(BUTTON_RIGHT)){
    Serial.println("Button Right is pressed");
  }

  //check if BUTTON_LEFT is pressed
  if(buttons.onPress(BUTTON_LEFT)){
      Serial.println("Button Left is pressed");
  }

 //check if BUTTON_SELECT is pressed
  if(buttons.onPress(BUTTON_SELECT)){
    Serial.println("Button Select is pressed");
  }
  
  delay(10);
}
