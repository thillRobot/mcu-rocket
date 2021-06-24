/****************************************************************************************/
/*  data_logger - Tennessee Technological University                                    */
/*  Tristan Hill - 2021                                                                 */
/*  Write sensor data to a csv file on an SD card                                       */    
/*  This example was designed and tested on the MKR1010wifi                             */   
/*  see README.md for version history and hardware information                          */          
/****************************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <SPI.h>
#include <SD.h>

#include <Adafruit_DPS310.h>

//Set the delay between fresh samples 
//#define BNO055_SAMPLERATE_DELAY_MS (100)
#define LOOP_DELAY_MS (100)

const int chipSelect = 7; // 7 used on MKR, not setting this can cause the SD to write to ALMOST work
int entry_number = 0;     // number of the first row in the data file 
int file_number = 1;      // change this number to create a new file
String file_string;       // global variables - should this be done differently?
bool delete_file = true;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)                                  
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // (id, address)

Adafruit_DPS310 dps;
/*************************************************************/
/*  setup function 'setup'                                   */
/*************************************************************/
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("----------------------------------------------------------------");
  Serial.println("Data Logger - MKR1010wifi: Initializing... "); Serial.println("");

  // Initialize the SD card and log file
  initSDCard();

  // Initialize the BNO055 Orientation sensor 
  if(!bno.begin())
  {
    // There was a problem detecting the BNO055 ... check your connections 
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  // Use external crystal for better accuracy 
  bno.setExtCrystalUse(true);

  // Display some basic information on this sensor 
  displayBNO055Details();

  Serial.println("BNO055 Absolute Orientation Sensor: Initialized\r\n");
  
   
  delay(1000);
   
  //Serial.println("DPS310");
  if (! dps.begin_I2C(0x77)) {             // Can pass in I2C address here
  //if (! dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
    Serial.println("Failed to find DPS");
    while (1) yield();
  }
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
  Serial.println("DPS310 Barometric Pressure and Temperature Sensor: Initialized\r\n");
  

  Serial.println("Data Logger - MKR1010wifi: Initialization Complete");
  Serial.println("----------------------------------------------------------------");
}

/*************************************************************/
/*  main function 'loop'                                     */
/*************************************************************/
void loop() {

  // print the entry number and calibration data before printing sensor data
  printHeader();

  printBNO055();

  printDPS310();

  // print a closing line after the sensor data
  printFooter();

  delay(LOOP_DELAY_MS);
  
}

/*************************************************************/
/*  this function 'initFile' opens a file for the datalog     */
/*************************************************************/
void initSDCard(void)
{

  Serial.println("Checking for SD Card and Log File");
  // check if the card is present and can be initialized
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card failed or not present");
    while (1); // wait forever if card fails?
  }
 
  //file_string="datalog"+String(file_number)+".txt";  // global variable for now
  file_string="mkr-"+String(file_number)+".txt";  // global variable for now

  //instantiate a string for assembling the data file header
  String buffer= "Log Filename: "+ file_string;

  // check to see if the file already exists on the SD card
  if (SD.exists(file_string)&&delete_file)
  {
    Serial.println(file_string+" already exist, deleting file before writing data");
    SD.remove(file_string);
  }else if(SD.exists(file_string))
  {
    Serial.println(file_string+" already exist, data will be appended to file");
  }else
  {
    Serial.println(file_string+" does not exist, a new file will be created");
  } 
  //Serial.println(buffer);

  

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
    Serial.println("Error opening datalog file: "+file_string);
  }

  // write the string to the serial output for debugging
  Serial.println(buffer);

  Serial.println("5v Ready SD Card Breakout: Initialized\r\n");
  //Serial.println("----------------------------------------");

}

/*****************************************************************************/
/*  Formats and writes the data entry header to file         */
/*****************************************************************************/
bool printHeader() {

  // instantiate and assemble a string for the data entry header
  String buffer = "Data Entry:"+String(entry_number);

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
bool printFooter(void) {
  
  // instantiate and assemble a string for the data entry footer
  String buffer = "Data Entry:"+String(entry_number)+": Complete";
  
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

/***************************************************************/
/*  Accesses data from BNO055 and prints to entry in the file  */
/***************************************************************/
bool printBNO055(void) {

  // get the BNO055 temp
  int8_t boardTemp = bno.getTemp();
  // get the BNO055 calibration data (notice this way uses pointers?)
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  // append the sensor calibration data to the string
  String buffer = "\r\nBNO055 Calibration: Sys= "+String(system)+", Gyro="+String(gyro)+", Accel="+String(accel)+", Mag="+String(mag)
  +"\r\nBNO055 Temperature:"+String(boardTemp);

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
    Serial.println("Error opening datalog file: "+file_string);
  }

  // instanstiate objects for different sensor types 
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  // print the bulk of the data to the file
  printBNO055Event(&orientationData);
  printBNO055Event(&angVelocityData);
  printBNO055Event(&linearAccelData);
  printBNO055Event(&magnetometerData);
  printBNO055Event(&accelerometerData);
  printBNO055Event(&gravityData);

  return true;
}


/*****************************************************************************/
/*  Formats and writes a single sensor event to file                         */
/*****************************************************************************/
void printBNO055Event(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //easy to spot dummy values

  // instantiate a string for assembling the data log
  String buffer;

  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    buffer += "BNO055 Accelerometer:";
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
    buffer += "BNO055 Orientation:";
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
    buffer += "BNO055 Magnetic Field:";
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    buffer += "BNO055 Gyroscope:";
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    buffer += "BNO055 Rotation Vector:";
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    buffer += "BNO055 Linear Acceleration:";
  }
  else {
    //Serial.print("BNO055 Unknown Data:");
    buffer += "BNO055 Unknown Data:";
  }
  
  // append the data feild to the string separated by commas
  buffer += String(x)+","+String(y)+","+String(z);

  /// open the file and instanstiate a file identifier object
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

  // write the string to the serial output for debugging
  Serial.println(buffer);

}

/**************************************************************************/
/*    Displays some basic information on this sensor from the unified     */
/*    sensor API sensor_t type (see Adafruit_Sensor for more information) */   
/**************************************************************************/
void displayBNO055Details(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  //Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  //Serial.println("------------------------------------");
  //Serial.println("");
  delay(500);
}

/***************************************************************/
/*  Accesses data from DPS310 and prints to entry in the file  */
/***************************************************************/
bool printDPS310(void) {
  
  // instanstiate objects for different sensor types 
  sensors_event_t temp_event, pressure_event;
  
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    //return; // wait until there's something to read
  }

  // print the bulk of the data to the file
  dps.getEvents(&temp_event, &pressure_event);

  String buffer ="DPS310 Temperature:"+String(temp_event.temperature)+"\r\nDPS310 Pressure:"+String(temp_event.pressure);

  Serial.println(buffer);

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
    Serial.println("Error opening datalog file: "+file_string);
  }

  return true;
}
