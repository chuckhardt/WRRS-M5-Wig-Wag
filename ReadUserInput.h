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

#ifndef ReadUserInput_h
#define ReadUserInput_h

#include "WigWag.h"
#include "MainMagnetControl.h"



#define kMaxDownCount 30000


// *****************************************************************************************
//
// ReadUserInput()
//
// Read the switches the user is allowed to set    
//
// ****************************************************************************************
//void ReadUserInput(void);

// State Events for the input switches
enum SwitchEventStates
{
    SwitchNoEvent = 0,
    SwitchFirstEvent = 1,
    SwitchSecondEvent = 2
    
}; 

class UserInput: public MainMagnet
{

    SwitchEventStates SwitchSeq = SwitchNoEvent;
    int miSwitchNumber;
    char mcFirstSerialCmd[15];
    char mcSecondSerialCmd[15];
  
    public:

    typedef void (*callback_func_ptr)();
    callback_func_ptr cb_func;
   

    // *****************************************************************************************
    //
    // UserInput()
    //
    // Our constructor, which takes as its input, the Ardruino IO pin number that it will uses 
    // when reading in the physical switch data.   
    //
    // ****************************************************************************************
    UserInput(int);

    // *****************************************************************************************
    //
    // SetSwitchSequence()
    //
    // Set the current switch debouncing sequence (1st, or 2nd)
    //
    // ****************************************************************************************
    void SetSwitchSequence(int);

    // *****************************************************************************************
    //
    // GetSwitchSequence()
    //
    // Get the current switch debouncing sequence (1st, or 2nd)
    //
    // ****************************************************************************************
    int  GetSwitchSequence(void);

    // *****************************************************************************************
    //
    // ReadSwitch()
    //
    // Read the specified limit or user switch and gets its current state.
    //
    // ****************************************************************************************  
    void ReadSwitch(char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght], callback_func_ptr ptr);  

  
};  // endof UserInput



#endif
