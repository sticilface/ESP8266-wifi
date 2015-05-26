void LoadParams()
{
  
  if (EEPROM.read(deviceidAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(deviceidAddress, deviceid, BUFSIZE);
  if (EEPROM.read(ssidAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(ssidAddress, ssid , BUFSIZE);
  if (EEPROM.read(passwordAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(passwordAddress, password, BUFSIZE);
  if (EEPROM.read(mqttAddressbyte) == flagvalue) 
  {
    //EepromUtil::eeprom_read_string(mqttAddress, mqttserver, BUFSIZE);
     for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    MQTTserver[i] = EEPROM.read( mqttAddress+i);
   }
   
   mqttserver = String(MQTTserver[0]) + "." + String(MQTTserver[1]) + "." + String(MQTTserver[2]) + "." + String(MQTTserver[3]) ; 

   Serial.print("SAVED MqqttServer: ");
   Serial.println(mqttserver);
  }
  
  if (EEPROM.read(MQTTenabledbyte) == flagvalue) {
  	MQTT_enabled = true;
  } else {
  	MQTT_enabled = false;
  }





  if (EEPROM.read(DEBUGenabledbyte) != 0) {
  		if (EEPROM.read(DEBUGenabledbyte) == 1) {
  			debug_command("timer");
  		}
  
	} else {
	  	DEBUG = false;

	}

}


void EEPROM_wipe() {

for (int i = 0; i < 512; i++) EEPROM.write(i,0);
EEPROM.commit();
Serial.print("EEPROM WIPED");
}




