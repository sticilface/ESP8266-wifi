
void setup( void ) {
  // put your setup code here, to run once:

  //WiFi.disconnect();

  EEPROM.begin(512);
  
  //SPIFFS.begin();


 bool wiped = firstboot(); 

  //long serialspeed = 0;

  currentspeed = EEPROM.read(SERIALspeedbyte);

  if (currentspeed == 0 || currentspeed > numberofbaudrates + 1)  {
    currentspeed = 2; 
    EEPROM.write(SERIALspeedbyte, currentspeed);
    EEPROM_commit_var = true;
  }; 


    serialspeed = baudrates[currentspeed - 1] ; 

  

  Serial.begin(serialspeed);

  Serial.print(F("\n\n"));

#ifdef DEBUG_YES
  Serial.setDebugOutput(true);
  Debugln(F("*****    DEBUG_YES defined *****"));
#else
  Serial.setDebugOutput(false);
#endif




  if (wiped) Debugln(F("FIRST BOOT EEPROM WIPED")); 

  Serial.println(F("Welcome to Andrew Melvin's ESP Software"));
  Serial.print(F("Compile Time: "));
  Serial.println(compile_date);
  Serial.println();
  
  if (EEPROM.read(APbyte) == flagvalue) 
    {
      WiFi.mode(WIFI_AP_STA);
      wifimode = 2;
      Debugln(F("AP MODE"));
      AP_STA_timer = millis();

    } else {

    WiFi.mode(WIFI_STA);
    Debugln(F("NORMAL MODE"));
    wifimode = 1;
    } 

    

  scannetworks();

  restartNetworking();

    Serial.print(F("Device name: "));
    Serial.println(deviceid);
  
  
  ///// ----- Set up MQTT ------ //////
  if (MQTT_enabled) initiatemqqt (); 


  ///// ---- WEB SERVER ------/////
  

  server.on("/", handle_root);  
  server.on("/wifi", handle_wifi);
  server.on("/bytedump",handle_bytedump);
  server.on("/mqtt", handle_mqtt);
  server.on("/ota", OTAreset);
  server.on("/misc", handle_misc);


  buf.reserve(2048);
  
  //httpupdate();  // definately NOT working yet



  server.begin();
  
  timer.setInterval(APtimeout, deactivateAP);
  timer.setInterval(MQTTtimeout, initiatemqqt);


  
if(WiFi.waitForConnectResult() == WL_CONNECTED){

#ifdef MDNSSERVICE  
    MDNS.begin(deviceid);
    MDNS.addService("arduino", "tcp", aport);
    MDNS.addService("http", "tcp", 80);
#endif

    OTA.begin(aport);
    TelnetServer.begin();
    TelnetServer.setNoDelay(true);

  }


  setup_Plugin ();


}
