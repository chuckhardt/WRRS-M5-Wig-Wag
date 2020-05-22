#include <stdio.h> 
#include "Arduino.h"
#include "ArduinoInit.h"
#include "WigWag.h"
#include "SerialLoop.h"
#include "ReadUserInput.h"
#include "MainMagnetControl.h"


#define kMaxLengthOfCmdLine   100


// **************************************************************************
//  ConvertCmdLineToArrayOfArgs()
//
//  Parse the string returned by serial port and convert it to an array of
//  arguments.
//
// **************************************************************************

int SerialPortType::ConvertCmdLineToArrayOfArgs(String sCmdLine, char cParseArgList[kMaxCommandsSupported][25])
{
    char  *cCmdParsedFromCmdLineString;
    int   iCountOfArgsFound = 0;
    bool  bDoneFlag;
    int   iIndex;
    int   iLength;
    int   iNumberOfArgFoundOnTheCmdLine;
    char  cCmdLine[kMaxLengthOfCmdLine];

    // The length of the line must be less than our max limit
    if(sCmdLine.length() < kMaxLengthOfCmdLine)
    {
        // convert from a string to an array of char
        strcpy(cCmdLine, sCmdLine.c_str());
 
        // Clear the cCmdParsedFromCmdLineString array
        for (iIndex = 0; iIndex < kMaxCommandsSupported; iIndex++)
            cParseArgList[iIndex][0] = (char) '\n';
        
        iCountOfArgsFound = 0;
        bDoneFlag = false;
        iNumberOfArgFoundOnTheCmdLine = 0;
        while(bDoneFlag == false)   
        {
            // max args must be less than our defined limit
            if(iCountOfArgsFound < kMaxCommandsSupported)
            {
                // we pass in the string to serach for only on the first pass
                // after that, we pass in NULL, which makes no sense at all
                if (iCountOfArgsFound == 0)
                    cCmdParsedFromCmdLineString = strtok(cCmdLine, " ");
                else
                    cCmdParsedFromCmdLineString = strtok(NULL, " ");
    
                // if we hit a NULL, then we are done, exit  
                if (cCmdParsedFromCmdLineString != NULL)
                {
                    // the command can not exceed the predefined size of the buffer
                    if(strlen(cCmdParsedFromCmdLineString) < kMaxLengthOfCmdLine)
                    {
                        strcpy(cParseArgList[iCountOfArgsFound], cCmdParsedFromCmdLineString);
                        iLength = strlen(cCmdParsedFromCmdLineString);
                        cParseArgList[iCountOfArgsFound][iLength + 1] = '\n';
                        iCountOfArgsFound++;
                        iNumberOfArgFoundOnTheCmdLine++;
                    }
                    else
                    {
                        Serial.println("ERROR: Max Length of a Single CMD Exceeded");
                        bDoneFlag = true;
                        iNumberOfArgFoundOnTheCmdLine = 0;
                    }
                }

                // we are done parsing the cmd line args
                else
                {
                    bDoneFlag = true;
                }
    
            } // endif iCountOfArgsFound

            // we exceeded our max limit
            else
            {
                Serial.println("Error: Max Number of CmdLine Args Exceeded");
                bDoneFlag = true;
                iNumberOfArgFoundOnTheCmdLine = 0;
            }
                
        }  // endof While

    } // endif

    return (iNumberOfArgFoundOnTheCmdLine);
  
}  // ConvertCmdLineToArrayOfArgs()

// **************************************************************************
//  SerialLoop()
//
//  If there is data waiting on the serial port, read in the data.  Then 
//  parse and determine if we need to act on any of the commands.
//
// **************************************************************************

void SerialPortType::SerialLoop(char cParseArgList[kMaxCommandsSupported][25])                         
{
    extern int iLightArrayIndex;
    String     sCommand;
    //char       cParseArgList[kMaxCommandsSupported][25];  
    int        iNumberOfArgFoundOnTheCmdLine; 
    bool       kMatch = false;

    // first thing we are going to do, is make sure the array of arguments has been cleared
    for (int x = 0; x < kMaxCommandsSupported;x++)
        cParseArgList[x][0] = '\0';


    // Is there any input waiting on the serial port
    if(Serial.available())
    {
        // read all data waiting on the serial input until we hit the newline char
        sCommand = Serial.readStringUntil('\n');

        // parse the command line and convert to an array of arguments
        iNumberOfArgFoundOnTheCmdLine = ConvertCmdLineToArrayOfArgs(sCommand, cParseArgList);


        // if we have at least one cmd line arg, then process this
        if (iNumberOfArgFoundOnTheCmdLine > 0)
        {

            if(kMatch == strcmp(cParseArgList[0],"switch"))
            {
                             
                // if we have been told so, then set the direction control relay to right
                if(kMatch == strcmp(cParseArgList[1],"right"))
                {
                    Serial.println ("CMD: switch => right");
                    
                }

                // if we have been told so, then set the direction control relay to left
                else if(kMatch == strcmp(cParseArgList[1],"left"))
                {
                    Serial.println ("CMD: switch => left");
                    
                }
                
                // if we have been told so, then set the direction control relay to left
                else if(kMatch == strcmp(cParseArgList[1],"user"))
                {
                    Serial.println ("CMD: switch => user");
                    
                }
                
                else
                  Serial.println ("CMD: switch - ERROR");    
               
            }
            
            
            
            else if(kMatch == strcmp(cParseArgList[0],"help"))
            {
                Serial.println ("help - this menu");
                Serial.println ("  main       activate | deactivate");
                Serial.println ("  direction  right | left");
                Serial.println ("  switch     right | left | user");
                
            }
            
            else
            {
                Serial.print("ERROR, Invalid CMD: ");
                Serial.println(sCommand);
            }

        }  // endif iNumberOfArgFoundOnTheCmdLine      

    }  // endif Serial.available()

}  // SerialLoop()
