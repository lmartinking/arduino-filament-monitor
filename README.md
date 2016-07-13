Arduino 3D Printer Feeder Monitor
=================================

Some simple code to monitor the filament feed to a 3D printer.

When it is detected that the filament is no longer moving (but the feeder motor is moving),
the control pin will be triggered. 

I wrote this for a friend who has a bunch of 3D printers running continuously.

Hardware Requirements
---------------------

* 2x Optical encoders (eg: YUMO HKT30)
* Feeder wheel with double shaft (so the optical encoder can be attached)
* Stepper motor with double shaft
* Ardunio
* Something connected to the "control" pin (eg: an solenoid, alarm, the printer itself, etc)

Hardware Set Up
---------------

* Connect the first optical encoder (ENC_1_PIN) to the roller/feeder.
* Connect the second optical encoder (ENC_2_PIN) to the stepper motor which feeds through the filament.
* Connect your alarm, solenoid, etc to the control pin (CTRL_PIN).

These pins are configurable in the source code.

NOTE: It is required that the two optical encoders are connected to interrupt pins. Check your arduino documentation, as these differ per model.

Software Set Up
---------------

1. Open the source code in the Arduno IDE
2. Edit the PIN configuration (if needed)
3. Adjust check interval and threshold (if required)
4. Adjust control pin active/inactive signals (may depend on what it is connected to)
5. Build / Flash
6. Open the serial monitor, set the baud rate to 9600 and verify output

When the unit triggers the control pin, it will keep monitoring and may trigger the pin again.
When the filament stoppage has been fixed, just reset the arduino.

