String message; //string that stores the incoming message
String Boiltime;
String Hopstime;
String Boiltemp;
int boiler_temp;
int boiler_time;
int hops_time;
int ledpin = 12; // LED connected to pin 48 (on-board LED)

// Defining the scheduling interval for each function
#define readPacket_Cycle 10000U
#define taskCycle 2750U
// Define variables to store last run time (in ms) for each function.
unsigned long readPacket_LastMillis = 0;
unsigned long taskLastMillis = 0;

void setup()
{
  Serial.begin(9600); //set baud rate for bluetooth serial input port
}

//----------------------------------------------
// Main Running loop that calls the individual functions via task scheduler function (cycleCheck)
//----------------------------------------------
void loop()
{
  if(cycleCheck(&readPacket_LastMillis, readPacket_Cycle))  // if readpacket is currently scheduled to be called
  {
     //run task 1
    readPacket();
    Serial.println("test");
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
                              Boiltemp += message.charAt(i);
                        break;
                        case 1:
                              Boiltime += message.charAt(i);
                        break;
                        case 2:
                              Hopstime += message.charAt(i);
                        break;
                        default:
                        break;
                      }
			i+=1;
		}
		count += 1;
		i+=1;
	}
	
	//read the values into integer storage for the control system.
	boiler_temp = Boiltemp.toInt();
	boiler_time = Boiltime.toInt();
	hops_time = Hopstime.toInt(); 
  Serial.println(message);
  Serial.flush();
}
void sendStatusToUL()
{
  //send status messages to android app
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
    

