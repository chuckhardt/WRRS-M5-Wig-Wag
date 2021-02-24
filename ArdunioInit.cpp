// ***************************************************
//
// WigWag Signal Program
//
// This software was developed to operate on an Arduino Uno
// microprocessor board.  It controls the WRRS Auto WigWag
// Model #5 at the Southeastern Railway Museum.
//
// Author: C. Hardt
// Target Platform: Arduino Uno
// Date: 04/19/20
//
// ****************************************************

#include "Arduino.h"
#include "ArduinoInit.h"
#include "WigWag.h"

extern MainMagnet      MainActionControl;

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
     pinMode(kArduinoIOpin02, INPUT);    // Right Limit
     pinMode(kArduinoIOpin03, INPUT);    // Left Limit
     pinMode(kArduinoIOpin04, INPUT);    // User Switch
     pinMode(kArduinoIOpin05, OUTPUT);   // Right/Left Magnet
  
     pinMode(kArduinoIOpin06, OUTPUT);   // Main Magnet
     pinMode(kArduinoIOpin07, OUTPUT);
     pinMode(kArduinoIOpin08, OUTPUT);   // Left Limit LED
     pinMode(kArduinoIOpin09, OUTPUT);   // Right Limit LED
     pinMode(kArduinoIOpin10, INPUT);
     pinMode(kArduinoIOpin11, OUTPUT);
     pinMode(kArduinoIOpin12, OUTPUT);

     // Set the Arduino on board LED as an output, so we can flash it.
     pinMode(kArduinoIOpin13, OUTPUT);

     // Now set the Inital power up state of all the IO to low
     digitalWrite(kArduinoIOpin05, LOW);
     digitalWrite(kArduinoIOpin06, LOW);
     digitalWrite(kRightLimitLED,  LOW);
     digitalWrite(kLeftLimitLED,   LOW);
 
     digitalWrite(kArduinoIOpin13, LOW);

} //endof InitializeArduinoIOpins()

// *****************************************************************************************
//
// LeftMagnetActivateWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void LeftMagnetActivateWrapper(void)
{

    MainActionControl.LeftMagnetActivate();
  
} // endof LeftMagnetActivateWrapper()

// *****************************************************************************************
//
// LeftMagnetActivateWrapperTimeout()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void LeftMagnetActivateWrapperTimeout(void)
{
    Serial.print (F("Timeout waiting for the right Limt... "));
    MainActionControl.LeftMagnetActivate();
  
} // endof LeftMagnetActivateWrapperTimeout()

// *****************************************************************************************
//
// RightMagnetActivateWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void RightMagnetActivateWrapper(void)
{

    MainActionControl.RightMagnetActivate();
  
} // endof RightMagnetActivateWrapper()

// *****************************************************************************************
//
// RightMagnetActivateWrapperTimeout()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void RightMagnetActivateWrapperTimeout(void)
{
    Serial.print (F("Timeout waiting for left Limt... "));
    MainActionControl.RightMagnetActivate();
  
} // endof RightMagnetActivateWrapperTimeout()

// *****************************************************************************************
//
// ActivateMainMagnetWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void ActivateMainMagnetWrapper(void)
{
    MainActionControl.ActivateMainMagnet();
  
} // endof ActivateMainMagnetWrapper()

// *****************************************************************************************
//
// DutyCycleTimer()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void DutyCycleTimer (void)
{
    MainActionControl.UpdateDutyCycleInActiveWindow();
  
} //endof DutyCycleTimer()

// *****************************************************************************************
//
// LedFlash()
//
// We are going to flash the main board LED on and off just to let us know that everthing
// is running.
//
// *****************************************************************************************
void LedFlash (void)
{
    static bool bLEDflashFlag = false;

    // Alternate flashing the main board LED
    if (bLEDflashFlag == false)
    {
        digitalWrite(kStatusLED, HIGH);
        bLEDflashFlag = true;
    }
    else
    {
        digitalWrite(kStatusLED, LOW);
        bLEDflashFlag = false;
    }
    
}  // endof LedFlash()
