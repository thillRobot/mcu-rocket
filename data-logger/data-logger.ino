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

#define M_BYTES 15
#define N_BYTES 60


const int chip_select = 10; // 10 for nano

int entry_number = 0;     // number of the first row in the data file 
int file_number = 4;      // change this number to create a new file
bool delete_file = false;  // true: create new file, false: append existing file

// String file_string;       // global variables - should this be done differently? // commented for debug

//int M=15;
//int N=50;

char file_string[M_BYTES];


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
    Serial.print("BNO055 not detected, Check wiring or I2C address.");
    while(1);
  }
  //Serial.println("Debug 6");
  delay(1000);

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

  //Serial.println("Checking for SD Card...");
  // check if the card is present and can be initialized
  if (!SD.begin(chip_select)) {
    Serial.println("SD card failed or not present");
    while (1); // wait forever if card fails?
  }
  //Serial.println("SD card initialized");

 
  snprintf(file_string,M_BYTES,"nano-%d.txt",file_number);

  // check to see if the file already exists on the SD card
  if (SD.exists(file_string)&&delete_file)
  {
    Serial.println("file already exist, deleting file before writing data");
    SD.remove(file_string);
  }else if(SD.exists(file_string))
  {
    Serial.println("file already exist, data will be appended to file"); 
  }else
  {
    Serial.println("file does not exist, a new file will be created");
  } 

  char buffer[N_BYTES];
  snprintf(buffer,M_BYTES,"Datalog Filename: %s",file_string);
  
  // Open the file and remember that you have to close it before it can be opened again (or before opening another?).
  File file_id = SD.open(file_string, FILE_WRITE);
  //delay(1000);
  if (file_id) {   // if the file is available, generate a first line and write it
    file_id.println(buffer);
    //file_id.close(); //close the file before opening another.
  }
  else{ 
    snprintf(buffer,N_BYTES,"Error Opening File: %s",file_string);  // else the file did not open, change the header message to show error
    //file_id.close();
  }
  file_id.close();
  // write the string to the serial output for debugging
  Serial.println(buffer);

}

/**********************************************************/
/*  Formats and writes the data entry to the file         */
/**********************************************************/

bool printData(void) {
  
  // print the entry number and calibration data before printing sensor data
  // printHeader();

  // instanstiate objects for different sensor types 
  sensors_event_t accelerometerData, orientationData, linearAccelData;// angVelocityData, magnetometerData, gravityData;
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  
  //bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  //bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  //bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  // print the bulk of the data to the file
  printEvent(&accelerometerData);
  printEvent(&orientationData);
  printEvent(&linearAccelData);
  
  //printEvent(&angVelocityData);
  //printEvent(&magnetometerData); 
  //printEvent(&gravityData);

  // print a closing line after the sensor data
  // printFooter();
  entry_number++;
  return true;
}


/*****************************************************************************/
/*  Formats and writes the data entry header and calibration to file         */
/*****************************************************************************/

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

/*****************************************************************************/
/*  Formats and writes a single sensor event to file                         */
/*****************************************************************************/
void printEvent(sensors_event_t* event) {
  //float x = -1000000, y = -1000000 , z = -1000000; //easy to spot dummy values
 
  //String topic, data; // the name of the sensor, or name of the data to follow
  char data_x[M_BYTES], data_y[M_BYTES], data_z[M_BYTES]; // if these c strings fill up the program will crash and reset the board
  //char data[100];
  char buffer[N_BYTES];

  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    //x = event->acceleration.x;
    //y = event->acceleration.y;
    //z = event->acceleration.z;
    //buffer = "Accelerometer:"+String(x)+","+String(y)+","+String(z);
    //topic = "Accelerometer: ";

    /*  // this is an alternate method that does not work, I was trying to use just one temp char array to save memory
    dtostrf(event->acceleration.x,10,4,data); // convert float to C string 
    sprintf(buffer,"%s, ",data);
    dtostrf(event->acceleration.y,10,4,data); // because %f is not supported in Arduino
    sprintf(buffer,"%s, ",data);
    dtostrf(event->acceleration.z,10,4,data); // https://stackoverflow.com/questions/27651012/arduino-sprintf-float-not-formatting
    sprintf(buffer,"%s; ",data);
    */
    
    dtostrf(event->acceleration.x,8,2,data_x); // convert float to C string 
    dtostrf(event->acceleration.y,8,2,data_y);  
    dtostrf(event->acceleration.z,8,2,data_z); 
    snprintf(buffer,N_BYTES,"Accelerometer: %s, %s, %s;",data_x, data_y, data_z); // print formatted values to the buffer
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    //x = event->orientation.x;
    //y = event->orientation.y;
    //z = event->orientation.z;
   //buffer += "Orientation:"+String(x)+","+String(y)+","+String(z);
   //topic = "Orientation: ";
    //sprintf(topic,"Orientation: ");
   //sprintf(buffer,"Orientation: %f,%f,%f",45.0,event->orientation.y,event->orientation.z);
    dtostrf(event->orientation.x,8,2,data_x); // convert float to C string 
    dtostrf(event->orientation.y,8,2,data_y); 
    dtostrf(event->orientation.z,8,2,data_z); 
    snprintf(buffer,N_BYTES,"Orientation: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    //x = event->acceleration.x;
    //y = event->acceleration.y;
    //z = event->acceleration.z;
    //buffer += "LinearAcceleration:"+String(x)+","+String(y)+","+String(z);
    //topic="LinearAcceleration: ";
    //sprintf(topic,"LinearAcceleration: ");
    //sprintf(buffer,"LinearAcceleration: %f, %f, %f",event->acceleration.x,event->acceleration.y,event->acceleration.z);
    dtostrf(event->acceleration.x,8,2,data_x); // convert float to C string 
    dtostrf(event->acceleration.y,8,2,data_y); 
    dtostrf(event->acceleration.z,8,2,data_z); 
    snprintf(buffer,N_BYTES,"LinearAcceleration: %s, %s, %s;",data_x, data_y, data_z);
  }/*
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    //x = event->gyro.x;
    //y = event->gyro.y;
    //z = event->gyro.z;
    //buffer += "Gyroscope:"+String(x)+","+String(y)+","+String(z);
    //topic="Gyroscope: ";
    //sprintf(topic,"Gyroscope: ");
    //sprintf(buffer,"Gyroscope: %f,%f,%f",event->gyro.x,event->gyro.y,event->gyro.z);
    dtostrf(event->gyro.x,8,2,data_x); // convert float to C string 
    dtostrf(event->gyro.y,8,2,data_y); 
    dtostrf(event->gyro.z,8,2,data_z); 
    sprintf(buffer,"AngularVelocity: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    //x = event->magnetic.x;
    //y = event->magnetic.y;
    //z = event->magnetic.z;
    //buffer += "MagneticField:"+String(x)+","+String(y)+","+String(z);
    //topic="MagneticField: ";
    //sprintf(topic,"MagneticField: ");
    //sprintf(buffer,"MagneticField: %f,%f,%f",event->magnetic.x,event->magnetic.y,event->magnetic.z);
    dtostrf(event->magnetic.x,8,2,data_x); // convert float to C string 
    dtostrf(event->magnetic.y,8,2,data_y); 
    dtostrf(event->magnetic.z,8,2,data_z); 
    sprintf(buffer,"MagneticField: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    //x = event->gyro.x;
    //y = event->gyro.y;
    //z = event->gyro.z;
    //buffer += "Gyroscope:"+String(x)+","+String(y)+","+String(z);
    //topic="Gyroscope: ";
    //sprintf(topic,"Gyroscope: ");
    //sprintf(buffer,"Gyroscope: %f,%f,%f",event->gyro.x,event->gyro.y,event->gyro.z);
    dtostrf(event->gyro.x,8,2,data_x); // convert float to C string 
    dtostrf(event->gyro.y,8,2,data_y); 
    dtostrf(event->gyro.z,8,2,data_z); 
    sprintf(buffer,"MagneticField: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    //x = event->gyro.x; // this looks like is might be a typo/bug -> look into this soon!
    //y = event->gyro.y; // should this be gyro?
    //z = event->gyro.z;
    //buffer += "RotationVector:"+String(x)+","+String(y)+","+String(z);
    //topic = "RotationVector: ";
    //sprintf(topic,"RotationVector: ");
    //sprintf(buffer,"RotationVector: %f,%f,%f",event->gyro.x,event->gyro.y,event->gyro.z);
    dtostrf(event->gyro.x,8,2,data_x); // convert float to C string 
    dtostrf(event->gyro.y,8,2,data_y); 
    dtostrf(event->gyro.z,8,2,data_z); 
    sprintf(buffer,"RotationVector: %s, %s, %s;",data_x, data_y, data_z);
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    //x = event->acceleration.x;
    //y = event->acceleration.y;
    //z = event->acceleration.z;
    //buffer += "LinearAcceleration:"+String(x)+","+String(y)+","+String(z);
    //topic="LinearAcceleration: ";
    //sprintf(topic,"LinearAcceleration: ");
    //sprintf(buffer,"LinearAcceleration: %f, %f, %f",event->acceleration.x,event->acceleration.y,event->acceleration.z);
    dtostrf(event->acceleration.x,8,2,data_x); // convert float to C string 
    dtostrf(event->acceleration.y,8,2,data_y); 
    dtostrf(event->acceleration.z,8,2,data_z); 
    sprintf(buffer,"LinearAcceleration: %s, %s, %s;",data_x, data_y, data_z);
  }
  else {
    //buffer += "Unknown:"+String(x)+","+String(y)+","+String(z);
    //topic="Unknown: ";
    sprintf(buffer,"Unknown: ");
  }*/


  // Re-open the file and remember that you have to close it before it can be opened again (or before opening another?).
  File file_id = SD.open(file_string, FILE_WRITE);

  if (file_id) {   // if the file is available, generate a first line and write it
    file_id.println(buffer);
    //file_id.close(); //close the file before opening another.
  }
  else{ 
    snprintf(buffer,N_BYTES,"Error Opening: %s",file_string);  // else the file did not open, change the header message to show error
    //file_id.close();
  } 
  file_id.close();

  // write the string to the serial output for debugging
  Serial.println(buffer);

}

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