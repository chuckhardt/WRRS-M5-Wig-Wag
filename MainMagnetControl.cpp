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
#include "Timer.h"
#include "WigWag.h"
#include "MainMagnetControl.h"

#define kMatch 0

extern Timer LeftMagnetEventTimer;
extern Timer RightMagnetEventTimer;
extern Timer MainEventTimer;


// ****************************************************************************************
//
// MainMagnet()
//
// Our Constructor for the class
//
// ****************************************************************************************
MainMagnet::MainMagnet(void)
{
    mlWigWagRestartInhibitTimeWindow = 0;
    miLeftDirectionTimerID = -1;
    miRightDirectionTimerID = -1;
    
} // MainMagnet::MainMagnet()

// ****************************************************************************************
//
// UpdateDutyCycleDownCount()
//
// We want to control the activation duty cycle count.  We will only permit reactivation
// once the target time has been reached.
//
// ****************************************************************************************
void MainMagnet::UpdateDutyCycleInActiveWindow(void)
{

    static long lLastDutyCyclePrintOut = 0;
    
    // get the current time, we will use this to see if it is time to shutdown
    mlCurrentTime = millis();

    if (lLastDutyCyclePrintOut == 0)
    {
        lLastDutyCyclePrintOut = mlCurrentTime;
    }
    
    // if the current time is now greater than the Duty Cycle time, then we can allow the 
    // Wigwag to restart
    if ( mlCurrentTime > mlWigWagRestartInhibitTimeWindow)
    {
         
        // we only want to print the termal down count message one time.
        if (mbDownCountReached != true)
        {
            Serial.println (F("UpdateDutyCycleDownCount() - Duty Cycle Cool Down Operation Complete... OK to Restart WigWag!"));
        }
        
        mbDownCountReached = true;
    }
    
    else
    {
        // we will only print out a message every 10 seconds (10000ms)
        if(mlCurrentTime > lLastDutyCyclePrintOut + 10000)
        {
          Serial.print (F("UpdateDutyCycleDownCount() - Current Time: "));
          Serial.print (mlCurrentTime);
          Serial.print (F(" Restart Activation Time: "));
          Serial.println (mlWigWagRestartInhibitTimeWindow);
          lLastDutyCyclePrintOut = mlCurrentTime;
        }
        
    }
    
} // MainMagnet::UpdateDutyCycleDownCount()


// ****************************************************************************************
//
// Operations()
//
// This function updates the operational mode down count.   If the user presses the activation
// button, the count will be incremented.  This function continues to subtract from the down 
// count until it reaches zero.  When this happens, the magnets are switched off. 
//
// ****************************************************************************************
void MainMagnet::Operations(char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght] ) 
{
    // get the current time, we will use this to see if it is time to shutdown
    mlCurrentTime = millis();

    // if the current time > then the expiration time, then we are done, shut off the main power
    // to the magnents
    if (mlCurrentTime > mlExpirationTime)
    {
        DeactivateMainMagnet();
    }

    // we have a serial override of the main relay control
    if(kMatch == strcmp(cParseArgList[0],"main"))
    {
        //Serial.println (F("Magnet Opers() - 1A"));
        // if we have been told so, then activate the main relay
        if(kMatch == strcmp(cParseArgList[1],"activate"))
        {
            Serial.println (F("CMD: direction => activate"));
            ActivateMainMagnet();
        }

        // if we have been told so, then deactivate the main relay
        else if(kMatch == strcmp(cParseArgList[1],"deactivate"))
        {
            Serial.println (F("CMD: direction => deactivate"));
            DeactivateMainMagnet();
        }
        else
            Serial.println (F("CMD: main - ERROR"));

    }  //endof kMatch    
    else if (kMatch == strcmp(cParseArgList[0],"cycle"))
    {

        // if there is a second argument, then it will be the time interval
        // We are going to change our max duty cycle downcount number to the new  value
        mlDutyCycleMaxTime = atoi(cParseArgList[1]);
               
    }
 
}  // MainMagnet::Operations(void)

// ****************************************************************************************
//
// ActivateMainMagnet()
//
// Calling this function will set an expiration time sometime in the future (based on default
// value set in h file)  So this function can act to start the signal (pull in the main relay)
// or it will just reset the timer count to some point in the future.
//
// ****************************************************************************************
void MainMagnet::ActivateMainMagnet(void)
{

    // We will limit the duty cycle of the WigWag to once operation every xxx seconds
    // Will probably start with one operation every 10 mins (or 600 seconds)
    if (mbDownCountReached == true)
    {

        //MainEventTimer.Pause();

        Serial.println (F("ActivateMainMagnet() -- Start Button has been pressed"));

        // activate the main (top) magnet.  This magnet stays on until the max time, timer expires 
        digitalWrite(kMainMagnetControl, true); 
        
        // a little inertia compensation
        // We are just going to engage the left magnet for a second
        // then the right for a second.  This will get the WigWag
        // started swinging.   
        digitalWrite(kLeftMagnetControl, false);
        delay(1000); 
        digitalWrite(kRightMagnetControl, true); 
        delay(1000);

        // set our flag
        mbMainMagnetOn = true;

        // We will start by getting the CPU time (in ms) at the time this function is called.
        mlCurrentTime = millis();
    
        // update the expiration time 
        mlExpirationTime = mlCurrentTime + kMaxOperationalTimeLimt;
 
        // activate the left magnet.  This magnet stays on until we hit the left limit. 
        LeftMagnetActivate();
 
        // once the WigWag has been activated, we will wait prevent a re-activation for x seconds
        // the duty cycle time is x + teh current time.
        mlWigWagRestartInhibitTimeWindow = kMaxWigWagDutyCycleInActiveDuration + millis();

        // We need to handle the situation when the time rolls over back to zero.
        if ((mlWigWagRestartInhibitTimeWindow + millis() ) < mlWigWagRestartInhibitTimeWindow )
            mlWigWagRestartInhibitTimeWindow = kMaxWigWagDutyCycleInActiveDuration;

        mbDownCountReached = false;

        //MainEventTimer.Resume();

    }
    
}  // MainMagnet::ActivateMainMagnet(void)


// ****************************************************************************************
//
// DeactivateMainMagnet()
//
// This function shuts off the main control relay which terminates Wig-Wag operations.
//
// ****************************************************************************************
void MainMagnet::DeactivateMainMagnet(void)
{

    // shut off the main power relay
    digitalWrite(kMainMagnetControl, false); 

    // shut off both the right and left limit LEDs
    digitalWrite(kRightLimitLED,  LOW);
    digitalWrite(kLeftLimitLED,   LOW);

    // only print one time.
    if (mbMainMagnetOn == true)
    {  
        Serial.println (F("DeactivateMainMagnet() -- Powering Down..."));

        // we need to stop the timer we use to catch problems sensing the left or right limit switch
        LeftMagnetEventTimer.stop(miLeftDirectionTimerID);
        RightMagnetEventTimer.stop(miRightDirectionTimerID);
    }
      
    mbMainMagnetOn = false;


} // endof DeactivateMainMagnet()


// *****************************************************************************************
//
// RightMagnetActivate()
//
// Select the Right Magnet, doing so will result in deactivating the left magnet, since
// we use a single DPDT relay to swap the magnet field direction   
//
// ****************************************************************************************
void MainMagnet::RightMagnetActivate(void)
{
    // We are only going to process right/left commands when the main magnet is on
    if (mbMainMagnetOn == true)
    {  
        Serial.println(F("RightMagnetActivate() "));
        
        LeftMagnetEventTimer.stop(miLeftDirectionTimerID);
         
        // We have one relay (a DPDT), that selects the left or right magnet
        // We use the DPDT relay, to switch the direction of current thru the two coils (right and left).
        // So when we deactivate the left magnet, we are actually activating the right magnet.
        digitalWrite(kRightMagnetControl, true);   
    
        mbMagnetDirection = RIGHT;
    
        // we need to set a timeout timer.  If we don't hit the left limit switch in xxx millseconds, 
        // we will automatically, shut off the right magnet, and activate left magnet
        miLeftDirectionTimerID = LeftMagnetEventTimer.after("LMA", 2000, LeftMagnetActivateWrapperTimeout);
 
    }
  
}  // endof RightMagnetActivate()


// *****************************************************************************************
//
// LeftMagnetActivate()
//
// Select the Left Magnet, doing so will result in deactivating the right magnet, since
// we use a single DPDT relay to swap the magnet field direction   
//
// ****************************************************************************************
void MainMagnet::LeftMagnetActivate(void)
{
    // We are only going to process right/left commands when the main magnet is on
    if (mbMainMagnetOn == true)
    { 
    
        Serial.println(F("LeftMagnetActivate() "));

        RightMagnetEventTimer.stop(miRightDirectionTimerID);
        
        // We have one relay (a DPDT), that selects the left or right magnet
        // We use the DPDT relay, to switch the direction of current thru the two coils (right and left).
        // So when we deactivate the left magnet, we are actually activating the right magnet.
        digitalWrite(kLeftMagnetControl, false);   
    
        mbMagnetDirection = LEFT;
    
        // we need to set a timeout timer.  If we don't hit the right limit switch in xxx millseconds, 
        // we will automatically, shut off the left magnet, and activate right magnet
        miRightDirectionTimerID = RightMagnetEventTimer.after("RMA", 2000, RightMagnetActivateWrapperTimeout);
    }
            
}  // endof LeftMagnetActivate()


// *****************************************************************************************
//
// GetDirectionMagnetSelection()
//
// Returns the current direction magnet relay selection (right = true, left = false)
//
// ****************************************************************************************
bool MainMagnet::GetDirectionMagnetSelection(void)
{
    return(mbMagnetDirection);

}  // endiof GetDirectionMagnetSelection()
