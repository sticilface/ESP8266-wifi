void LoadParams()
{
  
  if (EEPROM.read(deviceidAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(deviceidAddress, deviceid, BUFSIZE);
  if (EEPROM.read(ssidAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(ssidAddress, ssid , BUFSIZE);
  if (EEPROM.read(passwordAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(passwordAddress, password, BUFSIZE);
  if (EEPROM.read(mqttAddressbyte) == flagvalue) EepromUtil::eeprom_read_string(mqttAddress, mqttserver, BUFSIZE);
  
  if (EEPROM.read(MQTTenabledbyte) == flagvalue) {
  	MQTT_enabled = true;
  } else {
  	MQTT_enabled = false;
  }

  if (EEPROM.read(Temperature_enablebyte) == flagvalue) {
  	isrunning = true;
  	// Plugin_enable
  } else {
  	isrunning = false;
  }   

  if (EEPROM.read(Humidity_enablebyte) == flagvalue) {
  	isrunning_Humidity = true;
  	// Plugin_enable
  } else {
  	isrunning_Humidity = false;
  } 


  if (EEPROM.read(DEBUGenabledbyte) != 0) {
  		if (EEPROM.read(DEBUGenabledbyte) == 1) {
  			debug_command("timer");
  		}
  
	} else {
	  	DEBUG = false;

	}

}

