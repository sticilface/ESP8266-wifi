String cache macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 2; ++i) {
    result += String(mac[i+4], 16);
    //if (i < 5)
      //result += ':';
  }
  return result;
}

void cache clearbufchar()
{
 for (int i = 0; i < BUFSIZE; i++) {
    bufchar[i] = 0;
  }
}

void cache updateIPaddress()
{
  LocalIP = "";
  IPAddress myaddr = WiFi.localIP();
  for (int i = 0; i < 4; i++)
  { 
  LocalIP += String(myaddr[i]); 
  if (i < 3) LocalIP += ".";
  }
}

bool cache firstboot() {

if (EEPROM.read(0) != flagvalue) {
 EEPROM_wipe();
 EEPROM.write(0,flagvalue);
 EEPROM.commit();
 return true; 
} else { return false; }



}

 void cache getdeviceID()
{
  
  Serial.println();  
  //clearbufchar();
    //EepromUtil::eeprom_read_string(deviceidAddress, , BUFSIZE);
    if(deviceid[0] == 0 || EEPROM.read(0) != flagvalue) 
    {
      
      //clientName = "esp-";
      //uint8_t mac[6];// = {12,34,56,78,89,34 };
      //WiFi.macAddress(mac);
      
      //clientName += macToStr(mac);
      (getdeviceID_MAC()).toCharArray(deviceid, BUFSIZE);      

      Serial.print("No Device name in EEPROM.....Creating Device Name: ");

      Serial.println(deviceid);
      //Serial.println();
      } 
} 


String cache getdeviceID_MAC () {
      
      clientName = "esp-";
      uint8_t mac[6];// = {12,34,56,78,89,34 };
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      return clientName;
}



void  restartNetworking() 
{
  
  networkrestart = false;
  AP_STA_timer = millis();
  APtimeout_done = false;

  LoadParams();
  getdeviceID();
  //Serial.println();


  if (wifimode == 1) WiFi.mode(WIFI_STA);
delay(5); 


  if (EEPROM.read(ssidAddressbyte) == flagvalue) {
    
      //Serial.print("Joining Wifi Network (");
      //Serial.print(ssid);
      //Serial.print(",");
      //Serial.print(password);
      //Serial.print(")");
#ifdef WIFIOVERRIDE 

      WiFi.begin("fyffest", "wellcometrust");

#else 

      WiFi.begin(ssid, password);
#endif

  //WiFi.begin();
    int i = 0;
    while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    i++;
    Serial.print(".");
    if (i == 100) { Serial.print("Failed"); break; } ;
    }
    

  }   else { Serial.print("NO SSID specified...");   }
  
  
  if(WiFi.status() != WL_CONNECTED)
  {
    //Serial.println();
    //Serial.print("Setting up Access Point....");
    //WiFi.mode(WIFI_AP_STA);
    wifimode = 2;
    //AP_STA_timer = millis();
    //WiFi.softAP(deviceid);
    //WiFi.softAP((char*)(getdeviceID_MAC()).c_str(), "melvana");
    //Serial.println((char*)(getdeviceID_MAC()).c_str());
  } else  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); 
  }

  if (wifimode == 2 ) {
    //Serial.println();
    Serial.print("Setting up Access Point....");
    WiFi.mode(WIFI_AP_STA);
    delay(5); 
    AP_STA_timer = millis();
    //WiFi.softAP(deviceid);
    WiFi.softAP((char*)(getdeviceID_MAC()).c_str(), "melvana");
    Serial.println((char*)(getdeviceID_MAC()).c_str());


  }
  
  updateIPaddress();
  
  }


void cache deactivateAP()
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


 void cache scannetworks()
{
  Serial.println("Scanning for Networks");
  wifinetworksfound = WiFi.scanNetworks();

} 




double cache os_atof(const char* s)
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




void cache Save_String (char * NewValue,int writeaddress,int writeaddressbyte)

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

              eeprom_write_string(writeaddress, &WipeString[0]);
              eeprom_write_string(writeaddress, NewValue);


              //if (EepromUtil::eeprom_write_string(writeaddress, NewValue)) 
              //    { 
              //      Serial.print(" ..success...");                               
              //    }
              //      else {
              //      Serial.print(" ..fail..");
              //    }
              
              if(EEPROM.read(writeaddressbyte) != flagvalue)   //  Check if check bit has been written...
                  {
                    Serial.println("Check bit written");
                    EEPROM.write(writeaddressbyte, flagvalue);
                   } else Serial.println();
              
               EEPROM.commit();
}



void cache uptime ()

{
   // send_mqtt_msg("Uptime", String(millis()/60000));
  long time = millis(); 
 // int sec = millis() / 1000;
 // int min = sec / 60;
 // int hr = min / 60;
 // char Up_time[20]; 
 // snprintf ( Up_time, 20, "%02d:%02d:%02d", hr, min % 60, sec % 60 );

  Serial.print("MODE = (");
  Serial.print(opState);
Serial.print(",");
Serial.print(Current_Effect_State);
Serial.print(") ");

 Serial.print(" HEAP = ");
  Serial.print(ESP.getFreeHeap());
  //Serial.print();
  Serial.print(" ");
  if (animator->IsAnimating()) { 
    Serial.print("* (");
  } else {
    Serial.print("- (");
  }
  Serial.print(time);
  Serial.print(" - ");
  Serial.print(lasteffectupdate);
  Serial.print(" > ");
  Serial.print(WS2812interval);

  Serial.print(") ("); 
  if  ( (millis() - lasteffectupdate > WS2812interval) || (lasteffectupdate == 0 ) )  {
    Serial.print("Update available");
  } else {
    Serial.print("waiting");
  } ;
  Serial.println(")");






}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void cache EEPROMWritelong(int address, long value)
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

long cache EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }


void OTA_UPDATE() {

  if (OTA.parsePacket()) {

    IPAddress remote = OTA.remoteIP();

   //int packetSize; 
   char packetBuffer[100];
   //char cmd[20], port[20], size[20]; 

   OTA.read(packetBuffer, 100); 
   Serial.print("packetBuffer = ");
   Serial.print(packetBuffer);

//int spaceindex = String(packetBuffer).indexOf(" ") ;

//Serial.printf(" Space index = %u \n", spaceindex); 

String packetString = String(packetBuffer); 


int cmd = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();

packetString = packetString.substring(packetString.indexOf(' ')+1, packetString.length() );

int port = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();

packetString = packetString.substring(packetString.indexOf(' ')+1, packetString.length() );

int size = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();

//Serial.printf("CMD = %s, PORT = %s, SIZE = %s \n", cmd, port, size); 


Serial.print("CMD = ");
Serial.println(cmd);
Serial.print("PORT = ");
Serial.println(port);
Serial.print("SIZE = ");
Serial.println(size);


if ( port > 0 && size > 0) {


  Serial.println("VALID PACKET PROCEED");

    WS2812_mode_string("off"); // switch off the lights!  

    Serial.print("Update Start: ip:");
    Serial.print(remote);
    Serial.printf(", port:%d, size:%d\n", port, size);
    uint32_t startTime = millis();


    WiFiUDP::stopAll();

    if(!Update.begin(size)){
      Serial.println("Update Begin Error");
      return;
    }

    WiFiClient client;
    if (client.connect(remote, port)) {

      uint32_t written;
      while(!Update.isFinished()){
        written = Update.write(client);
        if(written > 0) client.print(written, DEC);
      }
      Serial.setDebugOutput(false);

      if(Update.end()){
        client.println("OK");
        Serial.printf("Update Success: %u\nRebooting...\n", millis() - startTime);
        ESP.restart();
      } else {
        Update.printError(client);
        Update.printError(Serial);
      }
    } else {
      Serial.printf("Connect Failed: %u\n", millis() - startTime);

    }
} else { 
  Serial.print("INVALID PACKET... CRASH PREVENTED...."); 
  OTA.flush();
}
    
  
}

}


String cache insertvariable(String Source , String insert) {

int position = Source.indexOf("%");
String one, two; 

one = Source.substring(0,Source.indexOf('%') );
two = Source.substring(Source.indexOf('%') +1 , Source.length());

return (one + insert + two); 

}

