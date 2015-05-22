const int PixelPIN_enablebyte = 8; 
const int PixelCount_enablebyte = 9;

const int PixelPIN_address = 204;
const int PixelCount_address = 208;



void handle_root () {

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Misc Functions</h1>\n";

  httpbuf += "<p> Heap Size = " + String(ESP.getFreeHeap()) + " </p>";
  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);
  
}

void setup_Plugin () {

  server.on("/ws2812", handle_WS2812);


    if (EEPROM.read(PixelPIN_enablebyte) == flagvalue) {

      Serial.println("PixelPIN_byte HIT");



      pixelPIN = EEPROM.read(PixelPIN_address);

      Serial.println("EEprom Read: PixelPIN = " + String(pixelPIN));
    

      if (isnan(pixelPIN)) pixelPIN = 2;
}


    
      
    if (EEPROM.read(PixelCount_enablebyte) == flagvalue) 
     {     


        pixelCount = EEPROM.read(PixelCount_address);
        //int b=EEPROM.read(PixelCount_address+1);
        //Serial.println("Reading a = " + String(a));
        //int c = 0;
        //int test = c * 256;
        //pixelCount = a*256+b;

      if (isnan(pixelCount)) pixelCount = 20;


      } 

// save value
/*
int a = value/256;
int b = value % 256;

EEPROM.write(0,a);
EEPROM.write(1,b);

// read value

int a=EEPROM.read(0);
int b=EEPROM.read(1);

value = a*256+b;


*/


  initiateWS2812();

  // timer.setInterval(30, run_animations);

  //timer.setTimeout(10000, OnceOnlyTask);

}

void OnceOnlyTask () {

	opState = ADALIGHT;
}

void loop_Plugin () {

//Adalight();
	
	//if(!strip.IsAnimating()) ws2812();
	//opState = LOOPAROUND;
	  
	   //LoopAround(192, 200); // very interesting on rings of NeoPixels
  //PickRandom(128);
   //FadeInFadeOutRinseRepeat(192);
  
  // start animating
 

 if (!strip->IsAnimating()) {
	    ws2812();
		strip->StartAnimating();
 } 
  
  
  // wait until no more animations are running
  while (strip->IsAnimating())
  {
    strip->UpdateAnimations();
    strip->Show();
    //delay(31); // ~30hz change cycle
  }

  //Serial.println("IsAnimating Time: " + String(elasped));

 
  

}