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
1. Calibrating the compass-pointer. The stepper motor can position the pointer with an accuracy of 2048 steps per rotation, but this specific motor does not know where the pointer is after it has been used, so it will calibrate at the start of the process to a set position using a magnet and a Hall Effect Sensor.
2. Triangulating a GPS position. The GPS module will
