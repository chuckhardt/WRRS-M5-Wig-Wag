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

#include "taskstate.h"

// *****************************************************************************************
//
// WigWagTaskStates::WigWagTaskStates()
//
// Constructor for the WigWagTaskStates Class
//
// *****************************************************************************************
WigWagTaskStates::WigWagTaskStates(void)
{ 
    this->WigWagTaskSwitchState = ReadUserInputRightLimit;
    
} //endof WigWagTaskStates::WigWagTaskStates()


// *****************************************************************************************
//
// WigWagTaskStates::getTaskState()
//
// Returns the current task state
//
// *****************************************************************************************
StateMachine WigWagTaskStates::getTaskState()
{

  return (WigWagTaskSwitchState);
}

// *****************************************************************************************
//
// WigWagTaskStates::operator++(int)
//
// Class method which creates an operator increment function.  We will be incrmenting the
// the task swithc states, using the ++ operation
//
// ******************************************************************************************
void WigWagTaskStates::operator++(int)
{ 
    switch (WigWagTaskSwitchState)
    {
        case ReadUserInputRightLimit:

            WigWagTaskSwitchState = ReadUserInputLeftLimit;
            break;
        
        case ReadUserInputLeftLimit:

            WigWagTaskSwitchState = ReadUserInputUserSwitch;
            break;

        case ReadUserInputUserSwitch:

            WigWagTaskSwitchState = CheckMainMagnet;
            break;
  
        case CheckMainMagnet:

            WigWagTaskSwitchState = ReadSerialPort;
            break;

        case ReadSerialPort:

            WigWagTaskSwitchState = ReadUserInputRightLimit;
            break;

        default:
            WigWagTaskSwitchState = ReadUserInputRightLimit;
    }         
    
} // endof WigWagTaskStates::operator++()
