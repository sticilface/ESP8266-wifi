////// ----- SSID Handle ---------
void ssid_command (String value)
{
          Serial.print("*SSID recieved....");
          String buf;
          buf = value.substring(0,BUFSIZE-1);
          buf.toCharArray(bufchar, BUFSIZE);
          if (strcmp(ssid, bufchar) == 0)
            {
              Serial.println("No change. Exiting SSID command");
              return;
            } else {
              Serial.print("New SSID recieved: ");
              networkrestart = true;   //****** MUST UNCOMMENT.......
              if (EEPROM.read(APbyte) != flagvalue) EEPROM.write(APbyte, flagvalue); // Set up Emergency ACCESSS BYTE....
              send_mqtt_msg("status","SSID updated: " + value);
              //eeprom_updates_pending = true;
              for (int i = 0; i < BUFSIZE; ++i) {
                 ssid[i] = bufchar[i];
              }
              Serial.println(ssid);     
              

              /// Save changes
              Save_String(ssid, ssidAddress,ssidAddressbyte);

            }
   }
   



////// ----- SSID Handle ---------


void password_command (String value)
{
          Serial.print("*Password recieved....");
          String buf;
          buf = value.substring(0,BUFSIZE-1);
          buf.toCharArray(bufchar, BUFSIZE);
          if (strcmp(password, bufchar) == 0)
            {
              Serial.println("No change. Exiting Password command");
              return;
            } else {
              Serial.print("New Password recieved: ");
              networkrestart = true;   //****** MUST UNCOMMENT.......
              if (EEPROM.read(APbyte) != flagvalue) EEPROM.write(APbyte, flagvalue);
              send_mqtt_msg("password","SSID updated: " + value);
              //eeprom_updates_pending = true;
              for (int i = 0; i < BUFSIZE; ++i) {
                 password[i] = bufchar[i];
              }
              Serial.println(password);     
              
              Save_String(password, passwordAddress,passwordAddressbyte);


            }
   }

void deviceid_command (String value)
{
          Serial.print("*Deviceid recieved....");
          String buf;
          buf = value.substring(0,BUFSIZE-1);
          buf.toCharArray(bufchar, BUFSIZE);
          if (strcmp(deviceid, bufchar) == 0)
            {
              Serial.println("No change. Exiting Deviceid command");
              return;
            } else {
              Serial.print("New Deviceid recieved: ");
              send_mqtt_msg("status","DeviceID updated: " + value);
              //eeprom_updates_pending = true;
              for (int i = 0; i < BUFSIZE; ++i) {
                 deviceid[i] = bufchar[i];
              }
              Serial.println(deviceid);     
              Save_String(deviceid, deviceidAddress,deviceidAddressbyte);

            }
   }
   
   
 /// -- MQTT Server ----
 
void mqttserver_command (String value)
{
          Serial.print("*MQTTServer recieved....");
          Serial.println(value);
          IPAddress tempaddress;
  for (int i = 0; i < 4; i++) {
   String token = value.substring(0,value.indexOf('.'));
   value = value.substring(value.indexOf('.') + 1 , value.length());
   //Serial.println(String(i) + " : " + token);
   int tokenInt = token.toInt();
   tempaddress[i] = (byte)tokenInt;
   }

   if (MQTTserver == tempaddress) {
    Serial.println("No change. Exiting MQTTServer command");
    return;
    } else {
   Serial.print("MQTTserver IPAddress CHANGED: ");
   MQTTserver = tempaddress;
   send_mqtt_msg("Status","MQTTServer updated: " + MQTTserver);
   Serial.println(tempaddress);

   for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    EEPROM.write( mqttAddress+i,(byte)MQTTserver[i]);
   }
    EEPROM.write (mqttAddressbyte,flagvalue);
    EEPROM.commit();
 /*

   IPAddress READtest;
     for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    READtest[i] = EEPROM.read( mqttAddress+i);
   }

  Serial.print("READ test : " );
  Serial.println(READtest);

*/
mqttreload = true;

 }


/*
          String buf;
          buf = value.substring(0,BUFSIZE-1);
          buf.toCharArray(bufchar, BUFSIZE);
          if (strcmp(mqttserver, bufchar) == 0)
            {
              Serial.println("No change. Exiting MQTTServer command");
              return;
            } else {
              Serial.print("New MQTTServer recieved: ");
              send_mqtt_msg("Status","MQTTServer updated: " + value);
              //eeprom_updates_pending = true;
              for (int i = 0; i < BUFSIZE; ++i) {
                 mqttserver[i] = bufchar[i];
              }
              Serial.println(mqttserver);     
              Save_String(mqttserver, mqttAddress,mqttAddressbyte);

               mqttreload = true;

            } */
   }
   
void debug_command (String value)

{
 Serial.println("DEBUG COMMAND IS CALLED");

 if(value == "timer")

 {

    Serial.println("DEBUG IS TIMER");
    DEBUG = 1;
    if(EEPROM.read(DEBUGenabledbyte) != 1) EEPROM.write(DEBUGenabledbyte, 1);
    EEPROM.commit();
    //flipper.attach(5, flip);
    send_mqtt_msg ("Status", "Debug set to timer");


 }

 if(value == "off")

 {
    DEBUG = 0;
    Serial.println("DEBUG IS DISBALED");
    if(EEPROM.read(DEBUGenabledbyte) != 0) EEPROM.write(DEBUGenabledbyte, 0);
    EEPROM.commit();
    //flipper.detach();
    //  digitalWrite(13, LOW);  
    send_mqtt_msg ("Status", "Debug disabled");



 }






}



  
void mqttreloadfunc ()

{

  // if (mqttreload) 
    
      mqttreload = false;
      Serial.println("Disconnecting MQTT Client:..... ");
      mqttclient.disconnect();      
      delay(10);
      Serial.println("Disconnecting MQTT wifiClient:..... ");
      wifiClient.stop();
      delay(10);      
      Serial.println("Creating new MQTT Client:..... ");
      WiFiClient wifiClient;
      delay(10);
      Serial.println("Calling pubsubclient:..... ");
      //PubSubClient mqttclient(mqttserver, 1883, callback, wifiClient);
      delay(10);
      initiatemqqt (); 
                         
       
}
  
void Serial_Command (String value)

{
  Serial.println("#" + value);
}






  


 // end of action handler....

