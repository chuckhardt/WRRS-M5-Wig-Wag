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
#include "Timer.h"
#include "WigWag.h"
#include "SerialLoop.h"
#include "ReadUserInput.h"
#include "MainMagnetControl.h"


Timer MainEventTimer;
int   giMainEventTimerID;
int   giDutyCycleEventTimerID;

SerialPortType  SerialMain;
MainMagnet      MainActionControl;


char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght];

eTaskCount iTaskSwitch = ReadUserInputRightLimit;


// ***************************************************
//
// setup()
//
// This is an Arduino function that is called when the processor
// has restarted.   We are using this function to initialize
// our timers and set various variables.
//
// ****************************************************
void setup()
{
  int iIndex;
    
  // have to initialize the serial port if we want to use if for debugging
  Serial.begin(9600);

  // Init all Arduino IO pins
  InitializeArduinoIOpins();
  
  // We are going to start the main loop event timer.
  // Every time the timer expires, the TaskSwitchMain() function will be called.
  giMainEventTimerID = MainEventTimer.every(25, TaskSwitchMain);

  // Every time the timer expires, the DutyCycleTimer() function will be called.
  giDutyCycleEventTimerID = MainEventTimer.every(1000, DutyCycleTimer);
 

  // upon boot up, we are going to activate the signal
  MainActionControl.ActivateMainMagnet();
  MainActionControl.RightMagnetActivate();
 
}  //endof setup()

// ***************************************************
//
// loop()
//
// This is the main Arduino function that is called once setup()
// has finished.   We are using this loop to kick off our
// timer.  The timers provide us with a pseudo 
// operating system.
//
// ****************************************************
void loop()
{
    // we need to update the timer continiously
    MainEventTimer.update();
    
    
}  //endof loop()



// *****************************************************************************************
//
// TaskSwitchMain()
//
// This is the main loop for the WigWag controller. It gets called every xxx ms.   
// Each time it is called, a different function will be called.  A poor man's
// task switch.    
//
// ****************************************************************************************
void TaskSwitchMain()
{

    static UserInput  RightLimitSwitch(kRightLimitSwitch);
    static UserInput  LeftLimitSwitch(kLeftLimitSwitch);
    static UserInput  UserInputSwitch(kUserInputSwitch);
    static bool       bLEDflashFlag = false;

    switch (iTaskSwitch)
    {
        case ReadUserInputRightLimit:  // read the right limit switch, if actuated, turn on the left magnet
         
            RightLimitSwitch.ReadSwitch(cParseArgList, &LeftMagnetActivate );
            break;
            
        case ReadUserInputLeftLimit:  // read the left limit switch, if actuated, turn on the right magnet 

            LeftLimitSwitch.ReadSwitch (cParseArgList, &RightMagnetActivate ); 
            break;

        case ReadUserInputUserSwitch:  // read the user input (start) switch, if actuated, turn on the main magnet.
              
            UserInputSwitch.ReadSwitch (cParseArgList, &ActivateMainMagnet ); 
            break;
      
        case CheckMainMagnet:         // check to see if we need to turn off the main magnet 

            MainActionControl.Operations(cParseArgList);
            break;
    
        case ReadSerialPort:         // see if a cmd is queued up to read and take action on it

            SerialMain.SerialLoop(cParseArgList);
            LedFlash();
            break;

        default:
            IncrementTask (iTaskSwitch);

    };

    IncrementTask (iTaskSwitch);


}  //endof TaskSwitchMain()
