Essential Arduino Components and libraries:
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

- Hall Effect Sensor: Standard model

Non essential components used in my personal setup:
- 3.7V 14500 Li-ion battery (2x)
- Voltage Regulator 7-12V > 5V


