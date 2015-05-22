

//NeoPixelBus* strip = NULL; // dynamic


String CurrentRGBcolour; // This is for the WEBPAGE... takes the value when colour is changed...

int lasteffectupdate; 
int WS2812interval = 2000; 
//int CurrentBrightness = 255; 
int WS2812timerID = -1; // make sure timer is put to off....

int spectrumValue[7];
int filter=80;




uint16_t effectState = 0;

RgbColor NewColour;


uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;



void handle_WS2812 () { // handles the web commands...
 boolean updateLEDs = false;
 //Serial.println("WS2812 - Web page called.");
 //= 0;
//String CurrentRGBcolour = "00000";
 if (server.arg("mode").length() != 0) WS2812_mode_string(server.arg("mode"));
 if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command")); 
 if ((server.arg("dim") != String(CurrentBrightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812interval)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 if (server.arg("rgbpicker").length() != 0) 
  { 
    WS2812_mode_string("rgb-" + server.arg("rgbpicker"));

    Serial.println("RGB picker command: " + server.arg("rgbpicker"));
  }

  if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command"));

  if (server.arg("leds").length() != 0) {
    pixelCount = server.arg("leds").toInt();
    updateLEDs = true;
  }

   if (server.arg("ledpin").length() != 0) {
    pixelPIN = server.arg("ledpin").toInt();
    updateLEDs = true;
  }
 


  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> WS2812 </h1>\n";   httpbuf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>";
  httpbuf += "<form action='/ws2812' method='POST'>     System is:  <font size='5' color='red'> " + String(opState) + " Last Op State: " + String(LastOpState) + "  </font> ";//"   <input type='submit' name='mode' value='on'>    <input type='submit' name='command' value='off'></form>"; 
  httpbuf += "<br> <a href='/ws2812?mode=off'>off</a>  <a href='/ws2812?mode=on'>on</a> ";
  httpbuf += "<br> <a href='/ws2812?mode=Colour'>Colour</a>  <a href='/ws2812?mode=Rainbow'>Rainbow</a>  <a href='/ws2812?mode=Fade'>Fade</a>  <a href='/ws2812?mode=ChaseRainbow'>ChaseRainbow</a>  <a href='/ws2812?mode=test'>TEST</a> ";
  httpbuf += "<br> <a href='/ws2812?mode=fadeinfadeout'>FadeInFadeOut</a> <a href='/ws2812?mode=pickrandom'>PickRandom</a> <a href='/ws2812?mode=looparound'>LoopAround</a>";
  httpbuf += "<br> <a href='/ws2812?mode=adalight'>Adalight</a>" ; 
  httpbuf += "<p><form name=frmTest action='/ws2812' method='POST'>\n";
  httpbuf += "<p> Select Mode <select name='mode' onChange='frmTest.submit();'>";
  httpbuf += "<option value='Colour'>Colour</option>";
  httpbuf += "<option value='Rainbow'>Rainbow</option>";
  httpbuf += "<option value='Fade'>Fade</option>";
  httpbuf += "<option value='ChaseRainbow'>ChaseRainbow</option>";
  httpbuf += "</select>";
  httpbuf += "</form></p>";
  httpbuf += "<p><form action='/ws2812' method='POST'";
  httpbuf += "<p>HEX Color CODE: <input class='color' name='rgbpicker' value = '" + CurrentRGBcolour + "'>";
  httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 
  httpbuf += "<p><form name=sliders action='/ws2812' method='POST'>\n";
  httpbuf += "<p>Brightness: <input type='range' name='dim'min='0' max='255' value='" + String(CurrentBrightness) + "'  > ";
  httpbuf += "<p>Timer: <input type='range' name='timer'min='0' max='2000' value='"+ String(WS2812interval)+ "' > ";
  httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 
  httpbuf += "<p><form action='/ws2812' method='POST'";
  httpbuf += "<form action='/ws2812' method='POST'>";    
  httpbuf += "\n\nNumber of LEDs: <input type='text' id='leds' name='leds' value='"+ String(pixelCount) + "'>";
  httpbuf += "\n\nPIN: <input type='text' id='ledpin' name='ledpin' value='"+ String(pixelPIN) + "'>";
  httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);

if (updateLEDs) { initiateWS2812(); updateLEDs = false;};

}

void WS2812_dim_string (String Value)
{

/*
  RgbColor prevColor;

      int dim = Value.toInt();
      if (dim > 255) dim = 255;
      if (dim < 0) dim = 0;

      int DiffDim = CurrentBrightness - dim;
      CurrentBrightness  = dim; 

      if (DiffDim > 0) {  
        Serial.println("Brightness Darken by: " + String(DiffDim));

        for (int i = 0; i < pixelCount; i++) {
          //prevColor = strip->GetPixelColor(i);
          prevColor.Darken(DiffDim);
          ////strip->SetPixelColor(i,prevColor);
        }
      } else if (DiffDim < 0) { 
        DiffDim = DiffDim * -1;
        Serial.println("Brightness Lighten by: " + String(DiffDim));
          for (int i = 0; i < pixelCount; i++) {
          //prevColor = strip->GetPixelColor(i);
          prevColor.Lighten(DiffDim);
          //strip->SetPixelColor(i,prevColor);       
        }
      } 

*/
//  
      int a = Value.toInt();
      if (a > 255) a = 255;
      if (a < 0) a = 0;

      CurrentBrightness  = a;

      Serial.println("Brightness set to: " + String(CurrentBrightness));


          //strip->Show();

}

void WS2812_mode_string (String Value)

{
  Serial.print("MODE recieved: " + Value);
  Value.toLowerCase();

  if (Value == "off" | Value == "OFF") { if (opState != OFF) { LastOpState = opState; opState = OFF; } ;};
  if (Value == "on" | Value == "ON") opState = LastOpState;
  if (Value == "looparound") { opState = LOOPAROUND; LastOpState = LOOPAROUND;};
  if (Value == "pickrandom") { opState = PICKRANDOM; LastOpState = PICKRANDOM; } ;
  if (Value == "fadeinfadeout") { opState = FADEINFADEOUT; LastOpState = FADEINFADEOUT; }; 
  if (Value == "fade") { opState = FADE; LastOpState = FADE; }; 
  if (Value == "rainbow") { opState = RAINBOW; LastOpState = RAINBOW; }; 
  if (Value == "adalight") { LastOpState = opState; opState = ADALIGHT;  }; 
  if (Value == "coolblobs") { opState = COOLBLOBS; LastOpState = COOLBLOBS;  }; 

  if (Value.indexOf("rgb") >= 0) 
    {
      opState = COLOR;
      String instruction = Value.substring(4,Value.length()+1 );
      Serial.println("RGB command recieved: " + instruction);
      CurrentRGBcolour = instruction;
      NewColour = HEXtoRGB(instruction);
      SetRGBcolour(NewColour);

  }

 
//send_mqtt_msg( "mode", currentstate);                

}


void WS2812timer_command_string (String Value)

{

WS2812interval = Value.toInt();

}


void WS2812_command_string (String Value) {
}
/*

if (Value.indexOf("rgb") >= 0) 
{
  opState = COLOR;
  String instruction = Value.substring(4,Value.length()+1 );
  Serial.println("RGB command recieved: " + instruction);
  NewColour = HEXtoRGB(instruction);
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {
      //strip->LinearFadePixelColor(1000, pixel, NewColour);
    }
}

} */


RgbColor dim(RgbColor value) {
    int amounttodarken = 255 - CurrentBrightness;
    value.Darken(amounttodarken);
    return value;
}

void SetRGBcolour (RgbColor value) {
    //RgbColor Newvalue = dim(value);
  value = dim(value);
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {
        strip->SetPixelColor(pixel,value);
    }

ApplyPixels();

}

RgbColor HEXtoRGB (String hexString) // Converts HEX to RBG object....

{

  uint32_t decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
    int r = decValue >> 16;
    int g = decValue >> 8 & 0xFF;
    int b = decValue & 0xFF;

    return RgbColor(r,g,b);

}

void StripOFF() {

  for (int i = 0; i < pixelCount; i++)
  {
    strip->SetPixelColor(i,0);
  }
  strip->Show();
;

}



void initiateWS2812 ()

{
opState = OFF;
  ChangeNeoPixels(pixelCount, pixelPIN); // initial setup

  strip->Begin();
  StripOFF();
  
  // 

  SetRandomSeed();

}




////////////////////////




void ws2812 ()  // put switch cases here...


{

  //Serial.print("ws2812 called case = " + String(opState));
  //wsPoint += 1;

switch (opState)
   {
   case OFF:
      //if (strip) StripOFF();
   StripOFF();
      break;
   case RAINBOW:
      rainbow();
      break;
   case COLOR:
      SetRGBcolour(NewColour);
      break;
   case ChaseRainbow:
      //TuneP();
      break;
   case FADE:
      Fade();
      break;
   case ADALIGHT:
      Adalight();
      break;
   case TEST:
      //test();
      break;
   case LOOPAROUND:
      LoopAround(192, 200);
      break;
   case PICKRANDOM:
      PickRandom(128);
      break;
   case FADEINFADEOUT:
      FadeInFadeOutRinseRepeat(192);
      break;
     case COOLBLOBS:
      CoolBlobs();
      break;    
   }



}

void CoolBlobs() {

    if (millis() > (lasteffectupdate + WS2812interval) ){
      





    } // end of timer if

} // end of coolblobs


//////// i nserted
/*

void setcolour () {

    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {

      //strip->LinearFadePixelColor(5000, pixel, NewColour);

    }

}

*/

void FadeInFadeOutRinseRepeat(uint8_t peak)
{
  if (effectState == 0)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(800,100);
      strip->LinearFadePixelColor(time, pixel, RgbColor(random(peak), random(peak), random(peak)));
    }
  }
  else if (effectState == 1)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(600,700);
      strip->LinearFadePixelColor(time, pixel, RgbColor(0, 0, 0));
    }
  }
  effectState = (effectState + 1) % 2; // next effectState and keep within the number of effectStates
  
}

void PickRandom(uint8_t peak)
{

  // pick random set of pixels to animate
  uint8_t count = random(pixelCount);
  while (count > 0)
  {
    uint8_t pixel = random(pixelCount);
    
    // configure the animations
    RgbColor color; // = //strip->getPixelColor(pixel);

    color = RgbColor(random(peak), random(peak), random(peak));

    
    uint16_t time = random(100,400);
    strip->LinearFadePixelColor( time, pixel, color);
    
    count--;
  }
}

void LoopAround(uint8_t peak, uint16_t speed)
{
  // Looping around the ring sample
  uint16_t prevPixel;
  RgbColor prevColor;
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 5)) % pixelCount; 
  strip->LinearFadePixelColor(speed, prevPixel, RgbColor(0, 0, 0));
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 4)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 3)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 2)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 1)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade current one light
  strip->LinearFadePixelColor(speed, effectState, RgbColor(random(peak), random(peak), random(peak)));
  effectState = (effectState + 1) % pixelCount;
  
}

void SetRandomSeed()
{
  uint32_t seed;
  
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);
  
  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }
  
  // Serial.println(seed);
  randomSeed(seed);
}


RgbColor Wheel (byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return  RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
   WheelPos -= 85;    
   return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return  RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
} 


void Fade() {

static int j;
if (millis() > (lasteffectupdate + WS2812interval) ){

  Serial.println("Fade updaed");
if (j > 256) j = 0; 
RgbColor col = Wheel(j);
//int col = 200; 

for (int i=0; i < pixelCount; i++) {
    strip->SetPixelColor(i, dim(col));    //turn every third pixel on
        }

ApplyPixels();

j++;
lasteffectupdate = millis();

}

}

void ApplyPixels () {
   



    strip->Show();
}

 void rainbow() {

 if (millis() > (lasteffectupdate + WS2812interval) ){

  static int wsPoint ;
  uint16_t i; // , j;
  //pixelsNUM = 60;
  //for(j=0; j<256; j++) { v
    for(i=0; i<pixelCount; i++) {
    //RgbColor tempcolour = Wheel(i+wsPoint);
    strip->SetPixelColor(i, dim(Wheel(i+wsPoint)));
    }
    ApplyPixels();
      if (wsPoint==256) wsPoint=0; 
    wsPoint++;
    lasteffectupdate = millis();
}
    // Serial.println("Colours Updated..." + String(//strip->numPixels()));

}   // END OF 


void Adalight () { //  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
static boolean Adalight_configured;

 if (!Adalight_configured) {
    delay(500);
    for(i=0; i<pixelCount; i++) {
      strip->SetPixelColor(i, RgbColor(255,0,0));
    }
    strip->Show();
;
    delay(500);
    for(i=0; i<pixelCount; i++) {
      strip->SetPixelColor(i, RgbColor(0,255,0));
    }
    strip->Show();
;
    delay(500);
    for(i=0; i<pixelCount; i++) {
      strip->SetPixelColor(i, RgbColor(0,0,255));
    }
    strip->Show();
    Serial.println();
    Serial.print("Ada\n"); // Send "Magic Word" string to host
    Adalight_configured = true;
    }

 //if(Serial.read() > 0 ) Serial.println("Serial Contect Recieved...");

/*

    if(Serial.available()) {
          Serial.println("Serial Contect Recieved...");
          for(i = 0; i < sizeof prefix; ++i) {
            if(prefix[i] == Serial.read()) 
                { 
                  Serial.println("Prefix found");   
                      } 
                }

          }
        
*/
    
/*






    for(i = 0; i < sizeof prefix; ++i) {
        waitLoop: while (!Serial.available()) ;;
        // Check next byte in Magic Word
        if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
    }

 while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // if checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55))
  {
    i=0;
    goto waitLoop;
  }


  for (uint8_t i = 0; i < pixelCount; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
   
    //strip->SetPixelColor(i, RgbColor(r,g,b));

  } */ 


//strip->Show();




}


// called to change the number of pixels
void ChangeNeoPixels(uint16_t count, uint8_t pin)
{
  
 bool commitchanges = false; 

    Serial.println("Change Neopixels called"); 


        int pixelPINstored = EEPROM.read(PixelPIN_address);    
        int pixelCountstored = EEPROM.read(PixelCount_address);
        //int b=EEPROM.read(PixelCount_address+1);
        //nt pixelCountstored = a*256+b;


    if (count != pixelCountstored) {
    Serial.println("Pixel count changed..."); 

      //int a = pixelCount/256;
      //Serial.print(a);
      //int b = pixelCount % 256;
      //Serial.print(b);
        EEPROM.write(PixelCount_address,(byte)count);
        //EEPROM.write(PixelCount_address+1,b);
        commitchanges = true;

    if (EEPROM.read(PixelCount_enablebyte) != flagvalue) EEPROM.write(PixelCount_enablebyte,flagvalue) ;
     Serial.println("pixel count byte updated");
    }

    if (pin != pixelPINstored) {
    Serial.println("Change Neopixels PIN called"); 

        EEPROM.write(PixelPIN_address, (byte)pin);
    if (EEPROM.read(PixelPIN_enablebyte) != flagvalue) EEPROM.write(PixelPIN_enablebyte,flagvalue) ;
     
     commitchanges = true;

    }

    if (commitchanges == true) {
      EEPROM.commit();
          Serial.println("Changes committed"); 
        }


    if (strip)
    {
        //StripOFF();

        delete strip;
    }

    strip = new NeoPixelBus(count, pin);
}
