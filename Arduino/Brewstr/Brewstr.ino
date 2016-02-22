#include <OneWire.h>
#include <DallasTemperature.h>

// variables to store brew configuration/setpoints
int mashing_temp_setpoint;
int mashing_time_setpoint;


int boiler_temp_setpoint = 0;
int boiler_temp_setpoint_in;
int boiler_time_setpoint;
int hops_time_setpoint;

// variables to store sensor/system states
float boiler_temp_feedback = 12.2;
float boiler_temp_feedback_memory; //stores previous boiler temp value

int ledpin =11; // LED connected to pin 48 (on-board LED)
int temp_pin = 3; // Temperature sensor pin in interrupt enabled pin

boolean start_cfg_recvd = 0;

// Defining the scheduling interval for each function
#define readMessageFromUL_Cycle 2500U
#define readTemp_Cycle 100U
#define controlTemp_Cycle 5000U
#define taskCycle 5000U

// Define the temperature sensor data pin = 3
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define variables to store last run time (in ms) for each function (used by task scheduler).
unsigned long readMessageFromUL_LastMillis = 0;
unsigned long readTemp_LastMillis = 0;
unsigned long controlTemp_LastMillis = 0;
unsigned long taskLastMillis = 0;

unsigned long start_brew_cycle_time;
unsigned long elapsed_brew_cycle_time;
unsigned long t;

  void setup()
  {
  Serial.begin(9600); //set baud rate for bluetooth serial input port
  // Start up the library
  sensors.begin();

  //testing setup
  pinMode(13, OUTPUT);
 // pinMode(3, OUTPUT);
}

//----------------------------------------------
// Main Running loop that calls the individual functions via task scheduler function (cycleCheck)
//----------------------------------------------
void loop()
{
  if(taskScheduler(&readMessageFromUL_LastMillis, readMessageFromUL_Cycle))  // if readpacket is currently scheduled to be called
  {
     //run task 1
    readMessageFromUL();
  }
  if(taskScheduler(&readTemp_LastMillis, readTemp_Cycle) && start_cfg_recvd) // if readtemp is currently scheduled to be called
  {
      readTemp();
  }
  if(taskScheduler(&controlTemp_LastMillis, controlTemp_Cycle) && start_cfg_recvd)    // task schedule template, add new tasks here and update names
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
              //read the values into integer storage for the control system.
              
              //convert all strings into integers
              mashing_temp_setpoint = Mashtemp.toInt();
              mashing_time_setpoint = Mashtime.toInt();
              boiler_temp_setpoint_in = Boiltemp.toInt();
              boiler_time_setpoint = Boiltime.toInt();
              hops_time_setpoint = Hopstime.toInt(); 

              if(!start_cfg_recvd)
              {
                start_brew_cycle_time = millis();
              }
              start_cfg_recvd = true;
              
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
   boiler_temp_feedback_memory = boiler_temp_feedback;
   sensors.requestTemperatures(); // Send the command to get temperatures
   boiler_temp_feedback = sensors.getTempCByIndex(0);
   int boiler_temp_feedback_int = boiler_temp_feedback*100;
   if((boiler_temp_feedback - boiler_temp_feedback_memory)!= 0)
   {  
      Serial.print("Temperature: ");
      Serial.println(boiler_temp_feedback_int);
      Serial.print("Time: ");
      Serial.println(millis() - start_brew_cycle_time);
   }
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
  elapsed_brew_cycle_time = millis();
  t = elapsed_brew_cycle_time - start_brew_cycle_time;
  if(t <= mashing_time_setpoint*1000)
  {
    boiler_temp_setpoint = mashing_temp_setpoint;
  }
  Serial.println();
  Serial.println((t > mashing_time_setpoint*1000) && (t < (mashing_time_setpoint*1000 + boiler_time_setpoint*1000)));
  
  if ((t > mashing_time_setpoint*1000) && (t < (mashing_time_setpoint*1000 + boiler_time_setpoint*1000)))
  {
    boiler_temp_setpoint = boiler_temp_setpoint_in;
  }
  else {
    boiler_temp_setpoint = 0;
  }
  Serial.println();
  Serial.print("Boiler Temp SP:");
  Serial.println(boiler_temp_setpoint);
  Serial.print("Boiler Temp FB: ");
  Serial.println(boiler_temp_feedback);
  Serial.print("Elapsed Time:");
  Serial.println(t);
  Serial.println();
  Serial.println("Mashing Temp Setpoint In: ");
  Serial.print(mashing_temp_setpoint);
  Serial.println("Mashing Time Setpoint In: ");
  Serial.print(mashing_time_setpoint);
  Serial.println("Boiler Temp Setpoint In: " );
  Serial.print(boiler_temp_setpoint_in);
  Serial.println("Boiler Time Setpoint In: ");
  Serial.print(boiler_time_setpoint);
  Serial.println();

  float temp_difference= (boiler_temp_setpoint - boiler_temp_feedback);
  float output = temp_difference*19 + (boiler_temp_feedback - boiler_temp_feedback_memory)*1;
  if( output > 100)
  {
    output = 100;
  }
  if (output < 0)
  {
    output = 0;
  }

  int pwm = output*2.55;
  analogWrite(ledpin,pwm);
  //Serial.print("Heater Ouput PWM:");
  //Serial.println(output);
  //Serial.println(pwm);
}
//Function to abort the brew process. input parameter is the system state, i.e. what stage in the brewing process we are at.
// the stage will determine the correct course of action for aborting the process
void abortProcess(int system_state)
{
  //implement abort process
}

