# data-logger
This project contains MCU programs to get sensor data and write it to a .txt file on an SD card.

  Write sensor data to a csv file on an SD card                         
  Tristan Hill - Tennessee Technological University- 2021                             

## Overview
  This program get sensor data and writes it to a .csv file on an SD card.
  
## Hardware Information
  
  Note: This README is specific to the MKR1010wifi MCU board. Choose the branch appropriate to the MCU in 
  in use for specific code.

### Available Hardware
  - Arduino Nano 3.0 328p
  - BNO055 - Absolute Orientation Sensor
  - GY-521 (MPU 6050 MEMS) - 3 Axis Accelerometer, 3 Axis Gyroscope, and Temperature Sensor 
  - GY-68 (BMP180) - Barometric Pressure Sensor
  - DPS310 - Barometric Pressure, Temperature Sensor
  - 5v Ready SD Breakout Board + 16 GB micro SD card
  - Lipo Battery - 1s, 3.7v, 150 mAh 


### BNO055 - i2c Sensor Board 
   
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
#### Conections to MKR1010wifi     
```
  MKR | BNO055 Sensor Board
  
  SDA (11)-----SDA
  SCL (12)-----SCL
  
  5v-----------Vin
            (nc)3v 
  Gnd----------Gnd     
```

### 5v Ready Micro-SD Card Breakout Board (from Adafruit.com) 

#### Connections to MKR1010wifi 
```
    MKR1010 | MicroSD Breakout
    
    GPIO(7)-----CS (set this pin in software)
    MOSI(8)-----D1
    SCK(9)-----CLK
    MISO(10-----D0 
    
    5v----------5v
            (nc)3v 
    Gnd--------Gnd
```
#### Connections to MEGA2560
```
  MEGA | MicroSD Breakout
  
  SS(53)-------CS (set this pin in software?)
  SCK(52)-----CLK 
  MOSI(51)-----D1
  MISO(50)-----D0
  
  5v-----------5v
           (nc)3v 
  Gnd---------Gnd
``` 
  
#### Compile and Upload with `arduino-cli` for MKR1010wifi

  Check for a connected board.
```
  arduino-cli board list
````
  Compile the script.
```  
  arduino-cli compile --fqbn arduino:samd:mkrwifi1010 data_logger_mkr.ino 
```
  Upload the script to the board.
```
  arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:samd:mkrwifi1010 data_logger_mkr.ino
```

#### testing 

  Listen to the serial connection with minicom. `ACM0` refers to a custom config file made with minicom.
```  
  $ minicom ACM0      
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
  - see https://github.com/thillRobot/data-logger.git for the continuation of this project
