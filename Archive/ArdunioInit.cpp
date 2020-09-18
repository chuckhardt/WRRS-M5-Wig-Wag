// ***************************************************
//
// WigWag Signal Program
//
// This software was developed to operate on an Arduino Uno
// microprocessor board.  It controls the WRRS Auto WigWag
// Model #5 at the Southeastern Railway Museum.
//
// Author: C. Hardt
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


// ***************************************************
//
// IncrementTask()
//
// Increment our task switch index to the next task
//
// ****************************************************
void IncrementTask(eTaskCount& eTaskNum)
{

    switch (eTaskNum)
    {
        case ReadUserInputRightLimit:

            eTaskNum = ReadUserInputLeftLimit;
            break;
            
        case ReadUserInputLeftLimit:

            
            eTaskNum = ReadUserInputUserSwitch;
            break;

        case ReadUserInputUserSwitch:

            
            eTaskNum = CheckMainMagnet;
            break;
      
        case CheckMainMagnet:

            eTaskNum = ReadSerialPort;
            break;
    
        case ReadSerialPort:

            
            eTaskNum = ReadUserInputRightLimit;
            break;

        default:
            eTaskNum = ReadUserInputRightLimit;
    }         
           
  
} // endof IncrementTask()


// *****************************************************************************************
//
// LeftMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void LeftMagnetActivate(void)
{

    MainActionControl.LeftMagnetActivate();
  
} // endof LeftMagnetActivate()


// *****************************************************************************************
//
// RightMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void RightMagnetActivate(void)
{

    MainActionControl.RightMagnetActivate();
  
} // endof RightMagnetActivate()

// *****************************************************************************************
//
// ActivateMainMagnet()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void ActivateMainMagnet(void)
{

    MainActionControl.ActivateMainMagnet();
  
} // endof ActivateMainMagnet()

// *****************************************************************************************
//
// DutyCycleTimer()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void DutyCycleTimer (void)
{
    MainActionControl.UpdateDutyCycleDownCount();
  
} //endof DutyCycleTimer()

// need to move this out of here, but this just flashes the LED on the main board


// *****************************************************************************************
//
// LedFlash()
//
// We are going to flash the main board LED on and off just to let us know that everthing
// is running.
//
// ****************************************************************************************
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
