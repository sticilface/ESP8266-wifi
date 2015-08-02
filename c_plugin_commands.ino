


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

uint8_t temp,tempb; 
/// LOAD FIRST ROW OF PIXEL SETTINGS..... 

  // PIN, Pixel COUNT, Last OP State  

    if (EEPROM.read(PixelPIN_enablebyte) == flagvalue) {
      pixelPIN = EEPROM.read(PixelPIN_address);
      if (isnan(pixelPIN)) pixelPIN = 2;
      }


// Last Op State

//
//     temp = EEPROM.read(Pixel_LastOpState);
//
//    if (isnan(temp)) temp = 0;
//    LastOpState = (operatingState)temp;

//  Number of pixels as DOUBLE... 

    if (EEPROM.read(PixelCount_enablebyte) == flagvalue) 
     {     

         temp = EEPROM.read(PixelCount_address);
         tempb = EEPROM.read(PixelCount_address+1);

        if(isnan(temp)) temp = 0;
        if(isnan(tempb)) tempb = 0;

        pixelCount = temp*256+tempb;

       // Serial.print("Loaded pixels = (");
       // Serial.print(temp);
       // Serial.print(",");
       // Serial.print(tempb);
       // Serial.print(") = ");
       // Serial.println(pixelCount); 


      if (pixelCount > 300 || pixelCount == 0) pixelCount = 20;


      } 

      //Serial.print("/n Saved Colours are: ");

// SAVED SETTINGS..... 

Load_LED_Defaults(0); 

// initiate PLUGIN


initiateWS2812();

bool CurrentRestartValue = EEPROM.read(AutoRestartEffectAddress);
bool Last = EEPROM.read(ON_OFF_State_Address); 


if ( CurrentRestartValue == true ) 
    { 
       if (Last == true) { 
          WS2812_mode_string("on");
          Serial.println("MODE returned to ON");
        } else {
          WS2812_mode_string("off");
          Serial.println("MODE returned to OFF");
        }
    }





  timer.setTimeout(10000, SendMQTTsettings);

 //     HoldingOpState = SQUARES2; 
 //   Current_Effect_State = POST_EFFECT;

}

// PixelCount_address + 0 = 248 =  Number of Pixels (double)
// PixelCount_address + 1 = 249 =  Number of pixels (double)
// PixelCount_address + 2 = 250 =  
// PixelCount_address + 3 = 251 =  LastOpState
// PixelCount_address + 4 = 252 =  RED
// PixelCount_address + 5 = 253 =  GREEN
// PixelCount_address + 3 = 254 =  BLUE

/// START SETTINGS AT 256...

// order of bytes is......  

// 



void SendMQTTsettings () {
  
  //Serial.println("Once only task hit"); 
  String message = "effect:"; 

  for (uint8_t i = 0; i < numberofmodes; i++)  {
    message += String(MODE_STRING[i]); 
    if (i < (numberofmodes - 1)) { message += ","; }; 
   }

    send_mqtt_msg("effectlist", message); 

    //delay(10);

    send_current_settings(); 


  //  HoldingOpState = SQUARES2; 
  // Current_Effect_State = POST_EFFECT;



  
}





void loop_Plugin () {

      ws2812();


}