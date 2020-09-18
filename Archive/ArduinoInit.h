// ***************************************************
//
// WigWag Signal Program
//
// This software was developed to operate on an Arduino Uno
// microprocessor board.  It controls the WRRS Auto WigWag
// Model #5 at the Southeastern Railway Museum.
//
// Author: C. Hardt
// Date: 04/19/20
//
// ****************************************************
#ifndef ArduinoInit_h
#define ArduinoInit_h

#include "Arduino.h"
#include "MainMagnetControl.h"
#include "WigWag.h"



// ***************************************************
//
// InitializeArduinoIOpins()
//
// Initializes the IO pins as either input or output.
// Then sets the outputs their initial state (high or low).
//
// ****************************************************
void InitializeArduinoIOpins();


// ***************************************************
//
// IncrementTask()
//
// Increment our task switch index to the next task
//
// ****************************************************
void IncrementTask(eTaskCount& eTaskNum);


// *****************************************************************************************
//
// LeftMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void LeftMagnetActivate(void);

// *****************************************************************************************
//
// RightMagnetActivate()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void RightMagnetActivate(void);

// *****************************************************************************************
//
// ActivateMainMagnet()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void ActivateMainMagnet(void);

// *****************************************************************************************
//
// DutyCycleTimer()
//
// This is a wrapper function for the Magnet Control Class.  Wrapper was needed to the 
// problems with the way Arduino handles callback functions.
//
// ****************************************************************************************
void DutyCycleTimer (void);

// *****************************************************************************************
//
// LedFlash()
//
// We are going to flash the main board LED on and off just to let us know that everthing
// is running.
//
// ****************************************************************************************
void LedFlash (void);

#endif
