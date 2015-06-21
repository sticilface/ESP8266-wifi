const int PixelPIN_enablebyte = 9; 
const int PixelCount_enablebyte = 10;

const int PixelPIN_address = 240;
const int PixelCount_address = 248;



void cache handle_root () {

if (WiFi.status() == WL_CONNECTED)  { 
  handle_WS2812(); 
 } else {
  handle_wifi();
  }

/*
  Serial.println();
  Serial.print("handle root hit..."); 
  //handle_wifi();
  
  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> SETUP DEVICE</h1>\n";

  httpbuf += "<p> Heap Size = " + String(ESP.getFreeHeap()) + " </p>";
  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

//////////////////////////
  
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, 400,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='30'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

    hr, min % 60, sec % 60
  );

  server.send ( 200, "text/html", temp );

  Serial.println();
  Serial.print(hr);
  Serial.print(":");
  Serial.print(min % 60);
  Serial.print(":");
  Serial.print(sec % 60);
  Serial.print(":");  


    Serial.print("finshed!");

/////////////////////////////////

  //server.send(200, "text/html", httpbuf);
  */
}

void cache setup_Plugin () {

  server.on("/ws2812", handle_WS2812);
  
  server.on("/lightsconfig", handle_lights_config);

    if (EEPROM.read(PixelPIN_enablebyte) == flagvalue) {

      // Serial.println("PixelPIN_byte HIT");
      pixelPIN = EEPROM.read(PixelPIN_address);
      // Serial.println("EEprom Read: PixelPIN = " + String(pixelPIN));
      if (isnan(pixelPIN)) pixelPIN = 2;
      }

// Brightness

    if (isnan(CurrentBrightness) || CurrentBrightness == 0) CurrentBrightness = 255;

// Last Op State


    uint8_t c = EEPROM.read(PixelCount_address + 3);

    if (isnan(c)) c = 0;
    LastOpState = (operatingState)c;

//  Number of pixels as DOUBLE... 

    if (EEPROM.read(PixelCount_enablebyte) == flagvalue) 
     {     

        uint8_t a = EEPROM.read(PixelCount_address);
        uint8_t b = EEPROM.read(PixelCount_address+1);
        if(isnan(a)) a = 0;
        if(isnan(b)) b = 0;
        pixelCount = a*256+b;

      if (pixelCount > 600) pixelCount = 20;


      } 

      //Serial.print("/n Saved Colours are: ");

// SAVED COLOURS

      uint8_t R = EEPROM.read(PixelCount_address + 4);
      uint8_t G = EEPROM.read(PixelCount_address + 5);
      uint8_t B = EEPROM.read(PixelCount_address + 6);

      NewColour = RgbColor(R,G,B);

      WebRGBcolour = String(R,HEX);
      WebRGBcolour += String(G,HEX);
      WebRGBcolour += String(B,HEX);
     


// initiate PLUGIN


      initiateWS2812();

 

  timer.setTimeout(10000, OnceOnlyTask);

}

void OnceOnlyTask () {

  //Serial.println("Once only task hit"); 
  String message = "effect:"; 

  for (uint8_t i = 0; i < numberofmodes; i++)  {
    message += String(MODE_STRING[i]); 
    if (i < (numberofmodes - 1)) { message += ","; }; 
   }

   send_mqtt_msg("effectlist",message); 


   delay(10);

   if (opState == OFF) { 
    send_mqtt_msg("mode","off");
    send_mqtt_msg("effect","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }

    delay(100);
    send_mqtt_msg("timer", String(WS2812interval));
    send_mqtt_msg("brightness", String(CurrentBrightness));
  
}

void loop_Plugin () {



      ws2812();


}