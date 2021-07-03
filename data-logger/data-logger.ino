//--------------------------------------------------------------------------------------//
//  data_logger_nano - Tennessee Technological University                               //
//  Tristan Hill - 2021                                                                 //
//  Write sensor data from BNO055 to a csv file on an SD card                           //    
//  this is example was reduced so that it could run on a NANO328                       //
//  this example is now a part of 'mcu-rocket'                                          //
//  see README.md for version history and hardware information                          //          
//--------------------------------------------------------------------------------------//

// the current 'debug' goal is to reduce the code size so that I can use the full functionality
// of both sensor board on the nano for nano-rocket

#include <Wire.h>
//#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_DPS310.h>
//#include <utility/imumaths.h>
//#include <SPI.h>
#include <SD.h>
//#include <stdio.h>
//#include <string.h>

#define M_BYTES 12 // length of file name in bytes
#define N_BYTES 40 // length of file serial buffer in bytes
#define O_BYTES 7 // length of single data (field width) in bytes 
#define P_BYTES 2  // number of decimal places of single data (precision) in bytes
#define Q_BYTES 15 //

#define clock_factor 0.004096 // 1.0/16000000.0*65536 - 1/16Mhz Clock * 16 Bit Counter

//volatile uint16_t ofcnt=0;
float time_sec=0;
int prescale;

int chip_select = 10;     // cs is pin 10 for nano
int entry_number = 0;     // number of the first row in the data file 
bool delete_file = false;  // true: create new file, false: append existing file
bool verbose = true; // true: print the rows of data to the serial monitor for debugging, false: turn off print to save memory (SRAM ?)

char file_string[M_BYTES]="nano-0.txt"; // hardcode the filename instead of formatting to save resources, just as easy...
char buffer[N_BYTES];  // define locally instead because it reduced the required program storage space ???

// instantiate object for BNO055 sensor board (by default address is 0x29 or 0x28)                                  
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // (id, address)

// instantiate object(s) for DPS310 sensor board 
Adafruit_DPS310 dps;

//-------------------------------------------------------------//
//   initialization function 'setup' runs once before 'loop'   //
//-------------------------------------------------------------//
void setup() {

  //char buffer[N_BYTES];

  // Initialize the serial monitor for debugging
  Serial.begin(38400);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only
 
  // Initialize ISR Timer 1 for timestamps and scheduling
  TCCR1A  = 0b00000000; 
  
  prescale=1;
  TCCR1B  = 0b00000001; // timer prescale = 1 (no prescale)       
  //TCCR1B | (1<<CS10);  //CS12:0=001 -> prescale = 1 -> 244.5 Hz // you could also set the bit individually as shown below         
  //TCCR1B &= ~(1<<CS11) & ~(1<<CS12);                  
  
  //prescale=8; // slow the timer down by a factor of 8, I am not sure that there are any benfits to this. just testing
  //TCCR1B | (1<<CS11) ; //CS12:0=010 -> prescale = 8 -> 30.5 Hz       
  //TCCR1B &= ~(1<<CS10) & ~(1<<CS12); 
  
  TIMSK1  = 0b00000001; // overflow interrupt enable 
  
  initFile();

  initSensors();

}

//--------------------------------------------------------//
//  main function 'loop' run continously after 'setup'    //
//--------------------------------------------------------//
void loop() {

  // write one entry of sensor data to the SD card  
  while (!(TIFR1&0b00000001)); //polling overflow to wait on timer
  
  // print the entry number and calibration data before printing sensor data
  printHeader();

  printBNO055();

  printDPS310();

  // print a closing line after the sensor data
  // printFooter(); // To save resources we do not need a footer. The next header will do just as much good
  
}

//--------------------------------------------------------------------------// 
//   interrupt suboutine function 'TIMER1_OVF_vect' for Timer1 overflow     //
//--------------------------------------------------------------------------// 
ISR(TIMER1_OVF_vect) {
    // compute the running time in seconds using the system clock and prescale
    // this timer code was written and tested on the mega2560 but it matches on a quick 20s stopwatch test on the 328p
    time_sec=time_sec+clock_factor*prescale; 
    //ofcnt++; // count overflow events if neccesary - disabled for now
} 

//----------------------------------------------------------------//
//    subroutine function 'initFile' prepares file on SD card     //  
//----------------------------------------------------------------//
void initFile(void)
{

  memset(buffer, 0,sizeof buffer); // clear the char array buffer
  //char buffer[N_BYTES]; // c string buffer to store a line of data before printing // defined globally for now, revisit this soon

  // check if the card is present and can be initialized
  if (SD.begin(chip_select)) {
    strncpy(buffer,"Card Ready\n",N_BYTES);
  }else{
    strncpy(buffer,"Card Error\r\n",N_BYTES);  
  }
  
  // check to see if the file already exists on the SD card
  if (SD.exists(file_string)&&delete_file){
    strncpy(buffer,"Overwriting:",N_BYTES);
    SD.remove(file_string);
  }else if(SD.exists(file_string)) {
    strncpy(buffer,"Appending:",N_BYTES);
  }else {
    strncpy(buffer,"Creating:",N_BYTES);
  } 

  // open the file and instanstiate a file identifier
  File file_id = SD.open(file_string, FILE_WRITE);

  strncat(buffer,file_string,N_BYTES); // append the filename to the message

  writeSDCard();

}

//-------------------------------------------------------------------//
//   subroutine function 'initSensors' prepares the sensor boards    //
//-------------------------------------------------------------------//
void initSensors(void)
{

  memset(buffer, 0,sizeof buffer); // clear the char array buffer
  //char buffer[N_BYTES];

  // Initialize the DPS310 Barometric Pressure and Temperature Sensor
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  if(dps.begin_I2C())
  {
    strncpy(buffer,"DPS310 Ready\r\n",N_BYTES); // There was a problem detecting the BNO055 -> check the connections 
  }else{
    strncpy(buffer,"DPS310 Error\r\n",N_BYTES); // There was a problem detecting the BNO055 -> check the connections  
  } 
  
  // Initialize the BNO055 Absolute Orientation Board 
  if(bno.begin())
  {
    strncat(buffer,"BNO055 Ready\r\n",N_BYTES); // There was a problem detecting the BNO055 -> check the connections 
  }else{
    strncat(buffer,"BNO055 Error\r\n",N_BYTES); // There was a problem detecting the BNO055 -> check the connections  
  } 

  delay(100);

  // Use external crystal for better accuracy 
  bno.setExtCrystalUse(true);
   
  writeSDCard();

}

//--------------------------------------------------------------------------------------------------------------//
//   subroutine function 'printBNO055' calls 'bno.getEvent' and 'printBNO055Event' for each information type    //
//--------------------------------------------------------------------------------------------------------------//
bool printBNO055(void) {
  
  // instanstiate objects for different sensor types 
  sensors_event_t accelerometerData,  magnometerData;
  sensors_event_t gravFieldData, linAccelData, angVelData, angPosData; 
  
  //bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);  // direct sensor data
  //bno.getEvent(&magnometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&gravFieldData, Adafruit_BNO055::VECTOR_GRAVITY);            // derived quantities
  bno.getEvent(&linAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&angPosData, Adafruit_BNO055::VECTOR_EULER);

  // print the bulk of the data to the file
  //printBNO055Event(&accelerometerData);
  //printBNO055Event(&magnometerData);
  printBNO055Event(&gravFieldData);
  printBNO055Event(&linAccelData);
  printBNO055Event(&angVelData); 
  printBNO055Event(&angPosData);
    
  return true;
}

//---------------------------------------------------------------------------------------------------------//
//   subroutine 'printEvent' formats a single sensor event and calls 'writeSDCard' to write the buffer     //
//---------------------------------------------------------------------------------------------------------//
void printBNO055Event(sensors_event_t* event) {

  memset(buffer, 0,sizeof buffer); // clear the char array buffer
  //char buffer[N_BYTES]; // defined globally for now, revisit this soon

  char str_x[M_BYTES], str_y[M_BYTES], str_z[M_BYTES]; // if these c strings fill up the program will crash and reset the board
                                                          // for this reason we switched from sprintf() to snprintf()

  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    dtostrf(event->acceleration.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->acceleration.y,O_BYTES,P_BYTES,str_y);  
    dtostrf(event->acceleration.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"Accel:",N_BYTES);
    //strncpy(buffer,accel,N_BYTES);
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    dtostrf(event->magnetic.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->magnetic.y,O_BYTES,P_BYTES,str_y); 
    dtostrf(event->magnetic.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"Mag:",N_BYTES);
    //strncpy(buffer,mag,N_BYTES);
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    dtostrf(event->acceleration.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->acceleration.y,O_BYTES,P_BYTES,str_y); 
    dtostrf(event->acceleration.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"LinAccel:",N_BYTES);
    //strncpy(buffer,linaccel,N_BYTES);
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    dtostrf(event->gyro.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->gyro.y,O_BYTES,P_BYTES,str_y); 
    dtostrf(event->gyro.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"AngVel:",N_BYTES);
    //strncpy(buffer,angvel,N_BYTES);
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    dtostrf(event->orientation.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->orientation.y,O_BYTES,P_BYTES,str_y); 
    dtostrf(event->orientation.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"AngPos:",N_BYTES);
    //strncpy(buffer,angpos,N_BYTES);
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    dtostrf(event->gyro.x,O_BYTES,P_BYTES,str_x); // convert float to C string 
    dtostrf(event->gyro.y,O_BYTES,P_BYTES,str_y); 
    dtostrf(event->gyro.z,O_BYTES,P_BYTES,str_z); 
    strncpy(buffer,"RotVec:",N_BYTES);
    //strncpy(buffer,rotvec,N_BYTES);
  }
  strncat(buffer,str_x,N_BYTES);
  strncat(buffer,",",N_BYTES);
  strncat(buffer,str_y,N_BYTES);
  strncat(buffer,",",N_BYTES);
  strncat(buffer,str_z,N_BYTES);
  strncat(buffer,";",N_BYTES);

  //snprintf(tmp,N_BYTES,"%s,%s,%s;",str_x, str_y, str_z); // print formatted values to the buffer
  //strncat(buffer,tmp,N_BYTES);
  
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

  writeSDCard();

}

//-----------------------------------------------------------------------------------------------------------//
//    subroutine function 'printDPS310' formats sensor data and calls 'writeSDCard' to write the buffer     //
//-----------------------------------------------------------------------------------------------------------//
bool printDPS310(){

  memset(buffer, 0,sizeof buffer);
  //char buffer[N_BYTES]; // defined globally for now, revisit this soon

  Adafruit_Sensor *dps_temp = dps.getTemperatureSensor();
  Adafruit_Sensor *dps_pressure = dps.getPressureSensor();

  sensors_event_t temp_event, pres_event;
  char str[M_BYTES];//,str_y[M_BYTES]; // string for float conversion
  float altitude; 

  // wait for the temp reading to be available
  while(!dps.temperatureAvailable());
  //if (dps.temperatureAvailable()) { // the example code used ifs, but I changed them to waiting whiles
  dps_temp->getEvent(&temp_event);
  dtostrf(temp_event.temperature,O_BYTES,P_BYTES,str);
  strncpy(buffer,"Temp:",N_BYTES);
  strncat(buffer,str,N_BYTES);
  strncat(buffer,"\r\n",N_BYTES);
  //}
  
  // reading pressure also reads temp so don't check pressure before temp - I am not sure what this means exactly 
  while(!dps.pressureAvailable());  
  //if (dps.pressureAvailable()) {
  dps_pressure->getEvent(&pres_event);
  dtostrf(pres_event.pressure,O_BYTES,P_BYTES,str);
  strncat(buffer,"Pres:",N_BYTES);
  strncat(buffer,str,N_BYTES);
  strncat(buffer,"\r\n",N_BYTES);
  //}

  // after reading temp and pressure, get the calculated altitude value 
  altitude=dps.readAltitude(1013.25);
  dtostrf(altitude,O_BYTES,P_BYTES,str);
  strncat(buffer,"Alt:",N_BYTES);
  strncat(buffer,str,N_BYTES);
  //strncat(buffer,"\r\n",N_BYTES);

  writeSDCard();

  return true;

}

//-------------------------------------------------------------------------------------------------//
//   subroutine 'printHeader' formats and writes the data entry header and calibration to file     //
//-------------------------------------------------------------------------------------------------//
bool printHeader() {
  // get the BNO055 temp for the data entry header
  // int8_t boardTemp = bno.getTemp();
  // get the BNO055 calibration data (notice this way uses pointers?)
  // uint8_t system, gyro, accel, mag = 0;
  // bno.getCalibration(&system, &gyro, &accel, &mag);

  memset(buffer, 0,sizeof buffer);
  //char buffer[N_BYTES]; // defined globally for now, revisit this soon

  char str[10];
  //dtostrf(entry_number,O_BYTES,P_BYTES,str);  // only needed for floats
  itoa(entry_number,str,O_BYTES);
  strncat(buffer,"Time:",N_BYTES);
  strncat(buffer,str,N_BYTES);
  strncat(buffer,",",N_BYTES);

  dtostrf(time_sec,O_BYTES,P_BYTES,str);  // only needed for floats
  strncat(buffer,str,N_BYTES);
  strncat(buffer,";",N_BYTES);
  
  writeSDCard();

  entry_number++; // increment the entry number 
  return true;
}

//-----------------------------------------------------------------------------------------------------------//
//    subroutine function 'writeSDCard' writes the buffer with formatted data to the file on the SD card     //
//-----------------------------------------------------------------------------------------------------------//
bool writeSDCard(void){

  //char file_err[15]="File Error\r\n";

  // open the file and instanstiate a file identifier
  File file_id = SD.open(file_string, FILE_WRITE);
  
  if (file_id) {                                          // if the file is available
    file_id.println(buffer);                              // write it to the file                            
  }else {
    strncpy(buffer,"File Error\r\n",N_BYTES);                     // else write an error msg to the buffer    
  }
  file_id.close(); //close the file before doing anything else (in case of power down we will still have access data file

  if (verbose) Serial.println(buffer);

  return true;

}


// to save resources we do not need a footer. The next header will do just as much good.  
//-----------------------------------------------------------------------------------//
//  subroutine 'printFooter' formats and writes the data entry footer to the file    //
//-----------------------------------------------------------------------------------// 
/*
bool printFooter(void) {
  
  // char buffer[N_BYTES]; // defined globally for now, revisit this soon

  strncpy(buffer,N_BYTES,"Entry Complete;");

  // open the file and instanstiate a file identifier
  File file_id = SD.open(file_string, FILE_WRITE);

  if (file_id) {                                          // if the file is available
    file_id.println(buffer);                              // write it to the file                            
  }else {
    strncpy(buffer,"File Error: ",N_BYTES);
    strncat(buffer,file_string,N_BYTES);
  }
  file_id.close(); //close the file before doing anything else (in case of power down we will still have access data file

  if (verbose) Serial.println(buffer);
  return true;
}
*/

/*
//----------------------------------------------------------------------------//
//    Displays some basic information on this sensor from the unified         //
//    sensor API sensor_t type (see Adafruit_Sensor for more information)     //   
//----------------------------------------------------------------------------//
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