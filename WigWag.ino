// ***************************************************
//
// WigWag Signal Program
//
// This software was developed to operate on an Arduino Uno
// microprocessor board.  It controls the WRRS Auto WigWag
// Model #5 at the Southeastern Railway Musuem.
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
int giMainEventTimerID;

SerialPortType  SerialMain;
MainMagnet      MainActionControl;


char cParseArgList[kMaxCommandsSupported][25];

eTaskCount iTaskSwitch = ReadUserInputLeftLimit;

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
  // Every time the timer expires, the ThreeColorLightMain() function will be called.
  giMainEventTimerID = MainEventTimer.every(25, TaskSwitchMain);

  //Serial.println("");
  //Serial.println("SRM WRRS Auto Flagman (Wig-Wag) - Ver 1.02");

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
    MainEventTimer.update();
    
}  //endof loop()


// *****************************************************************************************
//
// LeftMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the callback function.
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
// problems with the callback function.
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
// problems with the callback function.
//
// ****************************************************************************************
void ActivateMainMagnet(void)
{

    MainActionControl.ActivateMainMagnet();
  
} // endof ActivateMainMagnet()


// *****************************************************************************************
//
// TaskSwitchMain()
//
// This is the main loop for the Wig-Wag controller. It gets called every xxx ms.   
// Each time it is called, a different function will be called.  A poor man's
// operating system.    
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
        case ReadUserInputRightLimit:

            // read the right limit switch, if actuated, turn on the left magnet (thereby automatically deactivating the right magnet)
            RightLimitSwitch.ReadSwitch(cParseArgList, &LeftMagnetActivate );
            iTaskSwitch = ReadUserInputLeftLimit;
            break;
            
        case ReadUserInputLeftLimit:

            // read the left limit switch, if actuated, turn on the right magnet (thereby automatically deactivating the left magnet)
            LeftLimitSwitch.ReadSwitch (cParseArgList, &RightMagnetActivate ); 
            iTaskSwitch = ReadUserInputUserSwitch;
            break;

        case ReadUserInputUserSwitch:

            // read the user input (start) switch, if actuated, turn on the main magnet.  This has no affect on the right or left magnets
            UserInputSwitch.ReadSwitch (cParseArgList, &ActivateMainMagnet ); 
            iTaskSwitch = CheckMainMagnet;
            break;
      
        case CheckMainMagnet:

            MainActionControl.Operations(cParseArgList);
            iTaskSwitch = ReadSerialPort;
            break;
    
        case ReadSerialPort:

            SerialMain.SerialLoop(cParseArgList);
            iTaskSwitch = ReadUserInputRightLimit;
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
                
            break;

        default:
            iTaskSwitch = ReadUserInputRightLimit;

    };

}  //endof TaskSwitchMain()
