/****************************************************************************************/
/*  data_logger_nano - Tennessee Technological University                               */
/*  Tristan Hill - 2021                                                                 */
/*  Write sensor data from BNO055 to a csv file on an SD card                           */    
/*  this is example was reduced so that it could run on a NANO328                       */
/*  this example is now a part of 'mcu-rocket'                                          */
/*  see README.md for version history and hardware information                          */          
/****************************************************************************************/

// the current 'debug' goal is to reduce the code size so that I can use the full functionality
// of both sensor board on the nano for nano-rocket

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>

#define LOOP_DELAY_MS 100 // main loop() delay

#define M_BYTES 15 // length of file name in bytes
#define N_BYTES 55 // length of file serial buffer in bytes
#define O_BYTES 8 // length of single data (field width) in bytes 
#define P_BYTES 2  // number of decimal places of single data (precision) in bytes

int chip_select = 10;     // cs is pin 10 for nano
int entry_number = 0;     // number of the first row in the data file 
int file_number = 5;      // change this number to create a new file
bool delete_file = false;  // true: create new file, false: append existing file
bool verbose = true; // true: print the rows of data to the serial monitor for debugging, false: turn off print to save memory (SRAM ?)
//bool sd_status = true; // operation status of the sd module and card

// String file_string;       // global variables - should this be done differently? // commented for debug

char file_string[M_BYTES];

//char buffer[N_BYTES]; 

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)                                  
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // (id, address)

/*************************************************************/
/*  setup function 'setup'                                   */
/*************************************************************/
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  initFile();

  //Serial.begin(115200);
  //Serial.println("Data Logger Nano \n Initializing"); Serial.println("");

  /* Initialize the sensor */
  //Serial.println("Debug 4");
  if(!bno.begin())
  {
    //Serial.println("Debug 5");
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("BNO055 Error");
    while(1);
  }
  //Serial.println("Debug 6");
  delay(100);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  //displaySensorDetails();

}

/*************************************************************/
/*  main function 'loop'                                     */
/*************************************************************/
void loop() {

  //printHeader();
  
  printData();

  delay(LOOP_DELAY_MS);
  
}

/*************************************************************/
/*  this function 'initFile' opens a file for the datalog     */
/*************************************************************/
void initFile(void)
{

  char buffer[N_BYTES]; // c string buffer to store a line of data before printing  

  // format a string for the filename
  snprintf(file_string,M_BYTES,"nano-%d.txt",file_number);

  // check if the card is present and can be initialized
  if (!SD.begin(chip_select)) {
    snprintf(buffer,N_BYTES,"Card Error\n");
    //sd_status=0;
  }/*else{
   snprintf(buffer,N_BYTES,"Card Ready\n");
    //sd_status=1;
  }*/
  
  // check to see if the file already exists on the SD card
  if (SD.exists(file_string)&&delete_file)
  {
    snprintf(buffer,N_BYTES,"Deleting: nano-%d.txt\n",file_number);
    SD.remove(file_string);
  }else if(SD.exists(file_string))
  {
    snprintf(buffer,N_BYTES,"Appending: nano-%d.txt\n",file_number); 
  }else
  {
    snprintf(buffer,N_BYTES,"Creating: nano-%d.txt\n",file_number);
  } 

  // open the file and instanstiate a file identifier
  File file_id = SD.open(file_string, FILE_WRITE);

  if (file_id) {                                          // if the file is available
    snprintf(buffer,N_BYTES,"Datalog: %s",file_string);   // format the first line and 
    file_id.println(buffer);                              // write it to the file                            
  }else {
    snprintf(buffer,N_BYTES,"Error Opening: %s",file_string);
  }
  file_id.close(); //close the file before doing anything else (in case of power down we will still have access data file

  if (verbose) Serial.println(buffer);
  
}

/**********************************************************/
/*  Formats and writes the data entry to the file         */
/**********************************************************/
bool printData(void) {
  
  // print the entry number and calibration data before printing sensor data
  // printHeader();

  // instanstiate objects for different sensor types 
  sensors_event_t accelerometerData,  magnometerData, gravFieldData, linAccelData, angVelData, angPosData; 
  
  //bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);  // direct sensor data
  //bno.getEvent(&magnometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&gravFieldData, Adafruit_BNO055::VECTOR_GRAVITY);            // derived quantities
  bno.getEvent(&linAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&angPosData, Adafruit_BNO055::VECTOR_EULER);

  // print the bulk of the data to the file
  //printEvent(&accelerometerData);
  //printEvent(&magnometerData);
  printEvent(&gravFieldData);
  printEvent(&linAccelData);
  printEvent(&angVelData); 
  printEvent(&angPosData);
  
  // print a closing line after the sensor data
  // printFooter();
  entry_number++;
  return true;
}

/*****************************************************************************/
/*  Formats and writes a single sensor event to file                         */
/*****************************************************************************/
void printEvent(sensors_event_t* event) {
 
  char data_x[M_BYTES], data_y[M_BYTES], data_z[M_BYTES]; // if these c strings fill up the program will crash and reset the board
                                                          // for this reason we switched from sprintf() to snprintf()
  //char s_buffer[S_BYTES]; // buffer for serial monitor
  char buffer[N_BYTES];   // buffer to store a line a data before writing to sd card file (or serial monitor) 
  
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    dtostrf(event->acceleration.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->acceleration.y,O_BYTES,P_BYTES,data_y);  
    dtostrf(event->acceleration.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"Accelerometer: %s, %s, %s;",data_x, data_y, data_z); // print formatted values to the buffer
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    dtostrf(event->magnetic.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->magnetic.y,O_BYTES,P_BYTES,data_y); 
    dtostrf(event->magnetic.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"Magnometer: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    dtostrf(event->acceleration.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->acceleration.y,O_BYTES,P_BYTES,data_y); 
    dtostrf(event->acceleration.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"LinearAcceleration: %s, %s, %s;",data_x, data_y, data_z);   
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    dtostrf(event->gyro.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->gyro.y,O_BYTES,P_BYTES,data_y); 
    dtostrf(event->gyro.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"AngularVelocity: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    dtostrf(event->orientation.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->orientation.y,O_BYTES,P_BYTES,data_y); 
    dtostrf(event->orientation.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"AngularPosition: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    dtostrf(event->gyro.x,O_BYTES,P_BYTES,data_x); // convert float to C string 
    dtostrf(event->gyro.y,O_BYTES,P_BYTES,data_y); 
    dtostrf(event->gyro.z,O_BYTES,P_BYTES,data_z); 
    snprintf(buffer,N_BYTES,"RotationVector: %s, %s, %s;",data_x, data_y, data_z);
  }

  //else {
  //  snprintf(buffer,N_BYTES,"Unknown: ");
  // }

  //x = event->acceleration.x; // this is the original way I was building the buffer string, it seems simple 
  //y = event->acceleration.y; // using Strings uses too much memory and/or code space, and I think that the cast to String is most costly
  //z = event->acceleration.z;
  //buffer = "Accelerometer:"+String(x)+","+String(y)+","+String(z); 

  /*  // this is an alternate method that does not work, I was trying to use just one temp char array to save memory
  dtostrf(event->acceleration.x,10,4,data); // convert float to C string 
  sprintf(buffer,"%s, ",data);
  dtostrf(event->acceleration.y,10,4,data); // because %f is not supported in Arduino
  sprintf(buffer,"%s, ",data);
  dtostrf(event->acceleration.z,10,4,data); // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
  sprintf(buffer,"%s; ",data);
  */

  // open the file and instanstiate a file identifier
  File file_id = SD.open(file_string, FILE_WRITE);

  if (file_id) {                                          // if the file is available
    file_id.println(buffer);                              // write it to the file                            
  }else {
    snprintf(buffer,N_BYTES,"Error Re-Opening: %s",file_string);
  }
  file_id.close(); //close the file before doing anything else (in case of power down we will still have access data file

  if (verbose) Serial.println(buffer);
  

}


/*****************************************************************************/
/*  Formats and writes the data entry header and calibration to file         */
/*****************************************************************************/

/*
bool printHeader() {
  // get the BNO055 temp for the data entry header
  //int8_t boardTemp = bno.getTemp();
  // get the BNO055 calibration data (notice this way uses pointers?)
  //uint8_t system, gyro, accel, mag = 0;
  //bno.getCalibration(&system, &gyro, &accel, &mag);

  // instantiate and assemble a string for the data entry header
  //String buffer = "Data Log Entry:"+String(entry_number)+"\r\nBNO055 Temp:"+String(boardTemp) // commented for debug
  //  +"\r\nCalibration[Sys,Gyro,Accel,Mag]:"+String(system)+","+String(gyro)+","+String(accel)+","+String(mag); // commented for debug

  // open the file and instanstiate a file identifier object
  //File file_id = SD.open(file_string, FILE_WRITE);  // replaced with line below for debug
  //File file_id = SD.open("debug_filename.txt", FILE_WRITE);

  // if the file is available, write to it:
  //if (file_id) {
    //file_id.println(buffer); // commented out for debug
    //file_id.println("debug_buffer_print");
    //file_id.close(); //close the file before opening another.
  //}
  // if the file did not open, change the header message to an error:
  //else {
    //Serial.println("Error opening datalog file: "+file_string);
    //Serial.println("debug_serial_print");  
  //}

  // write the string to the serial output for debugging
  //Serial.println(buffer);
  //Serial.println("debug_buffer_print");
  return true;
}
*/

/**************************************************************************/
/*  Formats and writes the data entry footer to file                      */
/**************************************************************************/
/*
bool printFooter(void) {
  
  // instantiate and assemble a string for the data entry footer
  String buffer = "DataLog Entry:"+String(entry_number)+": Complete";
  
  // open the file and instanstiate a file identifier object
  File file_id = SD.open(file_string, FILE_WRITE);
  // if the file is available, write the string to it:
  if (file_id) {
    file_id.println(buffer);
    file_id.close(); //close the file before opening another
  }
  // if the file did not open, change the message to an error
  else {
    Serial.println("Error opening datalog file: "+file_string);
  }
  entry_number++;
  // write the string to the serial output for debugging
  Serial.println(buffer);
  return true;
}
*/

/**************************************************************************/
/*    Displays some basic information on this sensor from the unified     */
/*    sensor API sensor_t type (see Adafruit_Sensor for more information) */   
/**************************************************************************/
/*
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
*/