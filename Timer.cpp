/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Code by Simon Monk
 http://www.simonmonk.org
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Timer.h"

Timer::Timer(void)
{
  
}

int8_t Timer::every(char timerName[5], unsigned long period, void (*callback)(), int repeatCount)
{
    int8_t i = findFreeEventIndex();
	  if (i == -1) 
	      return -1;

	  _events[i].eventType     = EVENT_EVERY;
	  _events[i].period        = period;
	  _events[i].repeatCount   = repeatCount;
	  _events[i].callback      = callback;
	  _events[i].lastEventTime = millis();
	  _events[i].count         = 0;

    memset(_events[i].timerName, '\0', sizeof(timerName));
    strcpy(_events[i].timerName, timerName);

    //Serial.print(F("Timer Created: "));
    //Serial.print(i);
    //Serial.print(F(" Name: "));
    //Serial.println(_events[i].timerName);
  
	  return i;
}

int8_t Timer::every(char timerName[5], unsigned long period, void (*callback)())
{
	  return every(timerName, period, callback, -1); // - means forever
}

int8_t Timer::after(char timerName[5], unsigned long period, void (*callback)())
{
	  return every(timerName, period, callback, 1);
}

int8_t Timer::oscillate(uint8_t pin, unsigned long period, uint8_t startingValue, int repeatCount)
{
	  int8_t i = findFreeEventIndex();
	  if (i == -1) 
	     return -1;

	  _events[i].eventType = EVENT_OSCILLATE;
	  _events[i].pin = pin;
	  _events[i].period = period;
	  _events[i].pinState = startingValue;
	  digitalWrite(pin, startingValue);
	  _events[i].repeatCount = repeatCount * 2; // full cycles not transitions
	  _events[i].lastEventTime = millis();
	  _events[i].count = 0;
	  return i;
}

int8_t Timer::oscillate(uint8_t pin, unsigned long period, uint8_t startingValue)
{
	  return oscillate(pin, period, startingValue, -1); // forever
}

int8_t Timer::pulse(uint8_t pin, unsigned long period, uint8_t startingValue)
{
	  return oscillate(pin, period, startingValue, 1); // once
}

void Timer::stop(int8_t id)
{
    
     // verify that the timer was in use, if not print an error
     if (_events[id].eventType == EVENT_NONE)
     {
         //Serial.print(F("Timer Stop Error: "));
     }  
     else
     {
       _events[id].eventType = EVENT_NONE;
       
       //Serial.print(F("Timer Deleted: "));
      
       _events[id].period        = 0;
       _events[id].repeatCount   = 0;
       _events[id].callback      = NULL;
       _events[id].lastEventTime = 0;
       _events[id].count         = 0;
        memset(_events[id].timerName, '\0', 5);
       
     } 
}

void Timer::update(void)
{
    if (mbPauseFlag != true)
    {
      	for (int8_t i = 0; i < MAX_NUMBER_OF_EVENTS; i++)
      	{
      		if (_events[i].eventType != EVENT_NONE)
      		{
      			_events[i].update();
      		}
      	}
    }
}

int8_t Timer::findFreeEventIndex(void)
{
  if (mbPauseFlag != true)
  {
    	for (int8_t i = 0; i < MAX_NUMBER_OF_EVENTS; i++)
    	{
      		if (_events[i].eventType == EVENT_NONE)
      		{
             //Serial.print(F("Timer Created: "));
             //Serial.println(i);
      			 return i;
      		}
     }
  } 
	return -1;
}

// **************************************************************************
//  Pause()
//
//  Pause the event timer. 
//
// **************************************************************************

void Timer::Pause(void)
{
    mbPauseFlag = true;
    Serial.println(F("Timer Pause"));
    
}  // Endof Timer::Pause

// **************************************************************************
//  Resume()
//
//  Resume the event timer
//
// **************************************************************************

void Timer::Resume(void)
{
    mbPauseFlag = false;
    Serial.println(F("Timer Resume"));
    
}  // Endof Timer::Resume
 
