#include "Arduino.h"
#include "ArduinoInit.h"
#include "WigWag.h"

// ***************************************************
//
// InitializeArduinoIOpins()
//
// Initialize all of the new Arduino IO pins prior to
// starting to use them.
//
// ****************************************************
void InitializeArduinoIOpins()
{
     // Setup the Arduino pins for input and output.  
     pinMode(kArduinoIOpin02, INPUT);
     pinMode(kArduinoIOpin03, INPUT);
     pinMode(kArduinoIOpin04, INPUT);
     pinMode(kArduinoIOpin05, OUTPUT);
  
     pinMode(kArduinoIOpin06, OUTPUT);
     pinMode(kArduinoIOpin07, OUTPUT);
     pinMode(kArduinoIOpin08, INPUT);
     pinMode(kArduinoIOpin09, INPUT);
     pinMode(kArduinoIOpin10, INPUT);
     pinMode(kArduinoIOpin11, OUTPUT);
     pinMode(kArduinoIOpin12, OUTPUT);

     // Set the Arduino on board LED as an output, so we can flash it.
     pinMode(kArduinoIOpin13, OUTPUT);

     // Now set the Inital power up state of all the IO to low
     //digitalWrite(kArduinoIOpin02, LOW);
     //digitalWrite(kArduinoIOpin03, LOW);
     //digitalWrite(kArduinoIOpin04, LOW);
     digitalWrite(kArduinoIOpin05, LOW);
     digitalWrite(kArduinoIOpin06, LOW);

     //digitalWrite(kArduinoIOpin07, LOW);
     //digitalWrite(kArduinoIOpin08, LOW);
     //digitalWrite(kArduinoIOpin09, LOW);
     //digitalWrite(kArduinoIOpin10, LOW);
     //digitalWrite(kArduinoIOpin11, LOW);
     //digitalWrite(kArduinoIOpin12, LOW);
  
     digitalWrite(kArduinoIOpin13, LOW);

} //endof InitializeArduinoIOpins()
