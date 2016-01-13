String message; //string that stores the incoming message
String Boiltime;
String Hopstime;
String Boiltemp;
int boiler_temp;
int boiler_time;
int hops_time;
int ledpin = 12; // LED connected to pin 48 (on-board LED)

void setup()
{
  Serial.begin(9600); //set baud rate
  pinMode(ledpin, OUTPUT);  // pin 48 (on-board LED) as OUTPUT
}

void loop()
{
	if(Serial.available())
	{
		readPacket();
	}
 
  delay(1000);
  message = "";
 
}

void readPacket()
{
	while(Serial.available())
	{
		//while there is data available on the serial monitor
		message+=char(Serial.read());//store string from serial command
	}
     
	// we now have the incoming packet, must decode:
	decodeMessage();
}

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


    

