const int PixelPIN_enablebyte = 9; 
const int PixelCount_enablebyte = 10;

const int PixelPIN_address = 240;
const int PixelCount_address = 248;



void handle_root () {

  handle_WS2812();
  /*
  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Misc Functions</h1>\n";

  httpbuf += "<p> Heap Size = " + String(ESP.getFreeHeap()) + " </p>";
  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);
  */
}

void setup_Plugin () {

  server.on("/ws2812", handle_WS2812);
  server.on("/lightsconfig", handle_lights_config);

    if (EEPROM.read(PixelPIN_enablebyte) == flagvalue) {

      // Serial.println("PixelPIN_byte HIT");
      pixelPIN = EEPROM.read(PixelPIN_address);
      // Serial.println("EEprom Read: PixelPIN = " + String(pixelPIN));
      if (isnan(pixelPIN)) pixelPIN = 2;
      }

// Brightness

    CurrentBrightness = EEPROM.read(PixelCount_address + 2);

    if (isnan(CurrentBrightness)) CurrentBrightness = 255;

// Last Op State


    uint8_t c = EEPROM.read(PixelCount_address + 3);

    if (isnan(c)) c = 0;
    LastOpState = (operatingState)c;

//  Number of pixels as DOUBLE... 

    if (EEPROM.read(PixelCount_enablebyte) == flagvalue) 
     {     

        int a = EEPROM.read(PixelCount_address);
        int b = EEPROM.read(PixelCount_address+1);
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


// initiate PLUGIN


      initiateWS2812();

 

  //timer.setTimeout(100, OnceOnlyTask);

}

void OnceOnlyTask () {

	opState = ADALIGHT;
}

void loop_Plugin () {



      ws2812();


}