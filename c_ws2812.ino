

//NeoPixelBus* strip = NULL; // dynamic



enum operatingState { OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, ADALIGHT, TEST};
operatingState opState = OFF;
operatingState LastOpState = OFF;

int WS2812interval = 2000; 
// default interval...  
int WS2812timerID = -1; // make sure timer is put to off....

int spectrumValue[7];
int filter=80;


uint16_t pixelCount = 8;
uint8_t pinPixels = 2;

uint16_t effectState = 0;







void handle_WS2812 () { // handles the web commands...
 
 //Serial.println("WS2812 - Web page called.");
int CurrentBrightness = 0;
int WS2812interval = 0;
String CurrentRGBcolour = "00000";
 if (server.arg("mode").length() != 0) WS2812_mode_string(server.arg("mode"));
 if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command")); 
 if ((server.arg("dim") != String(CurrentBrightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812interval)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 if ((server.arg("rgb") != CurrentRGBcolour) && (server.arg("rgb").length() != 0)) 
  { WS2812_command_string("rgb " + server.arg("rgb"));
    //Serial.println("RGB command: " + server.arg("rgb"));
  }
 if (server.arg("rgbpicker").length() != 0) 
  { WS2812_command_string("rgb " + server.arg("rgbpicker"));
    //Serial.println("RGB picker command: " + server.arg("rgbpicker"));
  }

  if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command"));
 


  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> WS2812 </h1>\n";   httpbuf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>";
  httpbuf += "<form action='/ws2812' method='POST'>     System is:  <font size='5' color='red'> " + String(opState) + " Last Op State: " + String(LastOpState) + "  </font> ";//"   <input type='submit' name='command' value='on'>    <input type='submit' name='command' value='off'></form>"; 
  httpbuf += "<a href='/ws2812?mode=Colour'>Colour</a>  <a href='/ws2812?mode=Rainbow'>Rainbow</a>  <a href='/ws2812?mode=Fade'>Fade</a>  <a href='/ws2812?mode=ChaseRainbow'>ChaseRainbow</a> <a href='/ws2812?command=off'>off</a>  <a href='/ws2812?command=on'>on</a> <a href='/ws2812?mode=test'>TEST</a> ";
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
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);



}

void WS2812_dim_string (String Value)
{





}

void WS2812_mode_string (String Value)

{




//send_mqtt_msg( "mode", currentstate);                

}


void WS2812timer_command_string (String Value)

{



}


void WS2812_command_string (String Value)

{


}

void run_animations () {

 // wait until no more animations are running
  if (strip.IsAnimating())
  {
    strip.UpdateAnimations();
    strip.Show();
    //delay(31); // ~30hz change cycle
  }




}







void initiateWS2812 ()

{

  strip.Begin();
  strip.Show();
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
      break;
   case RAINBOW:
      break;
    case COLOR:
      break;
   case ChaseRainbow:
      //TuneP();
      break;
   case FADE:
      //TuneI();
      break;
   case ADALIGHT:
      //TuneD();
      break;
   case TEST:
      //test();
      break;
   }



}




//////// i nserted



void FadeInFadeOutRinseRepeat(uint8_t peak)
{
  if (effectState == 0)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(800,100);
      strip.LinearFadePixelColor(time, pixel, RgbColor(random(peak), random(peak), random(peak)));
    }
  }
  else if (effectState == 1)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(600,700);
      strip.LinearFadePixelColor(time, pixel, RgbColor(0, 0, 0));
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
    RgbColor color; // = strip.getPixelColor(pixel);

    color = RgbColor(random(peak), random(peak), random(peak));

    
    uint16_t time = random(100,400);
    strip.LinearFadePixelColor( time, pixel, color);
    
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
  strip.LinearFadePixelColor(speed, prevPixel, RgbColor(0, 0, 0));
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 4)) % pixelCount; 
  prevColor = strip.GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip.LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 3)) % pixelCount; 
  prevColor = strip.GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip.LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 2)) % pixelCount; 
  prevColor = strip.GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip.LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 1)) % pixelCount; 
  prevColor = strip.GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip.LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade current one light
  strip.LinearFadePixelColor(speed, effectState, RgbColor(random(peak), random(peak), random(peak)));
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








