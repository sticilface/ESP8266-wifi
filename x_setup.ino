
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

  

    //currentspeed = 2;
/*
  for (int i = 0; i //< numberofbaudrates; i++) {
    if(currentspeed  i) serialspeed = baudrates[i];
  }
*/
  //serialspeed = 115200;

  Serial.begin(serialspeed); // 921600 460800 115200
  //Serial.begin(2000000); // 921600 460800 115200
Serial.print("\n\n");

#ifdef DEBUG_YES
  Serial.setDebugOutput(true);
  Debugln("*****    DEBUG_YES defined *****");
#else
  Serial.setDebugOutput(false);
#endif




  if (wiped) Debugln("FIRST BOOT EEPROM WIPED"); 

  Serial.println("Welcome to Andrew Melvin's ESP Software");
  Serial.print("Compile Time: ");
  Serial.println(compile_date);
  Serial.println();
  
  if (EEPROM.read(APbyte) == flagvalue) 
    {
      WiFi.mode(WIFI_AP_STA);
      wifimode = 2;
      Debugln("AP MODE");
      AP_STA_timer = millis();

    } else {

    WiFi.mode(WIFI_STA);
    Debugln("NORMAL MODE");
    wifimode = 1;
    } 

    

  scannetworks();

  restartNetworking();

    Serial.print("Device name: ");
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

  //server.on("/jscolor.js", []() { server.send_P ( 200, "text/plain", PAGE_JSCOLOUR ); } );

    /* JavaScript and Stylesheets */
   // server.on ("/style.css", []() { server.send_P(200, "text/plain", PAGE_STYLE_CSS); });
   // server.on ("/microajax.js", []() { server.send_P(200, "text/plain", PAGE_MICROAJAX_JS); });


   // server.on ("/test", []() { server.send_P(200, "text/html", PAGE_ROOT); });

   // server.on("/rootvals", send_root_vals_html);


  //server.on("/test", handle_test);

  //server.on("/power", handle_power); 

  //server.serveStatic("/", SPIFFS, "/");




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

//test123b();


}
