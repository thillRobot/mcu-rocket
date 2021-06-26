# mcu-rocket
Data Acquisition and Control of a Model Rocket with an MCU based Flight Computer 

Tristan Hill - 2021                             

## Overview
  This project contains MCU programs and other utilities related to data acquisition and control of a model rocket. The rocket payload contains an on-board    flight computer consisting of a micro-controller and sensor package for collecting flight data and environmental conditions during the launch of a model rocket. 
  
## Mission Information

Note: This is the README for Mission A: NANO-Rocket. To find information about a different mission or rocket flight computer, choose a different branch.

### Available Hardware
  #### Mission A: NANO-Rocket
    
  - MCU Board: Arduino Nano 3.0, MCU: Atmega 328p
  - Sensors:
    - BNO055 - Absolute Orientation Sensor
    - GY-521 (MPU 6050 MEMS) - 3 Axis Accelerometer, 3 Axis Gyroscope, and Temperature Sensor 
    - GY-68 (BMP180) - Barometric Pressure Sensor
    - DPS310 - Barometric Pressure, Temperature Sensor
  
  Additional Hardware:
    - 5v Ready SD Breakout Board + 16 GB micro SD card
    - Lipo Battery - 1s, 3.7v, 150 mAh 


### BNO055 - Absolute Orientation Sensor Board
   
  This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
  which provides a common 'type' for sensor data and some helper functions.
  
  To use this driver you will also need to download the Adafruit_Sensor
  library and include it in your libraries folder.
  
  You should also assign a unique ID to this sensor for use with
  the Adafruit Sensor API so that you can identify this particular
  sensor in any data logs, etc.  To assign a unique ID, simply
  provide an appropriate value in the constructor below (12345
  is used by default in this example).
  
#### BNO055 layout and Sensor Orientation
```  
       +----------+
       |         *| RST   PITCH  ROLL  HEADING
   ADR |*        *| SCL
   INT |*        *| SDA     ^            /->
   PS1 |*        *| GND     |            |
   PS0 |*        *| 3VO     Y    Z-->    \-X
       |         *| VIN
       +----------+
``` 
#### Conections to NANO328p     
```
  NANO | BNO055 Sensor Board
  
  SDA (11)-----SDA
  SCL (12)-----SCL
  
  5v-----------Vin
            (nc)3v 
  Gnd----------Gnd    
```

### 5v Ready Micro-SD Card Breakout Board 

#### Connections to NANO328p
```
  NANO | MicroSD Breakout
  
  SS(10/14)-------CS (set this pin in software?)
  SCK(13/17)-----CLK 
  MOSI(11/15)-----D1
  MISO(12/16)-----D0
  
  5v-----------5v
           (nc)3v 
  Gnd---------Gnd
``` 

#### Compile and Upload with `arduino-cli` for NANO328p

  Check for a connected board.
```
  arduino-cli board list
````
  Compile the script.
```  
  arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328 data-logger.ino
```
  Upload the script to the board.
```
  arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328 data-logger.ino
```

#### testing 

  Listen to the serial connection with minicom. `USB0` refers to a custom config file made with minicom.
```  
  minicom USB0      
```
  After running the script the data file (.txt) should be on the SD card. Remove the card and inpect with a PC. If the filename defined in the script is a new name, then a new file will be created. If the filename defined in the script matches a file on the SD card, then that file will be appended with a new set of data entries.

  mount the SD card
  ```
  sudo mount -t vfat /dev/sdc1 /media/fat32 -o rw,uid=1000,gid=1000
  
  ```

## Changelog  
  - This code began as example code from the Arduino library created 24 Nov 2010, modified 9 Apr 2012 by Tom Igoe
  - The example code 'DataLogger' was modified for this project
  - Example code 'Bunny' from the Arduino library BNO055 was also used. 
  - Modified by Tristan Hill - 06/24/2019 
  - Revised by Tristan Hill - 04/20/2021
  - switched to the MKR1010 Wifi board - 04/20/2021
  - tested BNO055 - working - values look good, not validated - 04/20/2021
  - test SD Card Breakout - working - data in files look good, not validated - 04/20/2021
  - added all the sensors as shown in the example code "read_all_data"
  - added and formatted README.md - 04/29/2021
  - added entry number and file number to data log format on SD card - 04/29/2021 
  - restructured main loop by adding functions printHeader, printData, printFooter - 05/05/2021 
  - more abstraction of structure, added initFile - 05/06/2021
  - tested hardware on NANO328p - 05/31/2021
  - added data_logger_BNO055, reduced version of data_logger to save code space on NANO - 06/01/2021
  - changed names data_logger and data_logger_BNO055 to data_logger_mkr,nano,test,archive - 06/05/2021
  - this code has grown to a point where it needs its own repository, goodbye /mcu - 06/05/2021
  - see https://github.com/thillRobot/data-logger.git for the continuation of this project - 06/05/2021
  - used git branches to handle different MCUs, added branches mkr-master and nano-master - 06/05/2021
