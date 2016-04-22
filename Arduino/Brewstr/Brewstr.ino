#include <OneWire.h>
#include <DallasTemperature.h>


//Code Rules:
// For variables that aren't varied externally: m_variable_names
// For variables that are varied externally: e_variable_names
//For variables that are temporary: variable_names
// For Constant Values: ALLCAPSNOUNDERSCORES
// For Configuration Values: ALL_CAPS_WITH_UNDERSCORES

// variables to store brew configuration/setpoints
unsigned long e_mashing_temp_setpoint;
unsigned long e_mashing_time_setpoint;


unsigned long m_boiler_temp_setpoint = 0;
unsigned long e_boiler_temp_setpoint;
unsigned long e_boiler_time_setpoint;
unsigned long e_hops_time_setpoint;

// variables to store sensor/system states
float m_boiler_temp_1 = 0;
float m_boiler_temp_1_memory; //stores previous boiler temp value

// variables to store sensor/system states
float m_boiler_temp_2 = 0;
float m_boiler_temp_2_memory; //stores previous boiler temp value

int m_boiler_temp_avg_memory;
int m_boiler_temp_avg = 0;
float m_boiler_temp_avg_flt;
float m_fermenterMass;
float m_fermenterZeroMass;

int TEMP_PIN = 3; // Temperature sensor pin in interrupt enabled pin
int HEATER_PIN =10; // SSR Relay switching pin for heater power 
int PUMP_PIN = 5;
int DRAIN_VALVE_PIN = 6; //DRAIN VALVE PIN
int LEVEL_VALVE_PIN = 7; //LEVEL VALVE PIN
int BLEEDOFF_VALVE_PIN = 8;
int m_pwm_output = 0;

boolean m_start_cfg_recvd = false;
boolean m_reached_mash_temp = false;
boolean m_drain_valve = false;
boolean m_level_valve = false;

// Valve state constants
int OPEN = 255;
int CLOSE = 0;

boolean m_pump_pwr = false;
boolean m_overflow_valve = false;
boolean m_pump_bleed_off_valve = false;

// Defining the scheduling interval for each function
#define READ_MESSAGE_FROM_UL_CYCLE 2500U
#define READ_TEMP_CYCLE 100U
#define CONTROL_TEMP_CYCLE 500U
#define CONTROL_SYSTEM_STATE_CYCLE 5000U
#define CONTROL_PUMP_CYCLE 5000U
#define MASH_OUT_OFFSET 45000U
#define PUMP_PRIME_OFFSET 45000U

// Define the temperature sensor data pin = 3
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define variables to store last run time (in ms) for each function (used by task scheduler).
unsigned long m_read_message_from_ul_last_millis = 0;
unsigned long m_read_temp_last_millis = 0;
unsigned long m_control_temp_last_millis = 0;
unsigned long m_control_system_state_last_millis = 0;
unsigned long m_control_pump_last_millis = 0;

unsigned long m_start_brew_cycle_time;
unsigned long m_elapsed_brew_cycle_time;
unsigned long long m_time;
unsigned long m_time_remaining;


String m_system_state = "Standby";

  void setup()
  {
  Serial.begin(9600); //set baud rate for bluetooth serial input port
  // Start up the library
  sensors.begin();
  TCCR1B = (TCCR1B & 0b11111000) | 0x05;

  //testing setup
  pinMode(HEATER_PIN, OUTPUT);
}

//----------------------------------------------
// Main Running loop that calls the individual functions via task scheduler function (cycleCheck)
//----------------------------------------------
void loop() 
{
  if(taskScheduler(&m_read_message_from_ul_last_millis, READ_MESSAGE_FROM_UL_CYCLE))  // if readpacket is currently scheduled to be called
  {
    //run task 1
    readMessageFromUL();
  }
  if(taskScheduler(&m_read_temp_last_millis, READ_TEMP_CYCLE) && m_start_cfg_recvd) // if readtemp is currently scheduled to be called
  {
    readTemp();
  }
  if(taskScheduler(&m_control_temp_last_millis, CONTROL_TEMP_CYCLE) && m_start_cfg_recvd)    // task schedule template, add new tasks here and update names
  {
    //run task  
    controlTemp();
  }
  if(taskScheduler(&m_control_system_state_last_millis, CONTROL_SYSTEM_STATE_CYCLE))    // task schedule template, add new tasks here and update names
  {
    controlSystemState();
  }
  if(taskScheduler(&m_control_pump_last_millis, CONTROL_PUMP_CYCLE))
  {
    controlPump();  
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
  String mash_temp;
  String mash_time;
  String boil_temp;
  String boil_time;
  String hops_time;
  String message_type;
  
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
                              message_type += message.charAt(i);
                              break;
                        case 1:
                              mash_temp += message.charAt(i);
                        break;
                        case 2:
                              mash_time += message.charAt(i);
                        break;
                        case 3:
                              boil_temp += message.charAt(i);
                        break;
                        case 4:
                              boil_time += message.charAt(i);
                              break;
                        case 5:
                              hops_time += message.charAt(i);
                        default:
                        break;
                      }
      i+=1;
    }
    count += 1;
  }
              //read the values into integer storage for the control system.
              
              //convert all strings into integers
              e_mashing_temp_setpoint = mash_temp.toInt();
              e_mashing_time_setpoint = mash_time.toInt();
              e_boiler_temp_setpoint = boil_temp.toInt();
              e_boiler_time_setpoint = boil_time.toInt();
              e_hops_time_setpoint = hops_time.toInt(); 

              if(!m_start_cfg_recvd)
              {
                m_start_brew_cycle_time = millis();
              }
              m_start_cfg_recvd = true;
              
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
   m_boiler_temp_1_memory = m_boiler_temp_1;
   m_boiler_temp_2_memory = m_boiler_temp_2;
   m_boiler_temp_avg_memory = m_boiler_temp_avg;
   sensors.requestTemperatures(); // Send the command to get temperatures
   
   m_boiler_temp_1 = sensors.getTempCByIndex(0);
   m_boiler_temp_2 = sensors.getTempCByIndex(1);
   int boiler_temp_1_int = m_boiler_temp_1*100;
   int boiler_temp_2_int = m_boiler_temp_2*100;
   if(((m_boiler_temp_1 - m_boiler_temp_1_memory)!= 0)||((m_boiler_temp_2 - m_boiler_temp_2_memory)!= 0))
   {  
      m_boiler_temp_avg = ( boiler_temp_1_int + boiler_temp_2_int)/2;
      Serial.print("Temperature: ");
      Serial.println(m_boiler_temp_avg);
      Serial.print("Time: ");
      Serial.println(millis() - m_start_brew_cycle_time);
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

unsigned long controlSystemState()
{
  unsigned long remaining_time;
  m_elapsed_brew_cycle_time = millis();
  m_time = m_elapsed_brew_cycle_time - m_start_brew_cycle_time;
  if(!m_reached_mash_temp) 
  {
    m_boiler_temp_setpoint = e_mashing_temp_setpoint;
    remaining_time = 0;
    if(m_boiler_temp_avg/100 < (e_mashing_temp_setpoint - 2)){
      m_system_state = "Pre-heating: ";
    }
    else{
    m_start_brew_cycle_time = millis();
    m_reached_mash_temp = true;
    }
  }
  else if(m_time <= e_mashing_time_setpoint*60000)
  {
    m_system_state = "Mashing";
    m_boiler_temp_setpoint = e_mashing_temp_setpoint;
    m_time_remaining = e_mashing_time_setpoint*60 - m_time/1000;
  }
  else if ((m_time > e_mashing_time_setpoint*60000) && (m_time < (e_mashing_time_setpoint*60000 + e_boiler_time_setpoint*60000)))
  {
    if(m_time < (e_mashing_time_setpoint*60000 + MASH_OUT_OFFSET))
    {
      m_system_state = "Mash Out";
      m_boiler_temp_setpoint = e_boiler_temp_setpoint;
      m_time_remaining = (e_mashing_time_setpoint*60 - MASH_OUT_OFFSET/1000) - m_time/1000;
    }
    else{
      m_drain_valve = true;
      m_level_valve = true;
      if(m_system_state == "Mash Out")
      {
        primePump();
      }
      m_system_state = "Boiling";
      m_boiler_temp_setpoint = e_boiler_temp_setpoint;
      m_time_remaining = (e_boiler_time_setpoint + e_mashing_time_setpoint)*60 - m_time/1000;
      m_pump_pwr = true;
    }
  }
  else {
    m_level_valve = false;
    m_system_state = "Finished";
    m_boiler_temp_setpoint = 0;
    remaining_time = 0;
  }
  Serial.print("System State: ");
  Serial.println(m_system_state);
  Serial.print("Time Remaining: ");
  Serial.print(m_time_remaining);
  Serial.println(" seconds");
  Serial.print("Boiler Temperature Setpoint: ");
  Serial.println(m_boiler_temp_setpoint);
  Serial.print("Boiler Temperature Feedback: ");
  Serial.println(m_boiler_temp_avg_flt);
  Serial.print("Heater PWM Signal: ");
  Serial.print(m_pwm_output*100/255);
  Serial.println(" %");
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println();
}
//--------------------------------------------------
// Function to compare setpoint and feedback of boiler.
//--------------------------------------------------
void controlTemp()
{
  m_boiler_temp_avg_flt = m_boiler_temp_avg;
  m_boiler_temp_avg_flt = m_boiler_temp_avg_flt/100;
  float temp_difference= (m_boiler_temp_setpoint - m_boiler_temp_avg_flt);
  float output = temp_difference*75 + (m_boiler_temp_avg - m_boiler_temp_avg_memory)*1;
  float boiler_temp_avg_float = m_boiler_temp_avg;
  boiler_temp_avg_float = boiler_temp_avg_float/100;
  if( output > 100)
  {
    output = 100;
  }
  if (output < 0)
  {
    output = 0;
  }

  m_pwm_output = output*2.55;
  analogWrite(HEATER_PIN,m_pwm_output);
}

void pumpPrime() 
{
  prepFermenter();
  releaseAir();
}

void prepFermenter () 
{
  valveAction("level", OPEN);  
  valveAction("drain",OPEN);
  while(m_fermenterMass < m_fermenterZeroMass);
  valveAction("level",CLOSE);
  valveAction("drain", CLOSE);
}

void releaseAir() 
{
  valveAction("bleedoff", OPEN);
  //run pump for 5 seconds to bleed air off
  valveAction("bleedoff", CLOSE);
}

void valveAction(String valve, int valve_state) 
{
  if(valve == "bleedoff") 
  {
    analogWrite(BLEEDOFF_VALVE_PIN, valve_state); 
  } 
  else if(valve = "drain")
  {
    analogWrite(DRAIN_VALVE_PIN, valve_state);
  }
  else{ //valve = "level"
    analogWrite(LEVEL_VALVE_PIN, valve_state);
  }
}

//Function to abort the brew process. input parameter is the system state, i.e. what stage in the brewing process we are at.
// the stage will determine the correct course of action for aborting the process
void abortProcess(int system_state)
{
  //implement abort process
}
// Function to control power to the 12vDC wort pump
void controlPump()
{
  if(m_pump_pwr)
  {
    analogWrite(PUMP_PIN,OPEN);
  }
}
void primePump()
{
  //prime the pump by running for 45 seconds with bleed off valve.
  unsigned long pump_prime_start_time = millis();
  m_pump_bleed_off_valve = true;
  m_pump_pwr = true;
  while(millis() <= (pump_prime_start_time + PUMP_PRIME_OFFSET))
  {
    // wait
  }
  m_pump_bleed_off_valve = false;
}
void controlValveState()
{
  if(m_drain_valve)
  {
    valveAction("drain",OPEN);
  }else
  {
    valveAction("drain",CLOSE);
  }
  if(m_level_valve)
  {
    valveAction("level",OPEN);
  }else
  {
    valveAction("level",CLOSE);
  }    
}




