/*
enum operatingState { OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, TUNE_D, AUTO};
operatingState opState = OFF;
operatingState LastOpState = OFF;

int WS2812interval = 2000; // default interval...  
int WS2812timerID = -1; // make sure timer is put to off....

int spectrumValue[7];
int filter=80;


//char pixels[24 * 3];

#define resetPin 5
#define strobePin 4

void handle_WS2812 () { // handles the web commands...
Serial.println("WS2812 - Web page called.");
 if (server.arg("mode").length() != 0) WS2812_mode_string(server.arg("mode"));
 if (server.arg("dim").length() != 0) WS2812_dim_string(server.arg("dim"));
 if (server.arg("timer").length() != 0) WS2812timer_command_string(server.arg("timer"));
 if (server.arg("rgb").length() != 0) WS2812_command_string("rgb " + server.arg("rgb"));

}

void WS2812_dim_string (String Value)
{

int dim = Value.toInt();
if (dim > 255) dim = 255;
if (dim < 0) dim = 0;

strip.setBrightness(dim);
Serial.print("Brightness updated to :" + Value);
send_mqtt_msg( "Brightness", Value);                


}

void WS2812_mode_string (String Value)

{
String currentstate;
if (Value == "Rainbow") { opState = RAINBOW; currentstate = "Rainbow"; }
if (Value == "Colour") { opState = COLOR; currentstate = "Colour"; }
if (Value == "ChaseRainbow")  {opState = ChaseRainbow; currentstate = "ChaseRainbow"; }
if (Value == "Fade")  {opState = FADE; currentstate = "Fade"; }


send_mqtt_msg( "mode", currentstate);                

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
  send_mqtt_msg( "mode", "Colour");                

  String instruction = Value.substring(4,Value.length()+1 );
  //Serial.println("RGB command recieved: " + instruction);
  //Serial.print("Example of colour in 32bit: ");
  //Serial.println(strip.getPixelColor(1));
  uint32_t NewColor = hexToDec(instruction);

  color(NewColor);


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

   strip.begin();
   strip.clear();
   strip.show(); 
   strip.show(); 

   Serial.println("WS2812  started");
   
   String temp = String(deviceid) + "/+";
   //char charBuf[30];

     char mqtttopicbuf[200];
 
 //if ( type == 1 ) topic = String(deviceid) +"/" + topic;
 //if ( type == 2 ) topic = "esp/" + topic;
 
  temp.toCharArray(mqtttopicbuf,200);
     
  // topic.toCharArray(mqtttopicbuf,200);
   //message.toCharArray(mqttcharbuf,200);

   //Serial.println("WS topic: " + temp);
   //Serial.println("Size of temp:" + String(sizeof(temp)));
   //Serial.println(mqtttopicbuf);
   //temp.toCharArray(charBuf, 30) ;

   mqttclient.subscribe(mqtttopicbuf);    // ---  subscribe to all esp messages 
  
      //ws2812_int.attach_ms(100, show);


}

uint32_t StripColor; 

void color(uint32_t color)

{
  StripColor = color;
  for(int i=0;i<strip.numPixels(); i++) {
    strip.setPixelColor(i,StripColor);
  }
      strip.setPixelColor(1,0);

  strip.show();

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
  //for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+wsPoint) & 255));
    }
    strip.setPixelColor(1,0);
    strip.show();
    //delay(wait);
  //}
    if (wsPoint==256) wsPoint=0; 
    wsPoint++;
    // Serial.println("Colours Updated..." + String(strip.numPixels()));

}

void Fade() {

static int j;
 
if (j > 256) j = 0; 
int col = Wheel(j);

for (int i=0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, col);    //turn every third pixel on
        }
        strip.setPixelColor(1,0);

        strip.show();



j++;
}

void theaterChaseRainbow() {

  static int j; 
  static int q;
  if (j > 256) j = 0; 
  if (q > 3) {q = 0; j++;}
  //for (int j=0; j < 256; j++) {  // cycle all 256 colors in the wheel

    //for (int q=0; q < 3; q++) {

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.setPixelColor(1,0);
        strip.show();
       
        //delay(1000);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    //}



    q++;
    // j++;

  //}
}



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
   case TUNE_D:
      //TuneD();
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
      strip.clear();
      strip.show();
      delay(1); 
      strip.show();

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

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

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

void CPU_clock_count () {

static long lastminute;
static long loop_count;
static int ten_minute_average_count;
static long ten_minute_total_count;
loop_count++;

if (millis() >= lastminute + 60000) {
  loop_count = loop_count / 60;
  double second_count = (1 / loop_count) * 1000;
  //Serial.println("1S Avg: " + String(loop_count)); 
  send_mqtt_msg( "1S Avg:", String(loop_count));
  send_mqtt_msg( "Loop cycle time (s)" , String(second_count));                
  lastminute = millis();
  ten_minute_average_count++;
  ten_minute_total_count += loop_count;
  loop_count = 0;
}
if (ten_minute_average_count == 10) {
  ten_minute_total_count = (ten_minute_total_count / 10);
  send_mqtt_msg( "10 Minute Loop count Average", String(ten_minute_total_count));                

  //Serial.println("10min average: " + String(ten_minute_total_count));
  ten_minute_total_count =0;
}

}


*/
