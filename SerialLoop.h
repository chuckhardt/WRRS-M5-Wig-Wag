// ***************************************************
//
// WigWag Signal Program
//
// This software was developed to operate on an Arduino Uno
// microprocessor board.  It controls the WRRS Auto Wig-Wag
// Model #5 at the Southeastern Railway Museum.
//
// Author: C. Hardt
// Target Platform: Arduino Uno
// Date: 04/19/20
//
// ****************************************************

#ifndef SerialLoop_h
#define SerialLoop_h

#include "Arduino.h"
#include "ReadUserInput.h"
#include "MainMagnetControl.h"
#include "WigWag.h"

// **************************************************************************
// SerialPortType
//
// Serial Port Class that is used to read and write to from the serial port
//
// **************************************************************************
class SerialPortType 
{
    public:

    // **************************************************************************
    //  SerialLoop()
    //
    //  If there is data waiting on the serial port, read in the data.  Then 
    //  parse and determine if we need to act on any of the commands.
    //
    // **************************************************************************
    void SerialLoop(char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght]); 

    // **************************************************************************
    //  ConvertCmdLineToArrayOfArgs()
    //
    //  Parse the string returned by serial port and convert it to an array of
    //  arguments.
    //
    // **************************************************************************
    int  ConvertCmdLineToArrayOfArgs(String sCmdLine, char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght]);

}; // SerialPortType

#endif
