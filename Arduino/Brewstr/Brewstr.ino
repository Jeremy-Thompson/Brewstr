#include <OneWire.h>
#include <DallasTemperature.h>

String message; //string that stores the incoming message
String Mashtemp;
String Mashtime;
String Boiltemp;
String Boiltime;
String Hopstime;
String Messagetype;
int mash_temp_sp;
int mash_time_sp;
int boiler_temp_sp;
int boiler_temp;
int boiler_time_sp;
int hops_time_sp;

int ledpin = 12; // LED connected to pin 48 (on-board LED)

// Defining the scheduling interval for each function
#define readPacket_Cycle 10000U
#define readTemp_Cycle 2500U
#define taskCycle 2725U

// Define the temperature sensor data pin = 2
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define variables to store last run time (in ms) for each function.
unsigned long readPacket_LastMillis = 0;
unsigned long readTemp_LastMillis = 0;
unsigned long taskLastMillis = 0;

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10

void setup()
{
  Serial.begin(9600); //set baud rate for bluetooth serial input port
  // Start up the library
  sensors.begin();
}

//----------------------------------------------
// Main Running loop that calls the individual functions via task scheduler function (cycleCheck)
//----------------------------------------------
void loop()
{
  if(cycleCheck(&readPacket_LastMillis, readPacket_Cycle))  // if readpacket is currently scheduled to be called
  {
     //run task 1
    Serial.println("readPacket: ");
    readPacket();
  }
  if(cycleCheck(&readTemp_LastMillis, readTemp_Cycle)) // if readtemp is currently scheduled to be called
  {
    boiler_temp = readTemp();
    Serial.println("readTemp: ");
    Serial.println(boiler_temp);
  }
  if(cycleCheck(&taskLastMillis, taskCycle))    // task schedule template, add new tasks here and update names
  {
    //run task 
  }
}


//--------------------------------------
// Function to accept packets coming from the upper level software via bluetooth port. It reads data from the serial port and passes it to the decode message function.
// Called by the task schedule on a defined interval.
//--------------------------------------
void readPacket()
{
  while(Serial.available())
  {
    //while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  // check if we now have the incoming packet. If so, decode it.
  if(message != "")
  {
    decodeMessage();
  }
  message = "";
}

//--------------------------------------
// Function to decode messages from the upper level software. It is passed a string by the readPacket() function for decoding.
// Called by the ReadPacket() function. Decodes the string and outputs to local variables.
//--------------------------------------
void decodeMessage()
{
  // message structure looks like: "boiltemp,boiltime,hopstime"
  int count = 0;
  int length = message.length();
  
  for(int i = 0;i<length;i++)
  {
    while(message.charAt(i) != ',' && i < length)
    {
                      switch(count)
                      {
                        case 0:
                              Messagetype += message.charAt(i);
                              break;
                        case 1:
                              Mashtemp += message.charAt(i);
                        break;
                        case 2:
                              Mashtime += message.charAt(i);
                        break;
                        case 3:
                              Boiltemp += message.charAt(i);
                        break;
                        case 4:
                              Boiltime += message.charAt(i);
                              break;
                        case 5:
                              Hopstime += message.charAt(i);
                        default:
                        break;
                      }
      i+=1;
    }
    count += 1;
  }
  Serial.println(message);
  //read the values into integer storage for the control system.
  Serial.println("Recieved Brew Configuration:");
  //convert all strings into integers
  mash_temp_sp = Mashtemp.toInt();
  mash_time_sp = Mashtime.toInt();
  boiler_temp_sp = Boiltemp.toInt();
  boiler_time_sp = Boiltime.toInt();
  hops_time_sp = Hopstime.toInt(); 
  
  Serial.print("Mash Temp: ");
  Serial.println(mash_temp_sp);
  Serial.print("Mash Time: ");
  Serial.println(mash_time_sp);
  Serial.print("Boil Temp: ");
  Serial.println(boiler_temp_sp);
  Serial.print("Boil Time: ");
  Serial.println(boiler_time_sp);
  Serial.print("Hops Time: ");
  Serial.println(hops_time_sp);
  Serial.flush();
}
void sendStatusToUL()
{
  //send status messages to android app
}
int readTemp()
{
   sensors.requestTemperatures(); // Send the command to get temperatures
   return sensors.getTempCByIndex(0);
}
boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle) 
{
 unsigned long currentMillis = millis();
 if(currentMillis - *lastMillis >= cycle)
 {
   *lastMillis = currentMillis;
   return true;
 }
 else
   return false;
}

