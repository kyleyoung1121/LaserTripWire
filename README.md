# LaserTripWire
Here is the code for my laser tripwire used in a game of capture the flag.

This code is run contiunously from my arduino. There are several components also connected to the arduino.
  PIR Sensor- This motion sensor checks for anyone coming to steal my flag.
  5V Relay- If motion is found, the relay activates, completing the circuit for a powerful laser.
  Light Dependent Resistor- If the laser lands on the LDR, its resistance is quite high.
  
The laser is bounced off of several mirrors, and lands on the LDR.
When someone crosses a laser, or disconnects the laser's power, or moves a mirror, the laser will
no longer fall on the LDR. When this happens, the resistance will lessen and this change can be
detected through this code. 

When a beam is tripped, the beamTrip() function is called, activating an LED and also another channel of the relay.
The second channel of the relay completes a circuit in an electric doorbell button, sending a push signal.
When the doorbell receiver hears that signal, it dings and I will be notified of an attacker.

Notes:
The relay is set up strange. Writing LOW to the relay pin will turn the laser on. Same with the doorbell button.
The laser should work regardless of ambient light. The first second or two of laser activation average the LDR
value with the beam resting on it. If the resistance dips significantly below that, a trip is assumed.
The code takes around 30sec to get up and running, giving time for the PIR to get adjusted and able to detect.
The lasers remain off until motion is found to extend laser life (don't want to push the duty cycle)
If no motion is seen at the regular 15sec check, the lasers will turn off.
After the lasers have been on for 15sec, and motion is still seen, they stay on and check in another 15sec.
If the Ardunino is connected to a computer, the program will print out timestamps of motion to the serial monitor.
The laser is powered off of 2 AA (3V), instead of the 3.7V battery it came with.
The arduino is powered off of 1 9V battery, connected in series to a switch to allow for quick power on and off.
