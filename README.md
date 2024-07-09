# MiniCar
STM32 based assisted driving for a radio controlled car.

## Features
- [x] RF communication
- [x] Motor controlling
- [ ] Reading voltage and current from the motors
- [x] Gyroscope and Accelerometer data
- [ ] Sensor fusion with gyroscope, accelerometer and magnetometer
- [ ] Distance measurement in multiple directions for collision detection
- [ ] Battery monitoring
- [ ] Changing settings "On the go" from a PC or laptop through RF communication without flashing the MCU. This would require the radio control to be plugged into the computer.

## Structure of the project
The above mentioned features will be divided into multiple modules.

### Modules
- #### SPI Communication:
	This would cover the SPI interfaces. Any communication request would go through this module.
- #### RF module:
	Provides a communication between the transmitter and the receiver. The received drive directions (or mode changes) can be accessed from this module.
	Uses SPI interface to communicate with hardware.
- #### Gyro module:
  	It's task is to handle the gyroscope, magnetometer and accelerometer communication from the MPU9050 module through SPI.
- #### Motion processing:
  	Converts raw MPU data to human understanding. Calculates motion of the MPU module with sensor fusion.
- #### Motor controller:
	A common interface to set the speeds and other features of the motors.
- #### Motor driver:
	Connects the motor controller with the hardware
