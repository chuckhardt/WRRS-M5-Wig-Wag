
#include "Arduino.h"
#include "ArduinoInit.h"
#include "WigWag.h"
#include "ReadUserInput.h"
#include "MainMagnetControl.h"


#define kMatch 0


// *****************************************************************************************
//
// SetSwitchSequence()
//
// Set the current switch debouncing sequence (1st, or 2nd)
//
// ****************************************************************************************
void UserInput::SetSwitchSequence(int iSwitchSeq)
{
    SwitchSeq = (SwitchEventStates) iSwitchSeq;
}

// *****************************************************************************************
//
// GetSwitchSequence()
//
// Get the current switch debouncing sequence (1st, or 2nd)
//
// ****************************************************************************************
int UserInput::GetSwitchSequence(void)
{
    return(SwitchSeq);
}

// *****************************************************************************************
//
// UserInput()
//
// Construtor for the class.  Based on the Arduino IO pin passed, determines what th supported
// serial commands will be.
//
// ****************************************************************************************
UserInput::UserInput(int iSwitchNumber)
{
    // save off the switch number, such that it becomes part of the class
    miSwitchNumber = iSwitchNumber;

    if (miSwitchNumber == kRightLimitSwitch)
    {  
        strcpy(mcFirstSerialCmd, "switch");
        strcpy(mcSecondSerialCmd, "right");
    }
    
    else if (miSwitchNumber == kLeftLimitSwitch)
    {  
        strcpy(mcFirstSerialCmd, "switch");
        strcpy(mcSecondSerialCmd, "left");
    }

    else if (miSwitchNumber == kUserInputSwitch)
    {  
        strcpy(mcFirstSerialCmd, "switch");
        strcpy(mcSecondSerialCmd, "user");
    }
   
  
}  // endof UserInput()


// *****************************************************************************************
//
// ReadSwitch()
//
// Read the specified limit or user switch and gets its current state.
//
// ****************************************************************************************
void UserInput::ReadSwitch(char cParseArgList[kMaxCommandsSupported][25], callback_func_ptr ptr) 
{

    static long lPreviousTime = 0;

    bool bSwitchState = true;

    long lDwellTime = 100;
    long lCurrentTime = 0;

    // select the correct switch
    if((kMatch == strcmp(cParseArgList[0], mcFirstSerialCmd)) && (kMatch == strcmp(cParseArgList[1], mcSecondSerialCmd))) 
    {
        // set the switch state to true and the seq to the first event
        // This will trigger a larger event
        bSwitchState = false;
        SetSwitchSequence(SwitchFirstEvent);
        lPreviousTime = 1;
        Serial.println ("Serial CMD to set switches received");
 
    }  //endof kMatch       

    else 
    {
        // Read the specified input switches
        bSwitchState  = digitalRead(miSwitchNumber);
        //Serial.print("ReadSwtich() Switch Number: ");
        //Serial.println(miSwitchNumber);
    }
 
    // if the switch is triggered (switch is low) then...
    if (bSwitchState == false )
    {
        // is this the first time we have seen the switch tripped?
        // if so, save off the time, and change the seq state 
        if (GetSwitchSequence() == SwitchNoEvent)
        {
            lPreviousTime = millis();
            SetSwitchSequence(SwitchFirstEvent);
            Serial.println("ReadSwitch() - Switch Seq #1");
            
        }

        // This is the second time we have seen the limit switch triggered
        else if (GetSwitchSequence() == SwitchFirstEvent)
        {
              // get the current time  
              lCurrentTime = millis();

              Serial.println("ReadSwitch() - Switch Seq #2");

              // if the time has been exceeded, then we have debounced the limit switch
              if (lCurrentTime > lPreviousTime + lDwellTime)
              {
                  // Now it is time to notifiy the control mechanism that
                  // the left limit switch has been tripped
                  SetSwitchSequence(SwitchSecondEvent);
 
                  Serial.println("ReadSwitch() - Switch Triggered Seq #2");

                  // this is where we make the callback that was set when the API was called.
                  // We will call the right/left magnet activation, or activate the main magnet
                  (ptr)();
             
              } // endof lCurrentTime > lPreviousTime
              else
              {
                  Serial.print("ReadSwitch() - Switch Seq #2 - Time not yet triggered CT: ");
                  Serial.print(lCurrentTime);
                  Serial.print(" TT: ");
                  Serial.println(lPreviousTime);
                
              }
 
        } // endif SwitchFirstEvent
 
     
    }  // endif Switch State
    
    // no switches have been triggered, so make sure all switch states are reset
    else
    {
        if (SwitchSeq == SwitchSecondEvent)
            Serial.println("ReadSwitch() - Switch Reset");    
      
        SetSwitchSequence(SwitchNoEvent);

        lPreviousTime = 0;
      
    }

}  // endof ReadSwitch()
