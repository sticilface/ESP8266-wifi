#ifdef WS2812_DEVICE

NeoPixelBus* strip = NULL; // dynamic

//Adafruit_ILI9341 tft = Adafruit_ILI9341();


enum operatingState { OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, ADALIGHT, AUTO};
operatingState opState = OFF;
operatingState LastOpState = OFF;

int WS2812interval = 2000; // default interval...  
int WS2812timerID = -1; // make sure timer is put to off....

int spectrumValue[7];
int filter=80;
uint32_t NewColor;
String CurrentRGBcolour;
int CurrentBrightness = 255;
//char pixels[24 * 3];
int DiffDim;

//#define resetPin 5
//#define strobePin 4
//#define WS2812PIN 2

//ESP_WS2812 strip = ESP_WS2812(16, WS2812PIN, NEO_GRB + NEO_KHZ800);

uint16_t countPixels = 8;
uint8_t pinPixels = 2;
uint16_t effectState = 0;
boolean Adalight_enabled;
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;


void handle_WS2812 () { // handles the web commands...
//Serial.println("WS2812 - Web page called.");
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
 


httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> WS2812 </h1>\n";
  //httpbuf += "<p>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  //httpbuf += "<br>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
   httpbuf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>";
  httpbuf += "<form action='/ws2812' method='POST'>     System is:  <font size='5' color='red'> " + String(opState) + " Last Op State: " + String(LastOpState) + "  </font> ";//"   <input type='submit' name='command' value='on'>    <input type='submit' name='command' value='off'></form>"; 
 
 //<option selected>

//Serial.println("Stored Color variable: " + CurrentRGBcolour);
//Serial.println("Stored brightness: " + String(CurrentBrightness));
//Serial.println("Stored Interval: " + String(WS2812interval));

// + ". Status is " + mqttconnected ;
  httpbuf += "<a href='/ws2812?mode=Colour'>Colour</a>  <a href='/ws2812?mode=Rainbow'>Rainbow</a>  <a href='/ws2812?mode=Fade'>Fade</a>  <a href='/ws2812?mode=ChaseRainbow'>ChaseRainbow</a> <a href='/ws2812?command=off'>off</a>  <a href='/ws2812?command=on'>on</a>";
  httpbuf += "<p><form name=frmTest action='/ws2812' method='POST'>\n";
  httpbuf += "<p> Select Mode <select name='mode' onChange='frmTest.submit();'>";
  httpbuf += "<option value='Colour'>Colour</option>";
  httpbuf += "<option value='Rainbow'>Rainbow</option>";
  httpbuf += "<option value='Fade'>Fade</option>";
  httpbuf += "<option value='ChaseRainbow'>ChaseRainbow</option>";
  httpbuf += "</select>";
  httpbuf += "</form></p>";
  httpbuf += "<p><form action='/ws2812' method='POST'";

  //httpbuf += "HEX Color CODE: <input type='color' name='rgb'>";
  httpbuf += "<p>HEX Color CODE: <input class='color' name='rgbpicker' value = '" + CurrentRGBcolour + "'>";
  //httpbuf += "<br>  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 

  httpbuf += "</form></p>"; 

  httpbuf += "<p><form name=sliders action='/ws2812' method='POST'>\n";
  httpbuf += "<p>Brightness: <input type='range' name='dim'min='0' max='255' value='" + String(CurrentBrightness) + "'  > ";
  httpbuf += "<p>Timer: <input type='range' name='timer'min='0' max='2000' value='"+ String(WS2812interval)+ "' > ";

  //httpbuf += "\n\nTemp: <input type='text' id='setpoint' name='setpoint' value='" + String(Setpoint) + "'><br>";
  //httpbuf += "\n\nKp: <input type='text' id='kp' name='kp' value='" + String(Kp) + "'>  (850)<br>";
  //httpbuf += "\n\nKi: <input type='text' id='kp' name='ki' value='" + String(Ki) + "'>  (0.5)<br>";
  //httpbuf += "\n\nKd: <input type='text' id='kp' name='kd' value='" + String(Kd) + "'>  (0.1)<br>";  
  //httpbuf += "\n\nWindow: <input type='text' id='window' name='window' value='" + String(WindowSize) + "'>  (10000)<br>";    
  // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
    httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 

  httpbuf += "</form></p>"; 

  //httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  //httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);



}

void WS2812_dim_string (String Value)
{
  //RgbColor prevColor;

int dim = Value.toInt();
if (dim > 255) dim = 255;
if (dim < 0) dim = 0;

DiffDim = 255 - dim;
CurrentBrightness  = dim; 

//if (diff > 0) {
  //diff = diff * -1;
/*for (int i; i < countPixels; i++)
{
  prevColor = strip->GetPixelColor(i);
  prevColor.Darken(diff);
  strip->SetPixelColor(i,prevColor);
}

} else if (diff > 0) {

;



} */ 

/*
RgbColor test = RgbColor(dim);
CurrentBrightness  = dim; 
//strip.setBrightness(dim);

  for(int i=0;i<countPixels; i++) {
    strip->SetPixelColor(i,test);
  } */

strip->Show();

Serial.print("Brightness updated to :" + Value);
//send_mqtt_msg( "Brightness", Value);                


}

void WS2812_mode_string (String Value)

{
String currentstate;
if (Value == "Rainbow") { opState = RAINBOW; currentstate = "Rainbow";}
if (Value == "Colour") { opState = COLOR; currentstate = "Colour"; }
if (Value == "ChaseRainbow")  {opState = ChaseRainbow; currentstate = "ChaseRainbow"; }
if (Value == "Fade")  {opState = FADE; currentstate = "Fade"; }


if (Value != "off" && WS2812timerID == -1) {
  WS2812timerID = timer.setInterval(WS2812interval, ws2812);
  //Serial.println("WS2812timer Enabled ID: " + String(WS2812timerID));
  //WS2812_command_string (Value);
}

if (Value == "off") 
{

  if (opState != OFF) LastOpState = opState;

  opState = OFF;
  //stripoff();

  if (Value == "on")
{
 opState = LastOpState;

}

}
ws2812(); //just call the statemachine to update state ASAP...

//send_mqtt_msg( "mode", currentstate);                

}


void WS2812timer_command_string (String Value)

{

WS2812interval = Value.toInt();
if (WS2812timerID > -1) {
  timer.deleteTimer(WS2812timerID);
  WS2812timerID = timer.setInterval(WS2812interval, ws2812);
  Serial.println("WS2812 Timer change to: " + String(WS2812interval));
} else {
  Serial.println("Timer not active, but WS2812interval has been changed");
}

}


void WS2812_command_string (String Value)

{
//Serial.println("WS2812 command: " + Value);
//Serial.println("Current OpState = " + String(opState));
//Serial.println("Last OpState = " + String(LastOpState));

//Serial.print("Index of RGB: " + String(Value.indexOf("rgb")));
//Serial.println("PRE WS2812timerID: " + String(WS2812timerID));

if (Value != "off" && WS2812timerID == -1) {
  WS2812timerID = timer.setInterval(WS2812interval, ws2812);
  //Serial.println("WS2812timer Enabled ID: " + String(WS2812timerID));
}


if (Value.indexOf("rgb") >= 0) 
{
  opState = COLOR;
  //send_mqtt_msg( "mode", "Colour");                

  String instruction = Value.substring(4,Value.length()+1 );
  CurrentRGBcolour = instruction;
  //Serial.println("RGB command recieved: " + instruction);
  //Serial.print("Example of colour in 32bit: ");
  //Serial.println(strip.getPixelColor(1));
  //uint32_t 
  NewColor = hexToDec(instruction);

  //color(NewColor);


}

if (Value == "off") 
{

  if (opState != OFF) LastOpState = opState;

  opState = OFF;
  //stripoff();
}

if (Value == "on")
{
 opState = LastOpState;

}

if (Value == "toggle")
{
if (opState == OFF ) {
  opState = LastOpState;
} else if (opState != OFF)
{
  LastOpState = opState;
  opState = OFF;
}



}


ws2812(); //just call the statemachine to update state ASAP...

}






void initiateWS2812 ()

{

   //strip.begin();
   //strip->clear();
   //strip.show(); 
   //strip.show(); 

   Serial.println("WS2812  started");

  ChangeNeoPixels(countPixels, pinPixels); // initial setup
   
     // ---  subscribe to all esp messages 
  strip->Show();
      //ws2812_int.attach_ms(100, show);


}

uint32_t StripColor; 

void color(uint32_t color)

{
  StripColor = color;
int r = StripColor >> 16;
int g = StripColor >> 8 & 0xFF;
int b = StripColor & 0xFF;

   for(int i=0;i<countPixels; i++) {
    RgbColor NewColor = RgbColor(r,g,b);
    NewColor.Darken(DiffDim);
    strip->SetPixelColor(i,NewColor);
  } 
      //strip.setPixelColor(0,0);

  strip->Show();

}

//void color(String color)

//{
  //Serial.println("Colour = " + color);
  //String r = color.substring(0,2 );
  //String g = color.substring(2,4 );
  //String b = color.substring(4,6 );



  //Serial.println("NEW colour as INT: (" + r + "," + g + "," + b + ")");
  //uint16_t i; // , j;


  //color(NewColor)


//}

 void rainbow() {
 
  
  static int wsPoint ;
  uint16_t i; // , j;
  //pixelsNUM = 60;
  //for(j=0; j<256; j++) { v
    for(i=0; i<countPixels; i++) {
      strip->SetPixelColor(i, Wheel((i+wsPoint) & 255));
    }
    //strip.setPixelColor(0,0);
    strip->Show();
    //delay(wait);
  //}
    if (wsPoint==256) wsPoint=0; 
    wsPoint++;
    // Serial.println("Colours Updated..." + String(strip.numPixels()));

}   // END OF 

void Fade() {

static int j;
 
if (j > 256) j = 0; 
RgbColor col = Wheel(j);
//int col = 200; 

for (int i=0; i < countPixels; i++) {
          strip->SetPixelColor(i, col);    //turn every third pixel on
        }
        //strip.setPixelColor(0,0);

        strip->Show();



j++;
}

////////////////////////

void theaterChaseRainbow() {
 
  static int j; 
  static int q;
  if (j > 256) j = 0; 
  if (q > 3) {q = 0; j++;}
  //for (int j=0; j < 256; j++) {  // cycle all 256 colors in the wheel

    //for (int q=0; q < 3; q++) {

        for (int i=0; i < countPixels; i=i+3) {
          strip->SetPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        //strip.setPixelColor(0,0);
        strip->Show();
       
        //delay(1000);
       
        for (int i=0; i < countPixels; i=i+3) {
          strip->SetPixelColor(i+q, 0);        //turn every third pixel off
        }
    //}



    q++;
    // j++;

  //}
}
 //End of chase the rainbow


void ws2812 ()  // put switch cases here...


{

  //Serial.print("ws2812 called case = " + String(opState));
  //wsPoint += 1;

switch (opState)
   {
   case OFF:
      stripoff();
      break;
   case RAINBOW:
      rainbow();
      break;
    case COLOR:
      color(StripColor);
      break;
   case ChaseRainbow:
      theaterChaseRainbow();
      //TuneP();
      break;
   case FADE:
      Fade();
      //TuneI();
      break;
   case ADALIGHT:
      //TuneD();
      if (Adalight_enabled != true)  initialiseAdalight;
      Adalight();
      break;
   }



}

void stripoff ()

{
      //stripOFFcalled = true;
  //Serial.println("off statement processed");
  if (WS2812timerID > -1 ) {
    timer.deleteTimer(WS2812timerID);
    WS2812timerID = -1;
  }

  //Serial.println("stripoff CALLED");
      //strip->clear();
    for(int i=0;i<countPixels; i++) {
    //strip->SetPixelColor(i,RgbColor(0)); 
  }
      strip->Show();
      delay(1); 
      strip->Show();

}

uint32_t hexToDec(String hexString) {
  
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
  
  return decValue;
}










uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


/*
// Set pixel color from 'packed' 32-bit RGB color:
void setPixelColor32(uint16_t n, uint32_t c) {
  if(n < countPixels) {
    uint8_t
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    

    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
    p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;
  }
}
*/

/*

void read_EQ ()

{

digitalWrite(resetPin, HIGH);
delayMicroseconds(20);
digitalWrite(resetPin, LOW);

  for (int i=0;i<7;i++){
    digitalWrite(strobePin, LOW);
    delay(5);
    spectrumValue[i]=analogRead(0); //analogRead(0)
    //spectrumValue[i]=constrain(spectrumValue[i], filter, 1023);
    //spectrumValue[i]=map(spectrumValue[i], filter,1023,0,255);
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
  }
  Serial.println();

}
*/ 
/*
void initiate_EQ ()

{


//  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  //pinMode(ledred, OUTPUT);
  //pinMode(ledblue, OUTPUT);
  //pinMode(ledgreen, OUTPUT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

}
*/

// called to change the number of pixels
void ChangeNeoPixels(uint16_t count, uint8_t pin)
{
    if (strip)
    {
        delete strip;
    }
    strip = new NeoPixelBus(count, pin);
    strip->Begin();
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

void initialiseAdalight (){

    Serial.print("Ada\n"); // Send "Magic Word" string to host
    Adalight_enabled = true;
}

void Adalight () {
}
/*
  for(i = 0; i < sizeof prefix; ++i) {
    //waitLoop: 
    while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    //goto waitLoop;
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
    //goto waitLoop;
  }
  
  memset(leds, 0, countPixels * sizeof(struct strip->));
  // read the transmission data and set LED values
  for (uint8_t i = 0; i < countPixels; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  // shows new values

         strip->Show();


} */

#endif
