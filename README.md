# MiniCar
STM32 based assisted driving for a radio controlled car.

## Features
- [ ] RF communication
- [ ] Motor controlling with voltage and current feedback from the motors
- [ ] Gyroscope and Accelerometer data for speed
- [ ] Distance measurement in multiple directions for collision detection
- [ ] Battery monitoring
- [ ] Changing settings "On the go" from a PC or laptop through RF communication without flashing the MCU. This would require the radio control to be plugged into the computer.

## Structure of the project
The above mentioned features will be divided into multiple modules.

### Modules
- #### SPI Communication:
	This would cover the SPI interfaces. Any communication request would go through this module.
- #### RF communication:
	Provides a communication between the transmitter and the receiver. The received drive directions (or mode changes) can be accessed from this module.
	Uses SPI interface to communicate with hardware.
- #### Motor controller:
	A common interface to set the speeds and other features of the motors.
- #### Motor driver:
	Connects the motor controller with the hardware
