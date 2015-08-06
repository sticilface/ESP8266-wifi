
void setup( void ) {
  // put your setup code here, to run once:

  //WiFi.disconnect();

  EEPROM.begin(512);


 bool wiped = firstboot(); 

  //long serialspeed = 0;

  currentspeed = EEPROM.read(SERIALspeedbyte);

  if (currentspeed == 0 || currentspeed > numberofbaudrates + 1)  {
    currentspeed = 2; 
    EEPROM.write(SERIALspeedbyte, currentspeed);
    EEPROM_commit_var = true;
  }; 


    serialspeed = baudrates[currentspeed - 1] ; 

  

    //currentspeed = 2;
/*
  for (int i = 0; i //< numberofbaudrates; i++) {
    if(currentspeed  i) serialspeed = baudrates[i];
  }
*/
  //serialspeed = 115200;

  Serial.begin(serialspeed); // 921600 460800 115200
  //Serial.begin(2000000); // 921600 460800 115200

#ifdef DEBUG_YES
  Serial.setDebugOutput(true);
  Debugln("YOU HAVE #DEBUG_YES defined");
#else
  Serial.setDebugOutput(false);
#endif
 
  //delay(10); //ivan said not needed anymore
  
  Serial.println();


  //Serial.print("Wiped = ");
  //Serial.print(wiped);
  //Serial.print("   FIRST BIT = ");
  //Serial.write(EEPROM.read(0));
  //Serial.println();

  if (wiped) Debugln("FIRST BOOT EEPROM WIPED"); 

  Serial.println("Welcome to Andrew Melvin's ESP Software");
  //Serial.println("OTA enabled");
  Serial.print("Compile Time: ");
  Serial.println(compile_date);
  Debug("Sketch size: ");
  Debugln(ESP.getSketchSize());
  Debug("Free size: ");
  Debugln(ESP.getFreeSketchSpace());
  
  
  
  // Serial.print("Emergency Byte Value: ");
  // Serial.write(EEPROM.read(APbyte));
  Serial.println();
  
  if (EEPROM.read(APbyte) == flagvalue) 
    {
      WiFi.mode(WIFI_AP_STA);
      wifimode = 2;
      Debugln("EMERGENCY ACCESS MODE ENABLED");
      AP_STA_timer = millis();

    } else {

    WiFi.mode(WIFI_STA);
    Debugln("NORMAL ACCESS MODE ENABLED");
    wifimode = 1;
    } 

    

  scannetworks();

  //delay(2000);

  //wifimode = 1;
 //Serial.print("Current wifi mode is : ");
 //Serial.println(WiFi.mode());

  //LoadParams();


  restartNetworking();

/*
WiFi.begin ( ssid, password );


  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
*/



//    TAKE OUT MDNS... buggy....
/*
if (wifimode == 1) {
  
   if (!mdns.begin(deviceid, WiFi.localIP())) {
    Serial.print("Error setting up MDNS responder!....(");
    
    while(1) { 
      delay(1000);
    }
}


  
  Serial.println("mDNS responder started.");

    }

*/ 

    Serial.print("Device name: ");
    Serial.println(deviceid);
  //Serial.println(".local)");
  
  
  ///// ----- Set up MQTT ------ //////
  if (MQTT_enabled) initiatemqqt (); 

Debugln("1");

  ///// ---- WEB SERVER ------/////
  
  //if (wifimode == 2)  { server.on("/", handle_wifi); } else { server.on("/", handle_root); }

  server.on("/", handle_root);
  
  server.on("/wifi", handle_wifi);
  server.on("/bytedump",handle_bytedump);
  server.on("/mqtt", handle_mqtt);
  server.on("/ota", OTAreset);
  //server.on("/test", handle_test);

  server.on("/misc", handle_misc);

  //httpupdate();  // definately NOT working yet
  

  // Start the server 

  //Udp.begin(localPort);

  //Udp.beginMulticast(WiFi.localIP(), multicast_ip_addr, localPort); 

  //Udp.beginMulticast(multicast_ip_addr, WiFi.localIP() , localPort); 


  server.begin();

  //Serial.println("HTTP server started");
    timer.setInterval(APtimeout, deactivateAP);
    timer.setInterval(MQTTtimeout, initiatemqqt);

  //  timer.setInterval(Uptimer_timeout, uptime);
  //Serial.println("Timers set up");

 //  OTA updater...
 // listener.begin(8266);
  
if(WiFi.waitForConnectResult() == WL_CONNECTED){
    MDNS.begin(deviceid);
    //MDNS.addService("arduino", "tcp", aport);
    //MDNS.addService("http", "tcp", 80);
    OTA.begin(aport);
    TelnetServer.begin();
    TelnetServer.setNoDelay(true);

  }


#ifdef HSL_FLOAT

Serial.println("HSL Floats ENABLED - to get rid of this..."); 

#endif


  
  setup_Plugin ();

  //Serial.println("Plugins started");
    //timer.setInterval(32,ws2812_animating);

 //Serial.print("Current wifi mode is : ");
 //Serial.println(wifimode);


}
