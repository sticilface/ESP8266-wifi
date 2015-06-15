String ICACHE_FLASH_ATTR macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 2; ++i) {
    result += String(mac[i+4], 16);
    //if (i < 5)
      //result += ':';
  }
  return result;
}

void ICACHE_FLASH_ATTR clearbufchar()
{
 for (int i = 0; i < BUFSIZE; i++) {
    bufchar[i] = 0;
  }
}

void ICACHE_FLASH_ATTR updateIPaddress()
{
  LocalIP = "";
  IPAddress myaddr = WiFi.localIP();
  for (int i = 0; i < 4; i++)
  { 
  LocalIP += String(myaddr[i]); 
  if (i < 3) LocalIP += ".";
  }
}

 void ICACHE_FLASH_ATTR getdeviceID()
{
  
  Serial.println();  
  //clearbufchar();
    //EepromUtil::eeprom_read_string(deviceidAddress, , BUFSIZE);
    if(deviceid[0] == 0 || EEPROM.read(0) == 21) 
    {
      
      clientName = "esp-";
      uint8_t mac[6];// = {12,34,56,78,89,34 };
      WiFi.macAddress(mac);
      
      clientName += macToStr(mac);
      clientName.toCharArray(deviceid, BUFSIZE);      
      Serial.print("No Device name in EEPROM.....Creating Device Name: ");
      Serial.println(deviceid);
      //Serial.println();
      } 
} 




void ICACHE_FLASH_ATTR restartNetworking() 
{
  networkrestart = false;
  AP_STA_timer = millis();
  APtimeout_done = false;
  LoadParams();
  getdeviceID();
  //Serial.println();
  if(EEPROM.read(ssidAddressbyte) == flagvalue) {
    
  Serial.print("Joining Wifi Network");

  WiFi.begin(ssid, password);
  //WiFi.begin();
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 40) {
    delay(500);
    i++;
    Serial.print(".");
    if (i == 39) Serial.print("Failed");
    }
    
  } else 
  { Serial.print("NO SSID specified...");
  }
  
  if(WiFi.status() != WL_CONNECTED)
  {
   Serial.println();
    Serial.print("Setting up Access Point....");
   WiFi.mode(WIFI_AP_STA);
   wifimode = 2;
   AP_STA_timer = millis();
   WiFi.softAP(deviceid);
   Serial.println(deviceid);
  } else 
  {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  }
  
  updateIPaddress();
  }


void ICACHE_FLASH_ATTR deactivateAP()
{
  //Serial.println("Deactivate AP Called");

if (WiFi.status() == WL_CONNECTED) Serial.println("Wifi Status: Connected");



 
  if( (EEPROM.read(APbyte) == flagvalue) && (WiFi.status() == WL_CONNECTED))
   {
     Serial.print("Emergency Bit...");
     if (EEPROM.read(APbyte) == flagvalue) 
     {
       EEPROM.write(APbyte,0);
       EEPROM.commit();
       Serial.println("Removed");
     } else
     {
       Serial.println("Not Set");
     }
   }
   
    if ( (wifimode == 2) && (WiFi.status() == WL_CONNECTED)) // normally set to 300000 
          {
            WiFi.mode(WIFI_STA);
            Serial.println("AP mode has been deactivated");
            wifimode = 1;
            //EEPROM.write(APbyte, 0);
            //EEPROM.commit();
          }
} 

 void ICACHE_FLASH_ATTR scannetworks()
{
  Serial.println("Scanning for Networks");
  wifinetworksfound = WiFi.scanNetworks();
} 




double ICACHE_FLASH_ATTR os_atof(const char* s)
{
	double rez = 0, fact = 1;
	while (*s && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'))
		s++;
	if (*s == 0)
		return 0; // can't read

	if (*s == '-')
	{
		s++;
		fact = -1;
	};
	for (int point_seen = 0; *s; s++)
	{
		if (*s == '.')
		{
			point_seen = 1;
			continue;
		};
		int d = *s - '0';
		if (d >= 0 && d <= 9)
		{
			if (point_seen)
				fact /= 10.0;
			rez = rez * 10.0f + (double) d;
		};
	};
	return rez * fact;
}




void ICACHE_FLASH_ATTR Save_String (char * NewValue,int writeaddress,int writeaddressbyte)

{

              //  int writeaddress = ssidAddress;  
              //int writeaddressbyte = ssidAddressbyte;        
              Serial.print("Saving new...");
              String WipeString = "";
              //char wipestring[BUFSIZE]; 
              //int j = 0;
              for (int i = 0; i < (BUFSIZE-1); i++)
              {
                WipeString += "?";
              }

              EepromUtil::eeprom_write_string(writeaddress, &WipeString[0]);
              if (EepromUtil::eeprom_write_string(writeaddress, NewValue)) 
                  { 
                    Serial.print(" ..success...");                               
                  }
                    else {
                    Serial.print(" ..fail..");
                  }
              
              if(EEPROM.read(writeaddressbyte) != flagvalue)   //  Check if check bit has been written...
                  {
                    Serial.println("Check bit written");
                    EEPROM.write(writeaddressbyte, flagvalue);
                   } else Serial.println();
              
               EEPROM.commit();
}



void uptime ()

{
    send_mqtt_msg("Uptime", String(millis()/60000));


}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void ICACHE_FLASH_ATTR EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

long ICACHE_FLASH_ATTR EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

/*
void OTA_UPDATE() {

  
  int cb = listener.parsePacket();
  
  if (cb) {
    IPAddress remote = listener.remoteIP();
    int cmd  = listener.parseInt();
    int port = listener.parseInt();
    int sz   = listener.parseInt();
    Serial.println("Got packet");
    Serial.printf("%d %d %d\r\n", cmd, port, sz);
    WiFiClient cl;
    if (!cl.connect(remote, port)) {
      Serial.println("failed to connect");
      return;
    }

    listener.stop();

    if (!ESP.updateSketch(cl, sz)) {
      Serial.println("Update failed");
    }
  }


}
*/
