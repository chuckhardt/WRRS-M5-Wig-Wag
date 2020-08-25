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
#include "MainMagnetControl.h"

#define kMatch 0


// ****************************************************************************************
//
// MainMagnet()
//
// Our Constructor for the class
//
// ****************************************************************************************
MainMagnet::MainMagnet(void)
{
    mlDutyCycleTime = 0;
    mbDownCountReached = false;
    mlDutyCycleMaxTime = kMaxWigWagDutyCycle;
    
} // MainMagnet::MainMagnet()

// ****************************************************************************************
//
// UpdateDutyCycleDownCount()
//
// When called our duty cycle down count is decremented.  When we hit 0, the WigWag
// can be activated again.
//
// ****************************************************************************************
void MainMagnet::UpdateDutyCycleDownCount(void)
{

    // as long as the count is positive, then process 
    if (mlDutyCycleTime > 0)
    {
        if (mlDutyCycleTime > mlDutyCycleMaxTime)
        {
            mlDutyCycleTime = mlDutyCycleMaxTime;
        }
        
        mlDutyCycleTime--;
        mbDownCountReached = false;
        if (mlDutyCycleTime == 0)
        {
            mbDownCountReached = true;
        }    

        // only print every so often
        if ((mlDutyCycleTime % 10) == 0)
        {
            Serial.print   ("UpdateDutyCycleDownCount() - Seconds Remaining: ");
            Serial.println (mlDutyCycleTime);
        }
    }
    else
    {
        // we only want to print the down count terminated message one time
        if (mbDownCountReached == true)
        {
            Serial.println("UpdateDutyCycleDownCount() - Reactivtion Down Count Reached");
            mbDownCountReached = false;
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

#if 0
    Serial.print   ("Operations() -- CT: ");
    Serial.print   (mlCurrentTime);
    Serial.print   (" ET: ");
    Serial.print   (mlExpirationTime);
    Serial.print   (" MR State: ");
    Serial.print   (mbMainMagnetOn);
    Serial.print   (" DR State: ");
    Serial.println (GetDirectionMagnetSelection());
#endif

    // if the current time > then the expiration time, then we are done, shut off the main power
    // to the magnents
    if (mlCurrentTime > mlExpirationTime)
    {
        DeactivateMainMagnet();
    }

    // we have a serial override of the main relay control
    if(kMatch == strcmp(cParseArgList[0],"main"))
    {
        // if we have been told so, then activate the main relay
        if(kMatch == strcmp(cParseArgList[1],"activate"))
        {
            //Serial.println ("CMD: direction => activate");
            ActivateMainMagnet();
        }

        // if we have been told so, then deactivate the main relay
        else if(kMatch == strcmp(cParseArgList[1],"deactivate"))
        {
            //Serial.println ("CMD: direction => deactivate");
            DeactivateMainMagnet();
        }
        else
          Serial.println ("CMD: main - ERROR");

    }  //endof kMatch    
    else if (kMatch == strcmp(cParseArgList[0],"cycle"))
    {

        // if there is a second argument, then it will be the time interval
        // We are going to change our max duty cycle downcount number to the new  value
        mlDutyCycleMaxTime = atoi(cParseArgList[1]);
        //Serial.print ("CMD: cycle => ");
        //Serial.println (mlDutyCycleMaxTime);
               
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

    Serial.println ("ActivateMainMagnet() -- Start Button has been pressed");

    // We will limit the duty cycle of the WigWag to once operation every xxx seconds
    // Will probably start with one operation every 10 mins (or 600 seconds)
    if (mlDutyCycleTime <= 0)
    {

        // We will start by getting the CPU time (in ms) at the time this function is called.
        mlCurrentTime = millis();
    
        // update the expiration time 
        mlExpirationTime = mlCurrentTime + kMaxOperationalTimeLimt;
    
        // activate the main (top) magnet.  This magnet stays on until the max time, timer expires 
        digitalWrite(kMainMagnetControl, true); 
        
        // activate the right magnet.  This magnet stays on until we hit the right limit. 
        digitalWrite(kRightMagnetControl, true); 
    
        // set our flag
        mbMainMagnetOn = true;

        // once the WigWag has been activated, we will wait xxx seconds before we activate again
        mlDutyCycleTime = mlDutyCycleMaxTime;

    }
    else
    {
        Serial.println ("Duty Cycle Down Count in progress");
      
    }

#if 0
    Serial.print   ("ActivateSignal() -- Current Time: ");
    Serial.print   (mlCurrentTime);
    Serial.print   (" Neww Expiration Time: ");
    Serial.println (mlExpirationTime);
#endif    
    
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

    // only print one time.
    if (mbMainMagnetOn == true)
    {  
        Serial.println ("DeactivateMainMagnet() -- Powering Down...");

        // once the WigWag has been activated, we will wait xxx seconds before we activate again
        mlDutyCycleTime = mlDutyCycleMaxTime;
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
    // We have one relay (a DPDT), that selects the left or right magnet
    // We use the DPDT relay, to switch the direction of current thru the two coils (right and left).
    // So when we deactivate the left magnet, we are actually activating the right magnet.

    Serial.println("RightMagnetActivate() ");

    // activate the direction control DPDT relay     
    digitalWrite(kRightMagnetControl, true);   

    mbMagnetDirection = true;
  
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
    // We have one relay (a DPDT), that selects the left or right magnet
    // We use the DPDT relay, to switch the direction of current thru the two coils (right and left).
    // So when we deactivate the left magnet, we are actually activating the right magnet.

    Serial.println("LeftMagnetActivate() ");

    // activate the direction control DPDT relay     
    digitalWrite(kLeftMagnetControl, false);   

    mbMagnetDirection = false;
  
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
