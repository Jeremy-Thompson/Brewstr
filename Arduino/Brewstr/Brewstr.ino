#include <OneWire.h>
#include <DallasTemperature.h>

// variables to store brew configuration/setpoints
int mash_temp_sp;
int mash_time_sp;
int boiler_temp_sp;
int boiler_time_sp;
int hops_time_sp;

// variables to store sensor/system states
int boiler_temp_fb;
int boiler_temp_fb_memory; //stores previous boiler temp value

int ledpin = 12; // LED connected to pin 48 (on-board LED)

// Defining the scheduling interval for each function
#define readMessageFromUL_Cycle 10000U
#define readTemp_Cycle 2500U
#define controlTemp_Cycle 1250U
#define taskCycle 100000U

// Define the temperature sensor data pin = 2
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define variables to store last run time (in ms) for each function (used by task scheduler).
unsigned long readMessageFromUL_LastMillis = 0;
unsigned long readTemp_LastMillis = 0;
unsigned long controlTemp_LastMillis = 0;
unsigned long taskLastMillis = 0;

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10

void setup()
{
  Serial.begin(9600); //set baud rate for bluetooth serial input port
  // Start up the library
  sensors.begin();

  //testing setup
  pinMode(13, OUTPUT);
}

//----------------------------------------------
// Main Running loop that calls the individual functions via task scheduler function (cycleCheck)
//----------------------------------------------
void loop()
{
  if(taskScheduler(&readMessageFromUL_LastMillis, readMessageFromUL_Cycle))  // if readpacket is currently scheduled to be called
  {
     //run task 1
    Serial.println("readPacket: ");
    readMessageFromUL();
  }
  if(taskScheduler(&readTemp_LastMillis, readTemp_Cycle)) // if readtemp is currently scheduled to be called
  {
      readTemp();
  }
  if(taskScheduler(&controlTemp_LastMillis, controlTemp_Cycle))    // task schedule template, add new tasks here and update names
  {
    //run task 
    controlTemp();
  }
  if(taskScheduler(&taskLastMillis, taskCycle))    // task schedule template, add new tasks here and update names
  {
    //run task 
  }
}

//--------------------------------------
// Function to accept packets coming from the upper level software via bluetooth port. It reads data from the serial port and passes it to the decode message function.
// Called by the task schedule on a defined interval.
//--------------------------------------
void readMessageFromUL()
{
  String message;
  while(Serial.available())
  {
    //while there is data available on the serial monitor
    message+=char(Serial.read());//store string from serial command
  }
  // check if we now have the incoming packet. If so, decode it.
  if(message != "")
  {
    decodeMessage(message);
  }
  message = "";
}

//--------------------------------------
// Function to decode messages from the upper level software. It is passed a string by the readPacket() function for decoding.
// Called by the ReadPacket() function. Decodes the string and outputs to local variables.
//--------------------------------------
void decodeMessage(String message)
{
   //strings to store the incoming message information
  String Mashtemp;
  String Mashtime;
  String Boiltemp;
  String Boiltime;
  String Hopstime;
  String Messagetype;
  
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
              
              Serial.print("Mash Temp Setpoint: ");
              Serial.println(mash_temp_sp);
              Serial.print("Mash Time Setpoint: ");
              Serial.println(mash_time_sp);
              Serial.print("Boil Temp Setpoint: ");
              Serial.println(boiler_temp_sp);
              Serial.print("Boil Time Setpoint: ");
              Serial.println(boiler_time_sp);
              Serial.print("Hops Time Setpoint: ");
              Serial.println(hops_time_sp);
              Serial.flush();
}

//---------------------------------------------------
// Function to send status/messages to the upper level software (i.e. android app). 
//---------------------------------------------------
void sendMessageToUL(String message)
{
  //send status messages to android app
  Serial.println(message);
}

//---------------------------------------------------
// Function to read the digital temperature from the 1 wire temp sensor.
//---------------------------------------------------
void readTemp()
{
   boiler_temp_fb_memory = boiler_temp_fb;
   sensors.requestTemperatures(); // Send the command to get temperatures
   boiler_temp_fb = sensors.getTempCByIndex(0);
}

//---------------------------------------------------
//
//---------------------------------------------------
boolean taskScheduler(unsigned long *lastMillis, unsigned int cycle) 
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

//--------------------------------------------------
// Function to compare setpoint and feedback of boiler.
//--------------------------------------------------
void controlTemp()
{
  float temp_diff = (boiler_temp_sp - boiler_temp_fb);
  if(temp_diff > 0)
  {
    if(temp_diff > 3)
    {
      //Heating element should be ON as we are quite far from the setpoint
       digitalWrite(13, HIGH); // for now we will just display the control signal for testing
    }
    else
    {
      int dir = boiler_temp_fb - boiler_temp_fb_memory;
      if(dir >= 0.5)
      {
        digitalWrite(13,LOW); //we're close to the setpoint and we're quickly heating so we can pre-emptively turn off
      }
      else
      {
        digitalWrite(14,HIGH); // we're close to the setpoint but not approaching it quickly, so keep heating.
      }
    }
    
  }
  else
  {
    //heating element should be OFF
    digitalWrite(13, LOW); //for now we will just display the control signal for testing
  }
}

