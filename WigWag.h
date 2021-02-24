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

#ifndef WigWag_h
#define WigWag_h

#define kMaxCommandLenght     10
#define kMaxCommandsSupported 6

#define kArduinoIOpin02 2   
#define kArduinoIOpin03 3
#define kArduinoIOpin04 4
#define kArduinoIOpin05 5

#define kArduinoIOpin06 6
#define kArduinoIOpin07 7
#define kArduinoIOpin08 8
#define kArduinoIOpin09 9
#define kArduinoIOpin10 10
#define kArduinoIOpin11 11
#define kArduinoIOpin12 12

#define kArduinoIOpin13 13

#define kRightMagnetControl kArduinoIOpin05
#define kLeftMagnetControl  kArduinoIOpin05
#define kMainMagnetControl  kArduinoIOpin06

#define kRightLimitSwitch kArduinoIOpin02
#define kLeftLimitSwitch  kArduinoIOpin03
#define kUserInputSwitch  kArduinoIOpin04

#define kRightLimitLED    kArduinoIOpin08
#define kLeftLimitLED     kArduinoIOpin09

#define kStatusLED        kArduinoIOpin13 


// *****************************************************************************************
//
// LeftMagnetActivatWrappere()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the callback function.
//
// ****************************************************************************************
void LeftMagnetActivateWrapper(void);

// *****************************************************************************************
//
// RightMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the callback function.
//
// ****************************************************************************************
void RightMagnetActivateWrapper(void);


// *****************************************************************************************
//
// ActivateMainMagnetWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the callback function.
//
// ****************************************************************************************
void ActivateMainMagnetWrapper(void);

// *****************************************************************************************
//
// TaskSwitchMain()
//
// This is the main loop for the Wig-Wag controller. It gets called every xxx ms.   
// Each time it is called, a different function will be called.  A poor man's
// operating system.    
//
// ****************************************************************************************
void TaskSwitchMain(void);

#endif
