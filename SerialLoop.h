#include "Arduino.h"
#include "ReadUserInput.h"
#include "MainMagnetControl.h"
#include "WigWag.h"

#ifndef SerialLoop_h
#define SerialLoop_h



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
      void SerialLoop(char cParseArgList[kMaxCommandsSupported][kMaxCommandLenght]); //, MainMagnet *MainController, DirectionMagnet *DirectionControl, UserInput *LeftSwitch, UserInput *RightSwitch, UserInput *UserSwitch);

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
