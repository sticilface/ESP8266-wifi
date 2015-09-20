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

 Save_All_Defaults(); // sets sensible defaults...  

  EEPROM_commit_var = true;
 return true; 
} else { return false; }



}

 void cache getdeviceID()
{
  
  Serial.println();  

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


  if (wifimode == 1) WiFi.mode(WIFI_STA);
   
   delay(5); 


  if (EEPROM.read(ssidAddressbyte) == flagvalue) {
    
      WiFi.begin(ssid, password);
      uint8_t i = 0;
      while (WiFi.status() != WL_CONNECTED ) {
        delay(500);
        i++;
        Serial.print(".");
        if (i == 100) { Serial.println("Failed"); break; } ;
      }
    
  }  else { Serial.println("NO SSID specified...");   }
  
  
  if(WiFi.status() != WL_CONNECTED)
  {

    wifimode = 2;

  } else  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); 
  }

  if (wifimode == 2 ) {

    Serial.print("Setting up Access Point....");
    WiFi.mode(WIFI_AP_STA);
    delay(5); 
    AP_STA_timer = millis();
    WiFi.softAP((char*)(getdeviceID_MAC()).c_str(), "melvana");
    Serial.println((char*)(getdeviceID_MAC()).c_str());

  }
  
  updateIPaddress();
  
  }


void cache deactivateAP()
{

if (WiFi.status() == WL_CONNECTED) Serial.println("Wifi Status: Connected");

  if( (EEPROM.read(APbyte) == flagvalue) && (WiFi.status() == WL_CONNECTED))
   {
     Serial.print("Emergency Bit...");
     if (EEPROM.read(APbyte) == flagvalue) 
     {
       EEPROM.write(APbyte,0);
       EEPROM_commit_var = true;
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


              EEPROM_writeAnything( writeaddress, &WipeString[0]); 
              EEPROM_writeAnything( writeaddress, NewValue); 

              //eeprom_write_string(writeaddress, &WipeString[0]);
              //eeprom_write_string(writeaddress, NewValue);
              
              if(EEPROM.read(writeaddressbyte) != flagvalue)   //  Check if check bit has been written...
                  {
                    Serial.println("Check bit written");
                    EEPROM.write(writeaddressbyte, flagvalue);
                   } else Serial.println();
              
               EEPROM_commit_var = true;
}



// My own OTA update, that does not crash on a malformed UDP Packet
void OTA_UPDATE() {

  if (OTA.parsePacket()) {

    IPAddress remote = OTA.remoteIP();
    char packetBuffer[100];
    OTA.read(packetBuffer, 100); 

      String packetString = String(packetBuffer); 
      int cmd = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();
      packetString = packetString.substring(packetString.indexOf(' ')+1, packetString.length() );
      int port = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();
      packetString = packetString.substring(packetString.indexOf(' ')+1, packetString.length() );
      int size = (packetString.substring(0 , packetString.indexOf(' ') )).toInt();
      Debugf("CMD = %u, PORT = %u, SIZE = %u \n", cmd, port, size); 

if (cmd == 99 && port == 0 && size == 0) ESP.restart(); 

if (cmd == 100 && port == 1 && size == 1) { 
  for (int i = START_address_settings; i < 512; i++) EEPROM.write(i,0);
  EEPROM.commit(); 
  delay(2);
  ESP.restart();
}


if ( port > 0 && size > 0) {


  Debugln("VALID PACKET PROCEED");

    WS2812_mode_string("off"); // switch off the lights!  

    Serial.print("Update Start: ip:");
    Serial.print(remote);
    Debugf(", port:%d, size:%d", port, size);
    Serial.println(); 
    uint32_t startTime = millis();


    WiFiUDP::stopAll();

    if(!Update.begin(size)){
      Serial.println("Update Begin Error");
      OTA.begin(aport); // resume listenting.. 
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
        OTA.begin(aport); // resume listening
        ESP.restart();

      }
    } else {
      Serial.printf("Connect Failed: %u\n", millis() - startTime);
      OTA.begin(aport); // resume listening
    }
} else { 
  Serial.print("INVALID PACKET... CRASH PREVENTED...."); 
  OTA.flush();
}
    
  
}

}



void cache OTAreset() {
    

    WiFiUDP::stopAll();
    //delay(5);
    OTA.begin(aport); // resume listenting.. 
    Debugln("OTA UDP restarted"); 
    server.send(200, "text", "DONE");


}


String cache insertvariable(String Source , String insert) {

    String one, two; 
    one = Source.substring(0,Source.indexOf('%') );
    two = Source.substring(Source.indexOf('%') + 1 , Source.length());
    return (one + insert + two); 
}



