#include <OneWire.h>
#include <DallasTemperature.h>

// variables to store brew configuration/setpoints
unsigned long mashing_temp_setpoint;
unsigned long mashing_time_setpoint;


unsigned long boiler_temp_setpoint = 0;
unsigned long boiler_temp_setpoint_in;
unsigned long boiler_time_setpoint;
unsigned long hops_time_setpoint;

// variables to store sensor/system states
float boiler_temp_feedback_1 = 0;
float boiler_temp_feedback_1_memory; //stores previous boiler temp value

// variables to store sensor/system states
float boiler_temp_feedback_2 = 0;
float boiler_temp_feedback_2_memory; //stores previous boiler temp value

int boiler_temp_feedback_avg_memory;
int boiler_temp_feedback_avg = 0;

int pwmpin =10; // LED connected to pin 48 (on-board LED)
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
unsigned long long t;

String system_state = "Standby";

  void setup()
  {
  Serial.begin(9600); //set baud rate for bluetooth serial input port
  // Start up the library
  sensors.begin();
  TCCR1B = (TCCR1B & 0b11111000) | 0x05;

  //testing setup
  pinMode(10, OUTPUT);
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
   boiler_temp_feedback_1_memory = boiler_temp_feedback_1;
   boiler_temp_feedback_2_memory = boiler_temp_feedback_2;
   boiler_temp_feedback_avg_memory = boiler_temp_feedback_avg;
   sensors.requestTemperatures(); // Send the command to get temperatures
   boiler_temp_feedback_1 = sensors.getTempCByIndex(0);
   boiler_temp_feedback_2 = sensors.getTempCByIndex(1);
   int boiler_temp_feedback_1_int = boiler_temp_feedback_1*100;
   int boiler_temp_feedback_2_int = boiler_temp_feedback_2*100;
   if(((boiler_temp_feedback_1 - boiler_temp_feedback_1_memory)!= 0)||((boiler_temp_feedback_2 - boiler_temp_feedback_2_memory)!= 0))
   {  
      boiler_temp_feedback_avg = ( boiler_temp_feedback_1_int + boiler_temp_feedback_2_int)/2;
      Serial.print("Temperature: ");
      Serial.println(boiler_temp_feedback_avg);
      Serial.println("----------------------------------------");

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
  unsigned long remaining_time;
  elapsed_brew_cycle_time = millis();
  t = elapsed_brew_cycle_time - start_brew_cycle_time;
  if(t <= mashing_time_setpoint*1000)
  {
    system_state = "Mashing";
    boiler_temp_setpoint = mashing_temp_setpoint;
    remaining_time = mashing_time_setpoint - t/1000;
  }
  else if ((t > mashing_time_setpoint*1000) && (t < (mashing_time_setpoint*1000 + boiler_time_setpoint*1000)))
  {
    system_state = "Boiling";
    boiler_temp_setpoint = boiler_temp_setpoint_in;
    remaining_time = boiler_time_setpoint - t/1000;
  }
  else {
    system_state = "Finished";
    boiler_temp_setpoint = 0;
    remaining_time = 0;
  }

  float temp_difference= (boiler_temp_setpoint - boiler_temp_feedback_avg);
  float output = temp_difference*19 + (boiler_temp_feedback_avg - boiler_temp_feedback_avg_memory)*1;
  float boiler_temp_feedback_avg_float = boiler_temp_feedback_avg / 100;
  if( output > 100)
  {
    output = 100;
  }
  if (output < 0)
  {
    output = 0;
  }

  int pwm = output*2.55;
  analogWrite(pwmpin,pwm);
  Serial.print("System State: ");
  Serial.println(system_state);
  Serial.print("Time Remaining: ");
  Serial.print(remaining_time);
  Serial.println(" seconds");
  Serial.print("Boiler Temperature Setpoint: ");
  Serial.println(boiler_temp_setpoint);
  Serial.print("Boiler Temperature Feedback: ");
  Serial.println(boiler_temp_feedback_avg_float);
  Serial.print("Heater PWM Signal: ");
  Serial.print(pwm*100/255);
  Serial.println(" %");
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println();
}
//Function to abort the brew process. input parameter is the system state, i.e. what stage in the brewing process we are at.
// the stage will determine the correct course of action for aborting the process
void abortProcess(int system_state)
{
  //implement abort process
}

