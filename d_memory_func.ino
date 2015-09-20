void LoadParams()
{
  
  if (EEPROM.read(deviceidAddressbyte) == flagvalue) EEPROM_readAnything(deviceidAddress, deviceid);
  if (EEPROM.read(ssidAddressbyte) == flagvalue) EEPROM_readAnything(ssidAddress, ssid);
  if (EEPROM.read(passwordAddressbyte) == flagvalue) EEPROM_readAnything(passwordAddress, password);

  if (EEPROM.read(mqttAddressbyte) == flagvalue) 
  {
    //EepromUtil::eeprom_read_string(mqttAddress, mqttserver, BUFSIZE);
     for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    MQTTserver[i] = EEPROM.read( mqttAddress+i);
   }
   
   mqttserver_string = String(MQTTserver[0]) + "." + String(MQTTserver[1]) + "." + String(MQTTserver[2]) + "." + String(MQTTserver[3]) ; 

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
delay(100);
}




