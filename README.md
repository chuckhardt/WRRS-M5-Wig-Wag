# WRRS-M5-Wig-Wag
WRRS Model 5 Wig-Wag Arduino Project
This source code is used on an Arduino Uno.  
It is a simple program that controls a 1930s Railroad WRRS Model 5 Auto Flagman (Wig-Wag).
The Auto Flagman was the precursor to a more modern roadway crossing guard.
This particular Auto Flagman resides at the Southeastern Railway Museum in Duluth, GA, USA
The Model 5 contained 3 magnets.  
The main magnet, which was on whenever power was applied.
A right a left magnet that we alternate the current direction thru.
There are three inputs to the Arduino:  
  1) A user push button that starts a 30 second on sequence
  2) A right limit switch that triggers the left magnet
  3) A left limit switch that triggers the right magnet
There are two outputs from the Arduino:
  1) Main Power relay that activates the main magnet, and also provides power to the right and left magnet control relay.
  2) The right/left power control magnet (either the right magnet is select or the left magnet, but not both).
  
  
  
