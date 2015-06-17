
void setup( void ) {
  // put your setup code here, to run once:
  EEPROM.begin(512);

  //long serialspeed = 0;

  currentspeed = EEPROM.read(SERIALspeedbyte);

  if (currentspeed == 0 || currentspeed > numberofbaudrates + 1)  {
    currentspeed = 2; 
    EEPROM.write(SERIALspeedbyte, currentspeed);
    EEPROM.commit();
  }; 


    serialspeed = baudrates[currentspeed - 1] ; 



    //currentspeed = 2;
/*
  for (int i = 0; i < numberofbaudrates; i++) {
    if(currentspeed == i) serialspeed = baudrates[i];
  }
*/
  //serialspeed = 115200;

  Serial.begin(serialspeed); // 921600 460800 115200
  //Serial.begin(115200); // 921600 460800 115200

  //Serial.setDebugOutput(true);
  //Serial.setDebugOutput(false);
  delay(10);
  Serial.println();
  Serial.println("Welcome to Andrew Melvin's ESP Software");
  
  
  
  // Serial.print("Emergency Byte Value: ");
  // Serial.write(EEPROM.read(APbyte));
  Serial.println();
  
  if (EEPROM.read(APbyte) == flagvalue) 
    {
  WiFi.mode(WIFI_AP_STA);
  wifimode = 2;
  Serial.println("EMERGENCY ACCESS MODE ENABLED");
  AP_STA_timer = millis();
  //Serial.print("Start time: ");
  //Serial.print(AP_STA_timer /1000);
  //Serial.print(" Wifi Mode: ");
  //Serial.println(wifimode);

    } else
    {
    WiFi.mode(WIFI_STA);
    Serial.println("NORMAL ACCESS MODE ENABLED");
    wifimode = 1;
    } 

  scannetworks();

  delay(2000);

  wifimode = 1;
 //Serial.print("Current wifi mode is : ");
 //Serial.println(WiFi.mode());

  LoadParams();


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


  //Serial.print(deviceid);
  //Serial.println(".local)");
  
  
  ///// ----- Set up MQTT ------ //////
  if (MQTT_enabled) initiatemqqt (); 



  ///// ---- WEB SERVER ------/////
  
  // if (wifimode == 2)  { server.on("/", handle_wifi); } else { server.on("/", handle_root); }

  server.on("/", handle_root);
  
  server.on("/wifi", handle_wifi);
  server.on("/bytedump",handle_bytedump);
  server.on("/mqtt", handle_mqtt);
  server.on("/io", handle_io);
  server.on("/test", handle_test);

  server.on("/misc", handle_misc);




  // Start the server 

  //Udp.begin(localPort);

  //Udp.beginMulticast(WiFi.localIP(), multicast_ip_addr, localPort); 

  //Udp.beginMulticast(multicast_ip_addr, WiFi.localIP() , localPort); 


  server.begin();

  //Serial.println("HTTP server started");

    timer.setInterval(APtimeout, deactivateAP);
    timer.setInterval(MQTTtimeout, initiatemqqt);

   // timer.setInterval(Uptimer_timeout, uptime);

  //Serial.println("Timers set up");

  // OTA updater...
  //listener.begin(8266);
  //
  
  setup_Plugin ();

  //Serial.println("Plugins started");
    //timer.setInterval(32,ws2812_animating);

 Serial.print("Current wifi mode is : ");
 Serial.println(wifimode);


}
