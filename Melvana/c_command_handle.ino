////// ----- SSID Handle ---------
void cache ssid_command (String value)
{
          char bufchar[BUFSIZE];
          value = value.substring(0,BUFSIZE-1);
          value.toCharArray(bufchar, BUFSIZE);

          if (strcmp(ssid, bufchar) != 0)
            {
              Serial.print(F("New SSID: "));
              networkrestart = true;   //****** MUST UNCOMMENT.......
              wifimode = 2; // Set this to sort out the mode, backup.. 
              if (EEPROM.read(APbyte) != flagvalue) EEPROM.write(APbyte, flagvalue); // Set up Emergency ACCESSS BYTE....
              //send_mqtt_msg("status","SSID-> " + value);
              for (int i = 0; i < BUFSIZE; ++i) {
                 ssid[i] = bufchar[i];
              }
              Serial.println(ssid);     
              Save_String(ssid, ssidAddress, ssidAddressbyte);

            }
   }
   



////// ----- SSID Handle ---------


void cache password_command (String value)
{
          char bufchar[BUFSIZE];
          value = value.substring(0,BUFSIZE-1);
          value.toCharArray(bufchar, BUFSIZE);
          if (strcmp(password, bufchar) != 0)
            {
              Serial.print(F("New Password: "));
              networkrestart = true;   //****** MUST UNCOMMENT.......
              wifimode = 2;
              if (EEPROM.read(APbyte) != flagvalue) EEPROM.write(APbyte, flagvalue);
              //send_mqtt_msg("password","SSID-> " + value);

              for (int i = 0; i < BUFSIZE; ++i) {
                 password[i] = bufchar[i];
              }
              Serial.println(password);     
              Save_String(password, passwordAddress, passwordAddressbyte);


            }
   }

void cache deviceid_command (String value)
{
          char bufchar[BUFSIZE];
          value = value.substring(0,BUFSIZE-1);
          value.toCharArray(bufchar, BUFSIZE);
          if (strcmp(deviceid, bufchar) != 0)
            {
              Serial.print(F("New Deviceid: "));
              send_mqtt_msg(F("status"),"DeviceID-> " + value);

              for (int i = 0; i < BUFSIZE; ++i) {
                 deviceid[i] = bufchar[i];
              }
              Serial.println(deviceid);     
              Save_String(deviceid, deviceidAddress,deviceidAddressbyte);

            }
   }
   
   
 /// -- MQTT Server ----
 
void cache mqttserver_command (String value)
{

  IPAddress tempaddress;
  
  for (int i = 0; i < 4; i++) {
   String token = value.substring(0,value.indexOf('.'));
   value = value.substring(value.indexOf('.') + 1 , value.length());
   //Serial.println(String(i) + " : " + token);
   int tokenInt = token.toInt();
   tempaddress[i] = (byte)tokenInt;
   }

   if (MQTTserver != tempaddress) {
   Serial.print(F("MQTTserver IPAddress: "));
   MQTTserver = tempaddress;
   //send_mqtt_msg("Status","MQTTServer-> " + MQTTserver);
   Serial.println(tempaddress);

   for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    EEPROM.write( mqttAddress+i,(byte)MQTTserver[i]);
   }

    EEPROM.write (mqttAddressbyte,flagvalue);
    EEPROM_commit_var = true;
    MQTT_enabled = true;
    EEPROM.write(MQTTenabledbyte, flagvalue); // write mqtt enabled byte....
    EEPROM_commit_var = true;
    mqttreload = true;

    }


   }


   
void cache debug_command (String value)    {


 Serial.println(F("DEBUG COMMAND IS CALLED"));

 if(value == "timer")

 {

    Serial.println(F("DEBUG IS TIMER"));
    DEBUG = 1;
    if(EEPROM.read(DEBUGenabledbyte) != 1) EEPROM.write(DEBUGenabledbyte, 1);
    EEPROM_commit_var = true;
    //flipper.attach(5, flip);
    send_mqtt_msg (F("Status"), F("Debug set to timer"));


 }

 if(value == "off")

 {
    DEBUG = 0;
    Serial.println(F("DEBUG IS DISBALED"));
    if(EEPROM.read(DEBUGenabledbyte) != 0) EEPROM.write(DEBUGenabledbyte, 0);
    EEPROM_commit_var = true;
    send_mqtt_msg (F("Status"), F("Debug disabled"));



 }

}



  
void cache mqttreloadfunc ()

{    
      mqttreload = false;
      mqttclient.disconnect();      
      delay(10);     
      initiatemqqt (); 
                                
}


