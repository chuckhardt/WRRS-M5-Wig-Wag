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

#ifndef MainControl_h
#define MainControl_h

#include "WigWag.h"
#include "Timer.h"

#define kMaxOperationalTimeLimt 30000
#define kMaxWigWagDutyCycleInActiveDuration 600000


#define RIGHT true
#define LEFT  false


// ****************************************************************************************
//
// MainMagnet
//
// This class controls all operations of the magnets (on/off, Left/right)
//
// ****************************************************************************************
class MainMagnet
{
    private:

        long mlCurrentTime;
        long mlExpirationTime;
        bool mbMainMagnetOn;
        bool mbMagnetDirection;   // stores the magnet diredtion, left or right
        long mlWigWagRestartInhibitTimeWindow;
        bool mbDownCountReached;
        long mlDutyCycleMaxTime;
        Timer mDirectionTimer;
        int   miLeftDirectionTimerID;
        int   miRightDirectionTimerID;
        

    public:

    // ****************************************************************************************
    //
    // MainMagnet()
    //
    // Our Constructor for the class
    //
    // ****************************************************************************************
    MainMagnet(void);

    // ****************************************************************************************
    //
    // UpdateDutyCycleInActiveWindow()
    //
    // When called our duty cycle down count is decremented.  When we hit 0, the WigWag
    // can be activated again.
    //
    // ****************************************************************************************
    void UpdateDutyCycleInActiveWindow(void);
    
    // ****************************************************************************************
    //
    // ActivateMainMagnet()
    //
    // Calling this function will set an expiration time sometime in the future (based on default
    // value set in h file)  So this function can act to start the signal (pull in the main relay)
    // or it will just reset the timer count to some point in the future.
    //
    // ****************************************************************************************
    void ActivateMainMagnet(void);
    
    // ****************************************************************************************
    //
    // Operations()
    //
    // This function determines if the max operating time of the Wig-Wag has been exceeded
    // If it has, main power is dropped out, and Wig-Wag opertion ends.
    //
    // ****************************************************************************************
    void Operations(char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght]); 
    
    // ****************************************************************************************
    //
    // DeactivateMainMagnet()
    //
    // This function shuts off the main control relay which terminates Wig-Wag operations.
    //
    // ****************************************************************************************
    void DeactivateMainMagnet(void);

    // *****************************************************************************************
    //
    // RightMagnetActivate()
    //
    // Select the Right Magnet, doing so will result in deactivating the left magnet, since
    // we use a single DPDT relay to swap the magnet field direction   
    //
    // ****************************************************************************************
    void RightMagnetActivate(void); 
    
    // *****************************************************************************************
    //
    // LeftMagnetActivate()
    //
    // Select the Left Magnet, doing so will result in deactivating the right magnet, since
    // we use a single DPDT relay to swap the magnet field direction   
    //
    // ****************************************************************************************
    void LeftMagnetActivate(void);

    // *****************************************************************************************
    //
    // GetDirectionMagnetSelection()
    //
    // Returns the current direction magnet relay selection (right = true, left = false)
    //
    // ****************************************************************************************
    bool GetDirectionMagnetSelection(void);


    // testing the callback function
    typedef void (MainMagnet::*callback_func_ptr)();
    callback_func_ptr cb_func;

}; // endof MainMagnet

#endif
