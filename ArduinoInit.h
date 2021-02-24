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
#ifndef ArduinoInit_h
#define ArduinoInit_h

#include "Arduino.h"
#include "MainMagnetControl.h"
#include "WigWag.h"

// ****************************************************************************************
//
// InitializeArduinoIOpins()
//
// Initializes the IO pins as either input or output.
// Then sets the outputs their initial state (high or low).
//
// ****************************************************************************************
void InitializeArduinoIOpins();

// *****************************************************************************************
//
// LeftMagnetActivateWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void LeftMagnetActivateWrapper(void);

// *****************************************************************************************
//
// LeftMagnetActivateWrapperTimeout()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void LeftMagnetActivateWrapperTimeout(void);

// *****************************************************************************************
//
// RightMagnetActivateWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void RightMagnetActivateWrapper(void);

// *****************************************************************************************
//
// RightMagnetActivateWrapperTimeout()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void RightMagnetActivateWrapperTimeout(void);

// *****************************************************************************************
//
// ActivateMainMagnetWrapper()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void ActivateMainMagnetWrapper(void);

// *****************************************************************************************
//
// DutyCycleTimer()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// *****************************************************************************************
void DutyCycleTimer (void);

// *****************************************************************************************
//
// LedFlash()
//
// We are going to flash the main board LED on and off just to let us know that everthing
// is running.
//
// *****************************************************************************************
void LedFlash (void);

#endif
