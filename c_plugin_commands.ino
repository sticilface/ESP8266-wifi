/// Use for plugin functions......  



const int SpAddressbyte = 21;
const int KpAddressbyte = 22;
const int KiAddressbyte = 23;
const int KdAddressbyte = 24;
const int WindowSizeAddressbyte = 25;

const int Humidity_SpAddressbyte = 26;
const int Humidity_KpAddressbyte = 27;
const int Humidity_KiAddressbyte = 28;
const int Humidity_KdAddressbyte = 29;
const int Humidity_WindowSizeAddressbyte = 30;


const int SpAddress = 224; // 8 bytes i think required...
const int KpAddress = 232;
const int KiAddress = 240;
const int KdAddress = 248;
const int WindowSizeAddress = 256;

const int Humidity_SpAddress = 264; // 8 bytes i think required...
const int Humidity_KpAddress = 272;
const int Humidity_KiAddress = 280;
const int Humidity_KdAddress = 288;
const int Humidity_WindowSizeAddress = 296;




double Setpoint;
double Input;
double Output;

double Input_Humidity;
double Output_Humidity;
double Setpoint_Humidity;
// pid tuning parameters
double Kp;
double Ki;
double Kd;

float Humidity;


double Kp_Humidity;
double Ki_Humidity;
double Kd_Humidity;
float pct_Humidity;
int WindowSize_Humidity = 10000; 
unsigned long windowStartTime_Humidity;
volatile long onTime_Humidity = 0;



String Status;
// ************************************************
// Pin definitions
// ************************************************

// Output Relay
#define RelayPin 5 // Temp
#define RelayPin2 4 // Humidity
#define RelayPin3 13 // Coil Relay...
#define LigthsPin 13
// One-Wire Temperature Sensor
// (Use GPIO pins for power/ground to simplify the wiring)
//#define ONE_WIRE_BUS 0 Not needed any more
//#define ONE_WIRE_PWR 3
//#define ONE_WIRE_GND 4

#define DHTPIN 12     // what pin we're connected to


#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE,15);


//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
PID myPID_humidity(&Input_Humidity, &Output_Humidity, &Setpoint_Humidity, Kp_Humidity, Ki_Humidity, Kd_Humidity, DIRECT);



// 10 second Time Proportional Output window
int WindowSize = 10000; 
unsigned long windowStartTime;

//byte ATuneModeRemember=2;

volatile long onTime = 0;


//double aTuneStep=500;
//double aTuneNoise=1;
//unsigned int aTuneLookBack=20;
float pct;
boolean tuning = false;
boolean isrunning = false; // true;
boolean isrunning_Humidity = false;
boolean eeprom_updates_pending = false;
// PID_ATune aTune(&Input, &Output); // DISABLE OUTTUNE TO START

const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;
long lastMQTTtime = 0;
long lastTemptime; 
int TempTimeout = 10000;

// ************************************************
// Sensor Variables and constants
// Data wire is plugged into port 2 on the Arduino

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature sensors(&oneWire);

// arrays to hold device address
//DeviceAddress tempSensor;


Ticker driverelay;
Ticker driverelay2;
//Ticker driveMQTT;
//Ticker driveMQTTtemp;

boolean driverelayinterrupt; 
boolean driverelayinterrupt2;

//bool MQTT_enabled = false;


/////// ---- Code base ------

void plugin_lights_command (String value)

{

  if (value == "ON" || value == "on")
  {
        if (!light_status) {
         light_status = true;
         Serial.println("Lights turned on..... BOOOM");
         send_mqtt_msg("Status","Lights Turned ON");
         digitalWrite(LigthsPin,HIGH);

         //mqttclient.publish("status", mqttcharbuf);
         return;

        }
  } else if (value == "OFF" || value == "off")
  {
        if (light_status) 

        {
         light_status = false;
         Serial.println("Lights turned off");
         send_mqtt_msg("Status","Lights Turned OFF");
         digitalWrite(LigthsPin,LOW);
         return;
        } 
  }




}

void melvide_state_command (String value)

{
  
  if (value == "ON" || value == "on")
  {
        if(!isrunning) {
         isrunning = true;
         Serial.println("Temperature Turned ON");
         //String msg = "ON";
         ReadTemperature();
         TempTimeout = 5000;
         send_mqtt_msg("Status","Temperature Turned ON");
         if (EEPROM.read(Temperature_enablebyte) != flagvalue) EEPROM.write(Temperature_enablebyte, flagvalue); // write mqtt enabled byte....
          EEPROM.commit();

         //mqttclient.publish("status", mqttcharbuf);
         return;

        }
  } else if (value == "OFF" || value == "off")
  {
        if(isrunning) {
         isrunning = false;
         if(!isrunning_Humidity) TempTimeout = 10000;
         Serial.println("Temperature Turned OFF.");
         send_mqtt_msg("Status","Temperature Turned OFF");
         if (EEPROM.read(Temperature_enablebyte) != 0) EEPROM.write(Temperature_enablebyte, 0); // write mqtt enabled byte....
          EEPROM.commit();
         return;


        }
  }
} // end of melvide state handle






/*
void melvide_setpoint_command (String value)

{
  Serial.print("Setpoint Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (Setpoint == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Setpoint Recieved: ");  
       Serial.println(ValueDouble);
       Setpoint = ValueDouble;
       send_mqtt_msg("Status","Setpoint:" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(SpAddress) != Setpoint ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(SpAddress,Setpoint);      ///// write new settemp if not equal....
                if(EEPROM.read(SpAddressbyte) != flagvalue)
                      {
                        EEPROM.write(SpAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }                       
            EEPROM.commit();
            }
       }
  
} */
  /*
void melvide_kp_command (String value)

{
  Serial.print("Kp Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (Kp == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Kp Recieved: ");  
       Serial.println(ValueDouble);
       Kp = ValueDouble;
       send_mqtt_msg("Status","Kp Changeed to :" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(KpAddress) != Kp ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(KpAddress,Kp);      ///// write new settemp if not equal....
                if(EEPROM.read(KpAddressbyte) != flagvalue)
                      {
                        EEPROM.write(KpAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }                       
            EEPROM.commit();
            }
       }
  
} */
/*
void melvide_kd_command (String value)

{
  
  Serial.print("Kd Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (Kd == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Kd Recieved: ");  
       Serial.println(ValueDouble);
       Kd = ValueDouble;
       send_mqtt_msg("Status","Kd Changeed to :" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(KdAddress) != Kd ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(KdAddress,Kd);      ///// write new settemp if not equal....
                if(EEPROM.read(KdAddressbyte) != flagvalue)
                      {
                        EEPROM.write(KdAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }                       
            EEPROM.commit();
            }
       }  
  
  
  
} */
/*
void melvide_ki_command (String value)

{
  
Serial.print("Ki Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (Ki == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Ki Recieved: ");  
       Serial.println(ValueDouble);
       Ki = ValueDouble;
       send_mqtt_msg("Status","Ki Changeed to :" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(KiAddress) != Ki ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(KiAddress,Ki);      ///// write new settemp if not equal....
                if(EEPROM.read(KiAddressbyte) != flagvalue)
                      {
                        EEPROM.write(KiAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      } 
            EEPROM.commit();                      
            }
       }  
  
} */
/*
void melvide_window_command (String value)

{
  
  Serial.print("Window Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  int ValueDouble = atoi(buf); 
  if (WindowSize == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Window Recieved: ");  
       Serial.println(ValueDouble);
       WindowSize = ValueDouble;
       send_mqtt_msg("Status","Window Changeed to :" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(WindowSizeAddress) != WindowSize ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(WindowSizeAddress,WindowSize);      ///// write new settemp if not equal....
                if(EEPROM.read(WindowSizeAddressbyte) != flagvalue)
                      {
                        EEPROM.write(WindowSizeAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }   
            EEPROM.commit();
                    
            }
       } 
  
  
  
} */

void Load_melvide_params()
{
    if (EEPROM.read(SpAddressbyte) == flagvalue) 
     {     
     Setpoint = EepromUtil::eeprom_read_double(SpAddress);
      if (isnan(Setpoint)) Setpoint = 60;
     } else {
       Setpoint = 60;
     }
  
      if (EEPROM.read(KpAddressbyte) == flagvalue) 
     {     
     Kp = EepromUtil::eeprom_read_double(KpAddress);
      if (isnan(Kp)) Kp = 850;
     } else {
       Kp = 850;
     }
  
       if (EEPROM.read(KiAddressbyte) == flagvalue) 
     {     
     Ki = EepromUtil::eeprom_read_double(KiAddress);
      if (isnan(Ki)) Ki = 0.5;
     } else {
       Ki = 0.5;
     }
     
      if (EEPROM.read(KdAddressbyte) == flagvalue) 
     {     
     Kd = EepromUtil::eeprom_read_double(KdAddress);
      if (isnan(Kd)) Kd = 0.1;
     } else {
       Kd = 0.1;
     }
     
       if (EEPROM.read(WindowSizeAddressbyte) == flagvalue) 
     {     
     WindowSize = EepromUtil::eeprom_read_double(WindowSizeAddress);
      if (isnan(WindowSize)) WindowSize = 10000;
     } else {
       WindowSize = 10000;
     }   

/// Humidty EEPROM retrieval

    if (EEPROM.read(Humidity_SpAddressbyte) == flagvalue) 
     {     
     Setpoint_Humidity = EepromUtil::eeprom_read_double(Humidity_SpAddress);
      if (isnan(Setpoint_Humidity)) Setpoint_Humidity = 60;
     } else {
       Setpoint_Humidity = 60;
     } 

      if (EEPROM.read(Humidity_KpAddressbyte) == flagvalue) 
     {     
     Kp_Humidity = EepromUtil::eeprom_read_double(Humidity_KpAddress);
      if (isnan(Kp)) Kp_Humidity = 850;
     } else {
       Kp_Humidity = 850;
     }
  
       if (EEPROM.read(Humidity_KiAddressbyte) == flagvalue) 
     {     
     Ki_Humidity = EepromUtil::eeprom_read_double(Humidity_KiAddress);
      if (isnan(Ki)) Ki_Humidity = 0.5;
     } else {
       Ki_Humidity = 0.5;
     }
     
      if (EEPROM.read(Humidity_KdAddressbyte) == flagvalue) 
     {     
     Kd_Humidity = EepromUtil::eeprom_read_double(Humidity_KdAddress);
      if (isnan(Kd)) Kd_Humidity = 0.1;
     } else {
       Kd_Humidity = 0.1;
     }
     
       if (EEPROM.read(Humidity_WindowSizeAddressbyte) == flagvalue) 
     {     
     WindowSize_Humidity = EepromUtil::eeprom_read_double(Humidity_WindowSizeAddress);
      if (isnan(WindowSize)) WindowSize_Humidity = 10000;
     } else {
       WindowSize_Humidity = 10000;
     } 


// if (server.arg("kp").length() != 0) double_pct_command (server.arg("kp"), Kp_Humidity, Humidity_KpAddress, Humidity_KpAddressbyte, "Kp_Humidity"); 
// if (server.arg("ki").length() != 0) double_pct_command (server.arg("ki"), Ki_Humidity, Humidity_KiAddress, Humidity_KiAddressbyte, "Ki_Humidity"); 
// if (server.arg("kd").length() != 0) double_pct_command (server.arg("kd"), Kd_Humidity, Humidity_KdAddress, Humidity_KdAddressbyte, "Kd_Humidity"); 

 }

// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************

  void DriveOutput()
{  
  if (isrunning)
  {
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
     windowStartTime += WindowSize;
  }
  if((onTime > 100) && (onTime > (now - windowStartTime)))
  {
     digitalWrite(RelayPin,HIGH);
     //digitalWrite(RelayPin2,HIGH);
     //digitalWrite(RelayPin3,HIGH);

     //Serial.println("ON");
  }
  else
  {
     digitalWrite(RelayPin,LOW);
     //digitalWrite(RelayPin2,LOW);
     //digitalWrite(RelayPin3,LOW);
     //Serial.println("OFF");

  }
  }
}


// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************

  void DriveOutput2()
{  
  if (isrunning_Humidity)
  {
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if(now - windowStartTime_Humidity>WindowSize_Humidity)
  { //time to shift the Relay Window
     windowStartTime_Humidity += WindowSize_Humidity;
  }
  if((onTime_Humidity > 100) && (onTime_Humidity > (now - windowStartTime_Humidity)))
  {
     digitalWrite(RelayPin2,HIGH);
     //digitalWrite(RelayPin2,HIGH);
     //digitalWrite(RelayPin3,HIGH);

     //Serial.println("ON");
  }
  else
  {
     digitalWrite(RelayPin2,LOW);
     //digitalWrite(RelayPin2,LOW);
     //digitalWrite(RelayPin3,LOW);
     //Serial.println("OFF");

  }
  }
}


void publishtemp ()
{
        lastMQTTtime = millis();        
        // Input.toCharArray(mqttcharbuf, 10);
         mqttbufcharclear ();      
        
        //String temptopic = String(deviceid) + "/temperature";
        //char mqtttemptopic[100];
        //temptopic.toCharArray(mqtttemptopic,100);

        String tempTemp = String(Input);
        //tempTemp.toCharArray(mqttcharbuf,20);    
        
        String tempHumidity = String(Humidity);     

      send_mqtt_msg("temperature", tempTemp);
      delay(2);
      send_mqtt_msg("humidity", tempHumidity);
      delay(2);
      if (isrunning) send_mqtt_msg("temperature-pct", String(pct));
      delay(2);      
      if (isrunning_Humidity) send_mqtt_msg("humidity-pct", String(pct_Humidity));

      //delay(2);
      //send_mqtt_msg("humidity", tempHumidity);


     //mqttclient.publish(mqtttemptopic, mqtttempmsg);
     
        /*    if(mqttclient.publish(mqtttemptopic, mqttcharbuf)) 
                   {
                       Serial.println(" Succeeded! Temp: " + String(Input));
                    } else {
                       Serial.println(" Failed! ");
                            } */
  
}

void ReadTemperature () 
{
  
//        sensors.requestTemperatures(); // Send the command to get temperatures
//  float TempC = sensors.getTempCByIndex(0);


    float H = dht.readHumidity();
  // Read temperature as Celsius
    float T = dht.readTemperature(); 

    //float H = 40;
    //float T = 22;

    float TempC;
  if (isnan(T) || isnan(H)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    TempC = T;
    Input = T;
    Humidity = H;
    Input_Humidity = H;
  }

  //Serial.println("Temperature: " + String(TempC));
  //Serial.println("Humidity: " + String(Humidity));
 if(isrunning) {
  //float TempC;
  if (TempC != 0.0)
  {
    Input = TempC;
  }
  else {
    Input = 99.99;
    Serial.println("WARNING: Temp Sensor ERROR!");
  }
}

  lastTemptime = millis();
}


void setup_Plugin () 
{   
  htmlendstring +=  "  <a href='/temperature'>  Temperature</a>  <a href='/humidity'>  Humidity</a>"; 
  //Serial.println(F("This string will be stored in flash memory"));
	Load_melvide_params();

  server.on("/temperature", handle_temperature);


  dht.begin();


	pinMode(RelayPin, OUTPUT);    // Output mode to drive relay
  pinMode(RelayPin2, OUTPUT);    // Output mode to drive relay
  pinMode(RelayPin3, OUTPUT);    // Output mode to drive relay
   
  digitalWrite(RelayPin, LOW);  // make sure it is off to start
  digitalWrite(RelayPin2, LOW);  // make sure it is off to start
  digitalWrite(RelayPin3, LOW);  // make sure it is off to start

   // Start up the DS18B20 One Wire Temperature Sensor

   //sensors.begin();


   myPID.SetTunings(Kp,Ki,Kd);

   myPID.SetSampleTime(1000);
   
   myPID.SetOutputLimits(0, WindowSize);
   
   myPID.SetMode(AUTOMATIC);

   //// Humidtiy set up

   myPID_humidity.SetTunings(Kp_Humidity,Ki_Humidity,Kd_Humidity);

   myPID_humidity.SetSampleTime(1000);
   
   myPID_humidity.SetOutputLimits(0, WindowSize_Humidity);
   
   myPID_humidity.SetMode(AUTOMATIC);



  timer.setInterval(TempTimeout, ReadTemperature);


}

void loop_Plugin()

{

   /*    if (millis() > (lastTemptime + TempTimeout))
       {
       //sensors.requestTemperatures(); // Send the command to get temperatures
       //Input = sensors.getTempCByIndex(0);
       ReadTemperature ();       
       } */

     if (millis() > (lastMQTTtime + MQTTtemptimeout)) 
      {
        publishtemp ();
      } 

if (isrunning)
 
 {
       myPID.SetTunings(Kp,Ki,Kd);
   
       pct = map(Output, 0, WindowSize, 0, 1000);
     
       myPID.Compute();

  // Time Proportional relay state is updated regularly via timer interrupt.
       onTime = Output;
     // Run();
       

      
      if(!driverelayinterrupt)
        {
          driverelay.attach(0.15, DriveOutput);
          //driveMQTTtemp.attach(0.5, ReadTemperature);
          driverelayinterrupt = true;
        }
 
  } else {
           if(driverelayinterrupt)
        {
          driverelay.detach();
          //driveMQTTtemp.detach();

          driverelayinterrupt = false;
        }
      pct = 0;
      //Input = 0;
      digitalWrite(RelayPin,LOW); // Make sure that the relay is OFF...

 } // end of else if for isrunning....

//// Humidity running LOOP

 if (isrunning_Humidity)
 
 {
       myPID_humidity.SetTunings(Kp_Humidity,Ki_Humidity,Kd_Humidity);  
       pct_Humidity = map(Output_Humidity, 0, WindowSize_Humidity, 0, 1000);
       myPID_humidity.Compute();
       // Time Proportional relay state is updated regularly via timer interrupt.
       onTime_Humidity = Output_Humidity;
       // Run();
       
       /* if (millis() > (lastMQTTtime + MQTTtemptimeout)) 
      {
        publishtemp ();
      } */ 
      
      if(!driverelayinterrupt2)
        {
          driverelay2.attach(0.15, DriveOutput2);
          //driveMQTTtemp.attach(0.5, ReadTemperature);
          driverelayinterrupt2 = true;
        }
 
  } else {
           if(driverelayinterrupt2)
        {
          driverelay2.detach();
          //driveMQTTtemp.detach();

          driverelayinterrupt2 = false;
        }
      pct_Humidity = 0;
      //Input = 0;
      digitalWrite(RelayPin2,LOW); // Make sure that the relay is OFF...

 }




}






void humidity_state_command (String value)

{
  
  if (value == "ON" || value == "on")
  {
        if(!isrunning_Humidity) {
         isrunning_Humidity = true;
         Serial.println("Humidity Turned ON");
         //String msg = "ON";
         ReadTemperature();
         TempTimeout = 5000;

         timer.setInterval(TempTimeout, ReadTemperature);


         send_mqtt_msg("Status","Humidity Turned ON");
         if (EEPROM.read(Humidity_enablebyte) != flagvalue) EEPROM.write(Humidity_enablebyte, flagvalue); // write mqtt enabled byte....
         EEPROM.commit();

         //mqttclient.publish("status", mqttcharbuf);
         return;

        }
  } else if (value == "OFF" || value == "off")
  {
        if(isrunning_Humidity) {
         isrunning_Humidity = false;
         if(!isrunning) TempTimeout = 10000;
         Serial.println("Humidity Turned OFF.");
         send_mqtt_msg("Status","Humidity Turned OFF");
         if (EEPROM.read(Humidity_enablebyte) != 0) EEPROM.write(Humidity_enablebyte, 0); // write mqtt enabled byte....
          EEPROM.commit();
         return;


        }
  }
} // end of melvide state handle
/*
float Humidity;
double Setpoint_Humidity;
double Output_Humidity;

double Kp_Humidity;
double Ki_Humidity;
double Kd_Humidity;
float pct_Humidity;
int WindowSize_Humidity = 10000; 
*/
/*
void humidity_setpoint_command (String value)
{

  Serial.print("Humidity Setpoint Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (Setpoint_Humidity == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New Humidity Setpoint Recieved: ");  
       Serial.println(ValueDouble);
       Setpoint_Humidity = ValueDouble;
       send_mqtt_msg("Status","Humidity Setpoint:" + value);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(Humidity_SpAddress) != Setpoint_Humidity ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(Humidity_SpAddress,Setpoint_Humidity);      ///// write new settemp if not equal....
                if(EEPROM.read(Humidity_SpAddressbyte) != flagvalue)
                      {
                        EEPROM.write(Humidity_SpAddressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }                       
            EEPROM.commit();
            }
       }


} */

void double_pct_command (String Newvalue, double OldValue, int Address, int Addressbyte, String var)

{


  Serial.print( var + " Recieved: ");
  //Serial.println("Newvalue: " + Newvalue);
  //Serial.println("Oldvalue: " + String(OldValue));
  //Serial.println("Address: " + String(Address));
  //Serial.println("Addressbyte: " + String(Addressbyte));
  char buf[Newvalue.length()];
  Newvalue.toCharArray(buf,50);
  double ValueDouble = os_atof(buf); 
  if (OldValue == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New " + var + " Recieved: ");  
       Serial.println(ValueDouble);
       
       // Setpoint_Humidity = ValueDouble;
       if (var == "Setpoint_Humidity") Setpoint_Humidity = ValueDouble;
       if (var == "Kp_Humidity") Kp_Humidity = ValueDouble;
       if (var == "Kd_Humidity") Kd_Humidity = ValueDouble;
       if (var == "Ki_Humidity") Ki_Humidity = ValueDouble;
       if (var == "Setpoint") Setpoint = ValueDouble;
       if (var == "Kp") Kp = ValueDouble;
       if (var == "Kd") Kd = ValueDouble;
       if (var == "Ki") Ki = ValueDouble;

       //Setpoint

//double Kp_Humidity;
//double Ki_Humidity;
//double Kd_Humidity;
       //if (var == "Setpoint_Humidity") Setpoint_Humidity = ValueDouble;
//Kp_Humidity      
       String msg = var + " Setpoint:" + Newvalue; 
       send_mqtt_msg("Status",msg);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(Address) != ValueDouble ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(Address,ValueDouble);      ///// write new settemp if not equal....
                if(EEPROM.read(Addressbyte) != flagvalue)
                      {
                        EEPROM.write(Addressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }                       
            EEPROM.commit();
            }
       }



}

void int_pct_command (String value, int OldValue, int Address, int Addressbyte, String var)

{
  
  Serial.println( var + " Recieved: ");
  char buf[value.length()];
  value.toCharArray(buf,50);
  int ValueDouble = atoi(buf); 

  if (OldValue == ValueDouble)
    {
       Serial.println("No change leaving command.");  
       return;
    } else
    {
       Serial.print("New " + var + " Recieved: ");  
       Serial.println(ValueDouble);
       
      if(var == "WindowSize") WindowSize = ValueDouble;
      if(var =="WindowSize_Humidity") WindowSize_Humidity = ValueDouble;

      String msg = var + " Changed to: " + value;
       send_mqtt_msg("Status",msg);
       //eeprom_updates_pending =  true;
         if(EepromUtil::eeprom_read_double(Address) != ValueDouble ) // && Kp != 850)
            {
                EepromUtil::eeprom_write_double(Address,ValueDouble);      ///// write new settemp if not equal....
                if(EEPROM.read(Addressbyte) != flagvalue)
                      {
                        EEPROM.write(Addressbyte,flagvalue) ;   ////  update flag bit if not set 
                      }   
            EEPROM.commit();
                    
            }
       } 
  
  
  
}





