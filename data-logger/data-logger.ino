/****************************************************************************************/
/*  data_logger_nano - Tennessee Technological University                               */
/*  Tristan Hill - 2021                                                                 */
/*  Write sensor data from BNO055 to a csv file on an SD card                           */    
/*  this is example was reduced so that it could run on a NANO328                       */                         
/*  see README.md for version history and hardware information                          */          
/****************************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <SD.h>

#define LOOP_DELAY_MS 100 // main loop() delay

const int chip_select = 10; // 10 for nano

int entry_number = 0;     // number of the first row in the data file 
int file_number = 1;      // change this number to create a new file
String file_string;       // global variables - should this be done differently?
bool delete_file = false;  // true: create new file, false: append existing file

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

  printHeader();
  
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

  file_string="nano-"+String(file_number)+".txt";  // global variable for now

  // check to see if the file already exists on the SD card
  if (SD.exists(file_string)&&delete_file)
  {
    //Serial.println("Debug 1");
    //Serial.println(file_string+" already exist, deleting file before writing data");
    SD.remove(file_string);
  }else if(SD.exists(file_string))
  {
    //Serial.println("Debug 2");
    //Serial.println(file_string+" already exist, data will be appended to file");
  }else
  {
    //Serial.println("Debug 3");
    //Serial.println(file_string+" does not exist, a new file will be created");
  } 
  //Serial.println(buffer);
  

  //instantiate a string for assembling the data file header
  String buffer= "Data Log Filename: "+ file_string + "\r\n";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File file_id = SD.open(file_string, FILE_WRITE);

  // if the file is available, write the string to it:
  if (file_id) {
    file_id.println(buffer);
    file_id.close(); //close the file before opening another.
  }
  // if the file did not open, change the header message to an error:
  else {
    Serial.println("Error Opening File: "+file_string);
  }

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
  sensors_event_t orientationData , angVelocityData, linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  //bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  //bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  //bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  // print the bulk of the data to the file
  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  //printEvent(&magnetometerData);
  //printEvent(&accelerometerData);
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
  int8_t boardTemp = bno.getTemp();
  // get the BNO055 calibration data (notice this way uses pointers?)
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  // instantiate and assemble a string for the data entry header
  String buffer = "Data Log Entry:"+String(entry_number)+"\r\nBNO055 Temp:"+String(boardTemp)
    +"\r\nCalibration[Sys,Gyro,Accel,Mag]:"+String(system)+","+String(gyro)+","+String(accel)+","+String(mag);

  // open the file and instanstiate a file identifier object
  File file_id = SD.open(file_string, FILE_WRITE);

  // if the file is available, write to it:
  if (file_id) {
    file_id.println(buffer);
    file_id.close(); //close the file before opening another.
  }
  // if the file did not open, change the header message to an error:
  else {
    Serial.println("Error opening datalog file: "+file_string);
  }

  // write the string to the serial output for debugging
  Serial.println(buffer);
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
  float x = -1000000, y = -1000000 , z = -1000000; //easy to spot dummy values

  // instantiate a string for assembling the data log
  String buffer;

  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    buffer = "Accelerometer:"+String(x)+","+String(y)+","+String(z);
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
   buffer += "Orientation:"+String(x)+","+String(y)+","+String(z);
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
    buffer += "MagneticField:"+String(x)+","+String(y)+","+String(z);
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    buffer += "SENSOR_TYPE_GYROSCOPE:"+String(x)+","+String(y)+","+String(z);
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    buffer += "RotationVector:"+String(x)+","+String(y)+","+String(z);
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    buffer += "LinearAcceleration:"+String(x)+","+String(y)+","+String(z);
  }
  else {
    buffer += "Unknown:"+String(x)+","+String(y)+","+String(z);
  }
  
  // append the data feild to the string separated by commas
  //buffer += String(x)+","+String(y)+","+String(z);

  /// open the file and instanstiate a file identifier object
  File file_id = SD.open(file_string, FILE_WRITE);

  // if the file is available, write the string to it:
  if (file_id) {
    file_id.println(buffer);
    file_id.close(); //close the file before opening another
  }
  // if the file did not open, change the message to an error
  else {
    Serial.println("Error opening file: "+file_string);
  }

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