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
#ifndef taskstate_h
#define taskstate_h


// enum for all possible WigWag Task State
enum StateMachine 
{
    ReadUserInputRightLimit,
    ReadUserInputLeftLimit,
    ReadUserInputUserSwitch,
    CheckMainMagnet,
    ReadSerialPort
};

// *****************************************************************************************
//
// WigWagTaskStates Class
//
// This class is used to create and increment the overall task switch state
//
// *****************************************************************************************
class WigWagTaskStates 
{ 

    public: 
    
        // our current task state
        StateMachine WigWagTaskSwitchState;
    
        // *****************************************************************************************
        //
        // WigWagTaskStates::WigWagTaskStates()
        //
        // Constructor for the WigWagTaskStates Class
        //
        // *****************************************************************************************
        WigWagTaskStates(void);

        // *****************************************************************************************
        //
        // WigWagTaskStates::getTaskState()
        //
        // Returns the current task state
        //
        // *****************************************************************************************
        StateMachine getTaskState(void);
    
        // *****************************************************************************************
        //
        // WigWagTaskStates::operator++(int)
        //
        // Class method which creates an operator increment function.  We will be incrmenting the
        // the task swithc states, using the ++ operation
        //
        // *****************************************************************************************
        void operator++(int);
 
}; // endof WigWagTaskStates

#endif
