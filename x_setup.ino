
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.setDebugOutput(false);
  delay(10);
  Serial.println();
  Serial.println("Welcome to Andrew Melvin's ESP Software");
  
  EEPROM.begin(512);
  
  
  // Serial.print("Emergency Byte Value: ");
  // Serial.write(EEPROM.read(APbyte));
  Serial.println();
  
  if (EEPROM.read(APbyte) == flagvalue) 
    {
  WiFi.mode(WIFI_AP_STA);
  wifimode = 2;
  Serial.println("EMERGENCY ACCESS MODE ENABLED");
  AP_STA_timer = millis();
  Serial.print("Start time: ");
  Serial.print(AP_STA_timer /1000);
  Serial.print(" Wifi Mode: ");
  Serial.println(wifimode);

    } else
    {
    WiFi.mode(WIFI_STA);
    Serial.println("NORMAL ACCESS MODE ENABLED");
    wifimode = 1;
    } 

  scannetworks();
  delay(100);
  wifimode = 1;
 //Serial.print("Current wifi mode is : ");
 //Serial.println(WiFi.mode());

  LoadParams();


  restartNetworking();
  
   if (!mdns.begin(deviceid, WiFi.localIP())) {
    Serial.print("Error setting up MDNS responder!....(");
    
    while(1) { 
      delay(1000);
    }
  }
  Serial.print("mDNS responder started.........(");
  Serial.print(deviceid);
  Serial.println(".local)");
  
  ///// ----- Set up MQTT ------ //////
  if (MQTT_enabled) {
    Serial.println("MQTT_enable var is true");
                initiatemqqt ();
// - Send  first round of messages on boot

  
  } else {

    Serial.println("MQTT_enable var is false");

  }
  ///// ---- WEB SERVER ------/////
  server.on("/", handle_root);
  server.on("/wifi", handle_wifi);
  server.on("/bytedump",handle_bytedump);
  server.on("/mqtt", handle_mqtt);
  server.on("/io", handle_io);
  server.on("/test", handle_test);

  server.on("/misc", handle_misc);
  //server.on("/ws2812", handle_WS2812);

  //server.on("/temperature", handle_temperature);

  // Start the server 

  server.begin();

  Serial.println("HTTP server started");

    timer.setInterval(APtimeout, deactivateAP);
    timer.setInterval(MQTTtimeout, initiatemqqt);
    timer.setInterval(Uptimer_timeout, uptime);
  
  setup_Plugin ();
  Serial.println("Plugins started");
    //timer.setInterval(32,ws2812_animating);


}
