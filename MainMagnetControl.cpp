#include "Arduino.h"
#include "ArduinoInit.h"
#include "WigWag.h"
#include "MainMagnetControl.h"

#define kMatch 0


// ****************************************************************************************
//
// Operations()
//
// This function updates the operational mode down count.   If the user presses the activation
// button, the count will be incremented.  This function continues to subtract from the down 
// count until it reaches zero.  When this happens, the magnets are switched off. 
//
// ****************************************************************************************
void MainMagnet::Operations(char cParseArgList[kMaxCommandsSupported][25] ) //, MainMagnet DirectionActionControl)
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
