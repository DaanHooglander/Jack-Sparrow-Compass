In this ReadMe:
 1. Link to 3D files of compass physical product.
 2. Required components and libraries.
 3. Functionality of the product and context.
 4. Description of the code.

This project was created as an elective project for the Bachelor's degree in Mechanical Engineering. As a mechanical engineer, we received little education on the field of programming and mechatronics, so I used this elective project to teach myself more about it. The code is not perfect and it will be updated according to personal development wishes.

=== [ 1 ] ===
Link to 3D files of final product: https://www.printables.com/model/688733-jack-sparrow-compass


=== [ 2 ] ===
Essential Arduino Components and libraries:

Links to libraries are not provided as libraries quickly become obsolete.

Current version libraries should not be difficult to find with the following information.

- Arduino: ATmega2560

  !! This code only works for an ATmega2560. When using a different microcontroller, make sure the serial and reception/transmission protocols work. 
  Possibly additional libraries are required when using a different model.

- GPS Module: Neo-8M

  library: TinyGPS++.h   (not in standard Arduino database)


- Magnetometer: QMC5883L

  library: QMC5883L.h    (not in standard Arduino database)

- Stepper motor: 28BYJ-48
- Motor Driver: ULN2003

  library: stepper.h      (in Arduino database)

- Hall Effect Sensor: Standard model + magnet.


Non essential components used in my personal setup:

- 3.7V 14500 Li-ion battery (2x)
- Voltage Regulator 7-12V > 5V


=== [ 3 ] === Functionality of the product and context.
The compass is based on Jack Sparrow's magic compass of the Pirates Of The Caribbean movie series, where the compass will point to whatever the person holding it desires most in life.
This compass was created to always point to the nearest McDonald's. However, it does not work everywhere around the planet.

About the McDonald's locations:

The code contains an array of coordinates of (supposedly) every McDonald's in The Netherlands, my country of residence, and every McDonald's in Vienna. 
Later in this file it will be explained how the array is implemented in the code, but if preferable this array could be swapped out for an array with different coordinates for different locations.
An API has been considered, to communicate with an external database like Google Maps to apply this compass worldwide. The scope of the project stretched to making a functional compass within a certain amount of time, but not to making a perfect compass that works everywhere. The easier option of a data-array was chosen.

The compass has been designed to point to the nearest McDonald's. This is done by the following steps:

Primary function:

1. Calibrating the compass-pointer. The stepper motor can position the pointer with an accuracy of 2048 steps per rotation, but this specific motor does not know where the pointer is after it has been used, so it will calibrate at the start of the process to a set position using a magnet and a Hall Effect Sensor.
2. Triangulating a GPS position. The GPS module will find its own coordinates. This can take a couple of minutes. The result will be a latitude and longitude.
3. Using the GPS coordinate, a trigonometric calculation will be made to determine what the closest McDonald's is, and what the required angle is to drive the pointer.
4. The heading in regards to the north will be detemined using a magnetometer. This is done in case the compass has not been aligned to the north. That way, it can compensate for any direction it is headed in and still point the correct way.
5. Using the determined angle and heading, the motor will be powered to take the specified steps and point to the nearest McDonald's



Secondary function:


6. After the initial process, the pointer will be updated every three seconds in case the heading has changed.
7. Since the magnetometer is not prefectly accurate, the updating of the pointer can lead to an accumulation of inaccuracies and result in a wrong direction, so every minute the pointer will be calibrated and repositioned.
8. Every two minutes the calculation of the nearest McDonald's will be remade. The assumption is that the GPS coordinates do not change drastically over time, so they do not need to be updated as frequently as the header direction.


=== [ 4 ] === Description of the code

Void 1: GPS()

Using the TinyGPS++.h library the GPS coordinate will be triangulated and stored in the flash memory as doubles 'latitude' and 'longitude.' The reception of the transmission is done in a while-loop. The function is set up in a way that the different problems can be troubleshot when there is no GPS found.
- Serial1.avaialble: Using the integrated serial reader of the ATmega under Serial1 protocol, the system checks whether a signal is coming through. If not, the pins are incorrectly connected, or the signal is not being read.
- gps.encode: If a signal comes through it needs to be encoded. The signal needs to be understood and deciphered using the protocols of the library.
- gps.location.isValid: While a transmission is coming through and has been correctly read, that does not mean that the result necessarily makes sense. This line verifies whether the result corresponds with what could be coordinates.
- If all of that checks out, the signal will be read. Because of the nature of triangulation, the first ping of a satellite won't be accurate. It takes time to find an accurate fix, so after 50 succesful pings, the coordinates will be stored in the flas memory.

- If the signal can be read correctly and all of the components are working as they should be, but there is still no GPS location, that means that the module hasn't succesfully connected with the necessary satellites yet. In this case a message will be printed saying the location is not valid, indicating the problem does not lie in the earlier mentioned causes.

Void 2: MotorCalibrate()

The motor does not know its position, so it needs to calibrate before the angle of rotation is given. Using this function, the motor will spin and turn itself off when the Hall Effect Sensor detects the magnet passing by.

Void 3: turnOffMotorPins()

Stepper motors consume electricity in stationary position and maintain their torque. This means that the motor will behave as if it were spinning even while standing still, meaning that if left on it will heat up considerably. To prevent overheating the components the pins can be turned off during a period where no rotation is required. A compass pointer does not need to maintain its torque, so there is no problem powering off the control pins.

Void 4: ReadHeading()

- Compass.read: This function allows the library of the Magnetometer to read the x, y an z axis in regards to the north respectively. The library will then decipher the signal.

- HeadingRadians: The heading will be calculated using the atan2 function on the x- and y-axis. The result is an angle in radians.

- Once again, the sensor could have trouble starting up, so the heading will be stored after the 20th signal that comes through.

Void 5: FindMacAndRotation()

- With a for-loop, the two dimensional array with the coordinates will be considered. A simple calculation using the deltas between GPS and McDonald's will be performed, followed by pythagoras to find the distance.
- By repeating this process for every instance of the array and comparing the last value to the one prior, it can be determined what the shortest distance is. This distance will be stored to the flash memory after the for-loop is done and the entire array has been examined.
- Using the deltas between the GPS and recently found nearest McDonald's, an angle can be calculated using the atan2 function. This will be the rotational angle that the motor has to spin. This value will have a 90° correction because the starting point for the angle and calibration point for the motor do not lie in the same position.

Void 6: UpdateSystem()

The above mentioned void functions will be called upon accordingly to make sure the system will calibrate, find its GPS, find the nearest McDonald's and rotation, heading and make the motor spin.

Void 7: UpdatePointer()

As earlier mentioned, the pointer should be updated more frequently than the entire system, so this process has its own void function with the GPS and calculations left out.


SETUP:

In the setup, the system will initialize the necessary components once, and run the UpdateSystem function.

LOOP:

- In the loop, the pointer will be updated every three seconds.
- The motor will be calibrated every minute to account for snowballing inaccuracies.
- The GPS will repeat its triangulation every two minutes to account for movement, to make sure that the pointer will be accurately updated according to a change in position.
