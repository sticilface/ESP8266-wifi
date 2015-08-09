// TODO.... 
// 1.  Move animationspeed AND effects timer into one variable! 
// 2.  
// 3.  consolidate text and order of effects
// 4.  MQTT - direct to pixel buffer (maybe have to re work the size of the buffer --->  look at update by mqtt...)
// 5.   
// 6.  Maybe get changes in current dim level to Walk the pixel buffer... and NOT reset the animation effect state
// 7.  rework ALL effects to use animations class in progress
// 8.  SORT OUT THE HSL FLOATS STUFF
// 9   Implement Var3 as pallette type.. Colour Picker, Complementary, analogous, Split complements, triadic, tetradic : http://www.sessions.edu/color-calculator
// 10. Var4 = Pallette Range
// 11. Var? = No of available colours to choose from.   
// 12. Use AJAX and XML to serve static web page!  


void cache ws2812 ()  // put switch cases here...
{


static unsigned long update_strip_time = 0;  //  keeps track of pixel refresh rate... limits updates to 33 Hrz.....
static unsigned long HoldingState_Failover = 0; 
static unsigned long timer_PixelPower = 0; 


switch (opState)
   {
   case OFF:
      StripOFF();
      break;
   case RAINBOW:
      rainbow();
      break;
   case COLOR:
      RGBcolour();
      break;
   case ChaseRainbow:
      theatreChaseRainbow();
      break;
   case FADE:
      Fade();
      break;
   case ADALIGHT:
      Adalight();
      break;
   case TEST:
      test();
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
    case UDP:
      UDPfunc();
      break;
    case RAINBOWCYCLE:
      Rainbowcycle();
      break;
    case SPIRAL:
      spiral();
      break;
    case SQUARES2:
      Squares2(0);
      break;
    case SQUARESRANDOM:
      Squares2(1);
      break;
    case TEST4:
      test4();
      break;
    case SQUARES:
      Squares();
      break;
    case EQ1:
      eq1();
      break;
    case RANDOM:
      Random_colour();
      break;
    case RANDOMFUNC:
      Random_function();
      break;
    //case ARTNET:
    //  Art_Net_func ();
    //  break;
    case RANDOM_TOP_BOTTOM:
      Effect_Top_Bottom(TWOCOLOUR,HSL);
      break;
    case RANDOM_TOP_BOTTOM_LINEAR:
      Effect_Top_Bottom(TWOCOLOUR, RGB);
      break;    
    case SINGLE_COLOUR_FADE:
      Effect_Top_Bottom(SINGLECOLOUR, HSL);
      break;  
    case RANDOM_COLOUR_FADE:
      Effect_Top_Bottom(RANDOMSINGLECOLOUR,HSL);
      break;      
    case HSICYCLE:
      HSI_Cycle();
      break;
    case NEWANIMATIONS:
      test_newanimations();
      break;
   }



if (millis() - update_strip_time > 30) {
    if ( animator->IsAnimating() ) animator->UpdateAnimations(100); 
    delay(0);
    strip->Show();  // takes 6ms with 200, take 12ms with 400 ----> so 100 takes 3ms. 
    update_strip_time = millis();
  } 

  
  if (millis() - timer_PixelPower > 10000 && opState != OFF) {
       power = getPixelPower();
       //Debugf("Power =%u \n",power); 
       timer_PixelPower = millis();

    //   File f = SPIFFS.open("/power.htm", "a");
    //   f.println(power);
    //   f.close();
    }
 


if (opState != HoldingOpState) {
  static bool triggered = false; 
  if (!triggered) { 
    HoldingState_Failover = millis(); 
    triggered = true; 
  } 


if (millis() - HoldingState_Failover > 5000) {
    opState = HoldingOpState;
    triggered = false; 
    Current_Effect_State = PRE_EFFECT; 
  //  Serial.println("failover activated");
  }

}

if(LED_Settings_Changed) {
  Save_LED_Settings(0);                    // Save current settings to EEPROM
  current_loaded_preset_changed = true;    // Change flag to say current effect has changed
  send_mqtt_msg("Preset", "0");            // Update the MQTT preset selection to 0
  LED_Settings_Changed = false;            // Reset settings changed. 
}


/*
static bool ani_update = false;

if (!ani_update && strip->IsAnimating()) {
  Serial.print("Animating...");
  ani_update = true;
} else if (ani_update && !strip->IsAnimating()) {
  Serial.println("Stopped");
  ani_update = false;
}
*/

//    send_mqtt_msg("effect", MODE_STRING[opState]); 



if (paused) lasteffectupdate = millis(); 


} // end of ws2812 function 


void  cache handle_WS2812 () { // handles the web commands...
animator->Pause();
String paused_string, selected; 
bool updateLEDs = false;

 // Serial.print("Current Preset = ");
 // Serial.println(CurrentPreset);
 if (server.args() > 0) {
 if (server.hasArg("mode")) WS2812_mode_string(server.arg("mode"));
 if (server.hasArg("preset")) WS2812_preset_string(server.arg("preset"));
 if ((server.arg("dim") != String(WS2812_Settings.Brightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812_Settings.Timer)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 if ((server.arg("anispeed") != String(CurrentAnimationSpeed)) && (server.arg("anispeed").length() != 0))  AnimationSpeed_command_string(server.arg("anispeed"));
 if (server.hasArg("paused")) {
      paused = (server.arg("paused")).toInt();
      if (paused) animator->Pause(); 
      if (!paused) animator->Resume(); 
    }

 if (server.hasArg("rgbpicker"))  { 
    //WS2812_mode_string("rgb-" + server.arg("rgbpicker"));
    WS2812_Set_New_Colour(server.arg("rgbpicker"));
    Debugln("RGB picker command: " + server.arg("rgbpicker"));
  }

   if (server.hasArg("presetsave")) Save_LED_Settings(  server.arg("presetsave").toInt() );

  //if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command"));

//  if (server.arg("leds").length() != 0) {
//    pixelCount = server.arg("leds").toInt();
//    updateLEDs = true;
//  }

//   if (server.arg("ledpin").length() != 0) {
//    pixelPIN = server.arg("ledpin").toInt();
//    updateLEDs = true;
//  }

  if (server.hasArg("modedrop")) 
    {
      WS2812_mode_string(server.arg("modedrop"));
      }
  } // END of Args for web page..
      //----  having this under here works better as the page gets updated before the request data is fired back!

      if (paused) { 
        paused_string = F("<a href='/ws2812?paused=0'>PLAY</a> ") ; 
      } else {
        paused_string = F("<a href='/ws2812?paused=1'>PAUSE</a>") ; 
      }

String content3 = F("\
<!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'><title> % </title></head><body><h1> % </h1>\
%\
<br> <a href='/ws2812?mode=off'>OFF</a> | <a href='/ws2812?mode=on'>ON</a>  | % | <a href='/ws2812?mode=refresh'>REFRESH</a> | <a href='/lightsconfig'>CONFIG</a>\
<br> PRESET: <a href='/ws2812?preset=1'>1</a> | <a href='/ws2812?preset=2'>2</a> | <a href='/ws2812?preset=3'>3</a> | <a href='/ws2812?preset=4'>4</a> | <a href='/ws2812?preset=5'>5</a> | <a href='/ws2812?preset=6'>6</a> | <a href='/ws2812?preset=7'>7</a> | <a href='/ws2812?preset=8'>8</a> | <a href='/ws2812?preset=9'>9</a> | <a href='/ws2812?preset=10'>10</a>\
<form name=frmTest action='/ws2812' method='POST'>\
Select Mode <select name='modedrop' onchange='this.form.submit();'>\
");

  buf = insertvariable ( content3, String(deviceid));
  buf = insertvariable ( buf, String(deviceid));
  
  if (WiFi.status() == WL_CONNECTED) 
    { buf = insertvariable ( buf, F("<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>"));
    } else { buf = insertvariable ( buf, " "); } ;

  buf = insertvariable ( buf, paused_string);


//  if (wifimode == 1) { buf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>"; } ; // This is needed as it pulls in stuff from internet... when in AP mode causes crash.  
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();
    server.sendContent(buf);

 buf = " ";


for (int k=0; k < numberofmodes; k++ ) {
    if (HoldingOpState == k) { 
        selected = "' selected "; 
      } else selected = "' "; 
  buf += "<option value='" + String(k) + selected + ">" + String(k) + ". " + MODE_STRING[k] + "</option>";
   // httpbuf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";

  }

  buf += "</select>";
  buf += "</form></p>";


    server.sendContent(buf);

    buf = " "; 

  if(WiFi.status() == WL_CONNECTED) {
  String content0 = F("<p><form action='/ws2812' method='POST'\
  <p>Color: <input class='color' name='rgbpicker' value = '%' >\
  <br>  <input type='submit' value='Submit'/>\
  </form>\
  ");

  buf = insertvariable ( content0, WebRGBcolour);  //WebRGBcolour
  
  server.sendContent(buf);

  }  // This is needed as it pulls in stuff from internet... when in AP mode causes crash.  
  

 // 1 = CurrentAnimationSpeed String(CurrentAnimationSpeed) 
 // 2 = CurrentBrightness String(CurrentBrightness)
 // 3 = WS2812interval String(WS2812interval)
 // 4 = pixelCount String(pixelCount)
 // 5 = pixelPIN String(pixelPIN)
 // 6 = String(power) 
  
  String content1 = F("\
  <form name=sliders action='/ws2812' method='POST'>\
  <br>Animation: <input type='range' name='anispeed'min='1' max='10000' value='%' onchange='this.form.submit();' >\
  <br>Brightness: <input type='range' name='dim'min='0' max='255' value='%' onchange='this.form.submit();' >\
  <br>Timer: <input type='range' name='timer'min='1' max='2000' value='%' onchange='this.form.submit();'>\
  </form><br>\
    <form action='/ws2812' method='POST' style='display:inline;'>\
    Current Preset: % \
    <br>Save Preset: <input type='text' id='presetsave' name='presetsave' value='%' >\
    <input type='submit'  value='Save'/>\
    </form>\
    <br>Power = %mA\
  ");
  
  buf = insertvariable ( content1, String(CurrentAnimationSpeed)); 
  buf = insertvariable ( buf, String(WS2812_Settings.Brightness)); 
  buf = insertvariable ( buf, String(WS2812_Settings.Timer)); 
 // buf = insertvariable ( buf, String(pixelCount)); 
 // buf = insertvariable ( buf, String(pixelPIN)); 

  String loaded_var; 

  if (current_loaded_preset == 0)  { loaded_var = "none" ; } else { loaded_var = String(current_loaded_preset) ; } ; 
  if (current_loaded_preset_changed) { loaded_var += " (unsaved changes)" ; }; 


  buf = insertvariable ( buf, loaded_var ); 
  buf = insertvariable ( buf, String(current_loaded_preset) ); 
  buf = insertvariable ( buf, String(power) ); 
  buf += htmlendstring; 
 
  //server.send(200, "text/html", buf);


    server.sendContent(buf);


//if (updateLEDs) { initiateWS2812(); updateLEDs = false;};
//power = getPixelPower();

animator->Resume();
}

void cache AnimationSpeed_command_string (String Value) {

  uint16_t newvalue = Value.toInt();
  Debugf("New Animation speed =%u \n",newvalue);
  CurrentAnimationSpeed = newvalue; 
  lasteffectupdate = 0; 
  current_loaded_preset_changed = true; 
  LED_Settings_Changed = true; 
  Effect_Refresh = true;                  // flag current effect that settings have changed 

}


void cache WS2812_autorestart_string(String Value) {
  
  Value.toLowerCase();

  bool CurrentRestartValue = EEPROM.read(AutoRestartEffectAddress);
  bool changed = false; 

if ( (Value == "on" || Value == "yes") && CurrentRestartValue == false ) 
    { 
      EEPROM.write(AutoRestartEffectAddress, 1) ;
      EEPROM_commit_var = true; 
      Serial.print("Auto Restart Set to True");
      send_mqtt_msg("AutoRestart", "Auto restart set to true"); 
      changed = true; 
    }

if ( (Value == "off" || Value == "no") && CurrentRestartValue == true ) 
    { 
      EEPROM.write(AutoRestartEffectAddress, 0) ;
      EEPROM_commit_var = true; 
      Serial.print("Auto Restart Set to False");
      send_mqtt_msg("AutoRestart", "Auto restart set to false"); 
      changed = true; 
    }
 
if (changed == false) send_mqtt_msg("AutoRestart", "Auto restart unchanged"); 


}


void cache WS2812_toggle_string(String Value) {
  //Serial.print("toggle hit, original mode = "); 
  //Serial.println(current_loaded_preset); 

      if (Value == "toggle") {

       lasteffectupdate = 0;
       current_loaded_preset_changed = true; 


        if (opState == OFF)  {
          //Serial.println("OFF, so turned ON"); 
          HoldingOpState = LastOpState;
          Current_Effect_State = POST_EFFECT;
        } else {
          //Serial.print("ON..advancing: "); 
          current_loaded_preset++;  // advance to next saved.... 
         // Serial.print(current_loaded_preset);
         // Serial.print(",");
          if (current_loaded_preset > 10) current_loaded_preset = 1;

          for (uint8_t i = 0; i < 10; i++ ) { // go through 10 saved ONLY.....
              uint16_t address = START_address_settings + (32 * current_loaded_preset); 
              uint8_t saved = EEPROM.read(address);
              if (saved != 0) break; 
              current_loaded_preset++;        
              if (current_loaded_preset > 10) current_loaded_preset = 1;
           //   Serial.print(current_loaded_preset);
           //   Serial.print(",");
          }
          //Serial.print("  scan end.... mode asked for = ");
          //Serial.print(current_loaded_preset);
          WS2812_preset_string(String(current_loaded_preset));


        }




      }

}




void cache WS2812_preset_string(String Value) {

      lasteffectupdate = 0;
      uint8_t preset = Value.toInt();
      //Serial.print("Preset recieved: ");
      //Serial.print(preset);

      if (preset > 10) preset = 1;
      if (preset < 0 ) preset = 1; 

        Load_LED_Defaults (preset) ; 
        // CurrentPreset = preset; 
        current_loaded_preset = preset;
        current_loaded_preset_changed = false; 

        //Serial.print("Op State = "); 
        //Serial.print()
        HoldingOpState =  LastOpState;
        Current_Effect_State = POST_EFFECT; 

        //send_mqtt_msg("Preset", Value); 
         send_current_settings();  // this prevents settings being sent IMMEDIATELY after boot...

        //  LED_Settings_Changed = true;   // DONT do this here... as it sets preset to 0.. which u don't want to do.... 

        Save_LED_Settings(0);         // this will save the ON state to EEPROM... and the last state.... 

      //Serial.print("...Loaded"); 
}

void cache send_current_settings () {

   if (HoldingOpState == OFF) { 
    send_mqtt_msg("mode","off");
  //  send_mqtt_msg("effect","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }

    //delay(100);
    send_mqtt_msg("timer", String(WS2812_Settings.Timer));
    send_mqtt_msg("brightness", String(WS2812_Settings.Brightness));
    send_mqtt_msg("Preset", String(current_loaded_preset)); 
    send_mqtt_msg("animationspeed", String(CurrentAnimationSpeed));
    send_mqtt_msg("effect", MODE_STRING[HoldingOpState] ); // has to be the effect of the future.. not the current one :)

}


// THINK THIS IS REDUNDANT FUNCTION NOW... 
void cache WS2812_mode_number(String Value) {

      lasteffectupdate = 0;
      uint8_t chosen_mode = Value.toInt();
      Debugln("MODE DROP recieved: " + Value);
      opState = (operatingState)chosen_mode;

      if (chosen_mode != 0)  { 
      LastOpState = (operatingState)chosen_mode;
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      }
}

//  might need to add an effect function timeout thing....  to reset effect and call update with NEW dim setting... 


void  cache  WS2812_dim_string (String Value)
{
      lasteffectupdate = 0; // RESET EFFECT COUNTER, force refresh of effects....
 //     current_loaded_preset_changed = true; 

      int a = Value.toInt();
      if (a > 255) a = 255;
      if (a < 0) a = 0;

      //CurrentBrightness  = a;
      WS2812_Settings.Brightness = a; 

      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
  Effect_Refresh = true;                  // flag current effect that settings have changed 

    // Dim_Strip(CurrentBrightness);  // this is not working yet...  maybe implement...

      // if (isnan(CurrentBrightness)) CurrentBrightness = 255;

      Debugln("Brightness set to: " + String(WS2812_Settings.Brightness));

      send_mqtt_msg("brightness", String(WS2812_Settings.Brightness));

}


void cache WS2812_effect_string (String request) {

      for (uint8_t i = 0; i < numberofmodes; i++ ) {

        if (request == String(MODE_STRING[i])) {
          HoldingOpState = LastOpState = (operatingState)i; // assign selection to holdingopstate... 
          Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
          LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
          //send_mqtt_msg("effect", MODE_STRING[HoldingOpState]); 
          break; 
        }
      } 

 // if (HoldingOpState == OFF) { 
 //   send_mqtt_msg("mode","off");
 // } else { 
 //   send_mqtt_msg("mode", "on");
 // }

      send_current_settings(); 

}

void  cache WS2812_mode_string (String Value)

{

  lasteffectupdate = 0 ; // RESET EFFECT COUNTER
  if(Value == "refresh" ) return; 

  //Random_func_timeout = 0; //RESET additionall timeout... 
  if (paused) paused = false; // this sets it back to play, if paused when a mode change occurs...

  if (Value.toInt() != 0) {  // if the numerical mode does not equal 0 which is off....

      uint8_t chosen_mode = Value.toInt(); // turn it to number...
      HoldingOpState = (operatingState)chosen_mode; // assign selection to holdingopstate... 
      Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
      LastOpState = (operatingState)chosen_mode;
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 

  } else {


  Value.toLowerCase();

  if (Value == "off" | Value == "OFF") { 
      if (opState != OFF) { 
          Current_Effect_State = POST_EFFECT; //  Set this to TERMINATE current effect.... 
          HoldingOpState = OFF;           
      } 
  }

  if (Value == "on" | Value == "ON") { 
      HoldingOpState = LastOpState; 
      Current_Effect_State = POST_EFFECT; 
  } ; 

  if (Value == "refresh" ) { 
    lasteffectupdate = 0;  
  }

  if (Value == "pause") paused = true;
  if (Value == "play") paused = false; 

}

    if(HoldingOpState == OFF) { 
      EEPROM.write(ON_OFF_State_Address,0); 
      Debugln("ON_OFF_State_Address (write) = 0");
    } else {
      EEPROM.write(ON_OFF_State_Address,1); 
      Debugln("ON_OFF_State_Address (write) = 1");
    } 


   if (HoldingOpState == OFF) { 
    send_mqtt_msg("mode","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }

      

    //send_mqtt_msg("effect", MODE_STRING[HoldingOpState]); 
    EEPROM_commit_var = true;
    //if (millis() > 11000) 
    send_current_settings(); 
}


void cache WS2812_Set_New_Colour (String instruction) {

      //opState = LastOpState = COLOR; //  this allows you to pick colour base for other MODES.... 
      //String instruction = Value.substring(4,Value.length()+1 );
      //Serial.println("/n RGB command recieved: " + instruction);
      lasteffectupdate = 0; 
      WebRGBcolour = instruction;
      WS2812_Settings.Color = HEXtoRGB(instruction);
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 

/*      float HUE = (HslColor(NewColour)).H;
      float SAT = (HslColor(NewColour)).S;
      float LIG = (HslColor(NewColour)).L;

      //Debugf("HSL choice : H = %u, S = %u, L = %u ")
      Debug("NEW HSL COLOUR : Hue = ");
      Debug(HUE); 
      Debug(", Sat = "); 
      Debug(SAT); 
      Debug(", Lig = ");       
      Debugln(LIG); */
  
      send_mqtt_msg("colour", WebRGBcolour); 

/*
#ifdef HSL_FLOAT

              float originalHUE = (HslColor(NewColour)).H;
              float originalSAT = (HslColor(NewColour)).S;
              float originalLIG = (HslColor(NewColour)).L;
              Serial.print("HSL components (float): ");
              Serial.print(originalHUE);
              Serial.print(",");
              Serial.print(originalSAT);
              Serial.print(",");
              Serial.println(originalLIG);
#else
              uint8_t originalHUE = (HslColor(NewColour)).H;
              uint8_t originalSAT = (HslColor(NewColour)).S;
              uint8_t originalLIG = (HslColor(NewColour)).L;
              Serial.print("HSL components (int): ");
              Serial.print(originalHUE);
              Serial.print(",");
              Serial.print(originalSAT);
              Serial.print(",");
              Serial.println(originalLIG);             

#endif

              uint8_t originalR = NewColour.R;
              uint8_t originalG = NewColour.G;
              uint8_t originalB = NewColour.B;
              Serial.print("RGB components: ");
              Serial.print(originalR);
              Serial.print(",");
              Serial.print(originalG);
              Serial.print(",");
              Serial.println(originalB);  
*/

}


void cache WS2812timer_command_string (String Value)

{

lasteffectupdate = 0;
WS2812_Settings.Timer = Value.toInt();
send_mqtt_msg("timer", Value); 
LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
Debugf("Timer = %u \n", WS2812_Settings.Timer);
}


//void  WS2812_command_string (String Value) {
//}
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


RgbColor cache dim(RgbColor original) {

    HslColor originalHSL = HslColor(original); 
    float originalLIG = originalHSL.L;
    float newLIG =  originalLIG   * ( float(WS2812_Settings.Brightness) / 255.0 ) ; 
    return RgbColor( HslColor(originalHSL.H, originalHSL.S, newLIG )  );
}

// RgbColor cache dim(RgbColor original, uint8_t brightness) {


//               float originalHUE = HslColor(original).H;
//               float originalSAT = HslColor(original).S;
//               float originalLIG = HslColor(original).L;


//     float newLIG =  originalLIG   * ( (float)brightness / 255.0f ) ; 


//  return RgbColor( HslColor(originalHUE, originalSAT, newLIG )  );

// }






RgbColor cache HEXtoRGB (String hexString) // Converts HEX to RBG object....

{

  uint32_t decValue = 0;
  int nextInt = 0;
  
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


String cache RGBtoHEX (RgbColor value) {

  //char R[3]
 //sprintf( R, "%X", value );





}

void cache Pre_effect() {

Current_Effect_State = RUN_EFFECT; 
lasteffectupdate = 0; 
Random_func_timeout = 0; //RESET additionall timeout... 

}

void cache Post_effect() {

if ( animator->IsAnimating() ) {
    for (uint16_t i = 0; i < strip->PixelCount();i++ ) {
      animator->StopAnimation(i);
    }
}

Current_Effect_State = PRE_EFFECT; 
opState = HoldingOpState; 

}





void cache initiateWS2812 ()

{
  //opState = OFF;
  ChangeNeoPixels(pixelCount, pixelPIN); // initial setup
  strip->Begin();


  //StripOFF();
  SetRandomSeed();
  animator->Resume(); 

}





void cache HSI_Cycle() {

/*
static int j;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

if (millis() > (lasteffectupdate) ){

  //Serial.println("Fade updaed");

if (j > 360) j = 0; 

double j_double = (double)j / 360; 
double v_double = (double)CurrentBrightness / 255; 
double s_double; 

//RgbColor col = hsvToRgb(j_double,1,v_double);

/*  Serial.println();
  Serial.print(j_double);
  Serial.print("-->");
  Serial.print(" R:");
  Serial.print(col.R);
  Serial.print(" G:");
  Serial.print(col.G);
  Serial.print(" B:");
  Serial.print(col.B);  


//int col = 200; 

for (int i=0; i < pixelCount; i++) {

double s_double = (double)i / pixelCount; 


RgbColor col = hsvToRgb(j_double,s_double,v_double);

    strip->SetPixelColor(i, col);   
        }

lasteffectupdate = millis() + WS2812interval ;

j++; 

}

//  END of EFFECT  

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}




void cache Random_function() {
/*
static uint8_t random_choice = 0 ; 
static uint32_t Random_func_next_time = 0;
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


if (millis() > Random_func_next_time + 1000) Random_func_next_time = 0; // This resets things if its been turned off after 1s... 

    if (millis() > Random_func_next_time) {

    random_choice = random(0, 5);

    Random_func_next_time = millis() + random(WS2812interval*30, (WS2812interval * 300) ); 

    }


    if (random_choice == 0) rainbow();
    if (random_choice == 1) spiral();
    if (random_choice == 2) Rainbowcycle();
    if (random_choice == 3) Random_colour();
    if (random_choice == 4) Squares2(1);
    if (random_choice == 5) Random_Top_Bottom(0);



if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache Random_colour() {

 //long Random_func_timeout = 0, Random_func_lasttime = 0; 
static uint8_t current_r = 0;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

    if (millis() > lasteffectupdate) {
      //Serial.print("New random choice..."); 

      uint16_t random_animation_speed = random(2000, 20000);

      //RgbColor random_colour_random = RgbColor(random(255),random(255),random(255));  // generates lots of white colours... not balenced..
      RgbColor random_colour_random = Wheel(random(255));

  //    SetRGBcolour(random_colour_random,random_animation_speed);

      //Random_func_lasttime = millis(); 
      long Random_func_timeout = random(60000, 60000*5);
      //Random_func_timeout = random(10000, 60000);
      lasteffectupdate = millis() + Random_func_timeout; 
    }




if (Current_Effect_State == POST_EFFECT) Post_effect(); 

} // end of random func


void cache CoolBlobs() {




if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
    if (millis() > (lasteffectupdate ) ){
      




      lasteffectupdate = millis() + WS2812_Settings.Timer; 
    } // end of timer if


if (Current_Effect_State == POST_EFFECT) Post_effect(); 

} // end of coolblobs


//////// i nserted
/*

void setcolour () {

    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {

      //strip->LinearFadePixelColor(5000, pixel, NewColour);

    }

}

*/

void cache  FadeInFadeOutRinseRepeat(uint8_t peak) {

  /*
  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (effectPosition == 0)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(800,100);
      strip->LinearFadePixelColor(time, pixel, RgbColor(random(peak), random(peak), random(peak)));
    }
  }
  else if (effectPosition == 1)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(600,700);
      strip->LinearFadePixelColor(time, pixel, RgbColor(0, 0, 0));
    }
  }
  effectPosition = (effectPosition + 1) % 2; // next effectPosition and keep within the number of effectPositions
  
if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache PickRandom(uint8_t peak)
{

  /*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
  // pick random set of pixels to animate
  uint8_t count = random(pixelCount);
  while (count > 0)
  {
    uint8_t pixel = random(pixelCount);
    
    // configure the animations
    RgbColor color = Wheel(random(255)); // = //strip->getPixelColor(pixel);

    //color = RgbColor(random(peak), random(peak), random(peak));

    
    uint16_t time = random(100,400);
    strip->LinearFadePixelColor( time, pixel, color);
    
    count--;
  }

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache LoopAround(uint8_t peak, uint16_t speed)
{
  /*
  // Looping around the ring sample
  uint16_t prevPixel;
  RgbColor prevColor;
  
  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 5)) % pixelCount; 
  strip->LinearFadePixelColor(speed, prevPixel, RgbColor(0, 0, 0));
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 4)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 3)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 2)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 1)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade current one light
  strip->LinearFadePixelColor(speed, effectPosition, RgbColor(random(peak), random(peak), random(peak)));
  effectPosition = (effectPosition + 1) % pixelCount;
  

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache SetRandomSeed()
{
 /* uint32_t seed;
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
*/
}


RgbColor cache Wheel (byte WheelPos) {

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


void cache Fade() {

switch(Current_Effect_State) {
    case PRE_EFFECT:

    effectPosition = 0; 
    Pre_effect(); 

    break;
    case RUN_EFFECT:  

 if (animator->IsAnimating()) { break; }  ; //  This line stops the effect from running if it is still animating! 
     
      if (millis() - lasteffectupdate > ( WS2812_Settings.Timer * 150 )) {      


     fade_to(   dim(Wheel(random(0,255) ) ) , CurrentAnimationSpeed, RGB ) ;
     lasteffectupdate = millis(); 
};


    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
}



void  cache Rainbowcycle() {
  switch(Current_Effect_State) {
    case PRE_EFFECT:
    effectPosition = 0; 
    Pre_effect(); 
    for (uint16_t i = 0; i < pixelCount; i++)
            {
              RgbColor original = strip->GetPixelColor(i);
        
        AnimUpdateCallback animUpdate = [=](float progress)
        {
           RgbColor updatedColor = RgbColor::LinearBlend(original, dim(Wheel(i * 256 / pixelCount + effectPosition)) ,  progress) ;
            strip->SetPixelColor(i, updatedColor);
        };
        animator->StartAnimation(i, 2000, animUpdate);
        lasteffectupdate = millis();  
    effectPosition++; 
    }
    break;
    case RUN_EFFECT:  
    if (animator->IsAnimating()) { break; }  ; //  This line stops the effect from running if it is still in the warm up! 
      if (millis() - lasteffectupdate > WS2812_Settings.Timer) {      

           for(uint16_t i=0; i< pixelCount; i++) {
                strip->SetPixelColor(i, dim(Wheel(i * 256 / pixelCount + effectPosition)));
            }
          if (effectPosition==256*5) effectPosition=0; 
          lasteffectupdate = millis(); 
          effectPosition++;
          }
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
} // END OF RAINBOW CYCLE







void cache test4() {

  uint8_t x,y, total_y;
  uint8_t total_x = WS2812_Settings.Total_X; 
  uint8_t square_size = WS2812_Settings.Effect_Max_Size;
  uint8_t numberofpoints = WS2812_Settings.Effect_Count ; // default 5, if it = 10, then its random......
  uint8_t effect_option = WS2812_Settings.Effect_Option;
  uint8_t dimoffset ; 
  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 1;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 

switch(Current_Effect_State) {

    case PRE_EFFECT:
      Pre_effect();

    case RUN_EFFECT:

      if (millis() - lasteffectupdate > 2000) {      

       uint8_t x_rand = random(0, total_x - square_size + 1) ; 
       uint8_t y_rand = random(0, total_y - square_size + 1) ;

      uint8_t sq_pixel = 3;

      Serial.print("(");
        Serial.print(x_rand);
        Serial.print(",");
        Serial.print(y_rand);
        Serial.print(") ==> ");


       for (uint8_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++) {

          int pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x ); 
          //Serial.print("."); 


         // if (pixel >= 0) { 
         //       if (animator->IsAnimating(pixel)) { coordinates_OK = false; Serial.println(" REJECT") ; break; }; 
         // }
         //   if (sq_pixel == (square_size * square_size)-1) { coordinates_OK = true; Serial.print(" ACCEPT") ;}; 

         // }


        Serial.print(pixel); 
        Serial.print(",");



}

      lasteffectupdate = millis(); 

Serial.println(")");
}








    break;


    case POST_EFFECT:
    Post_effect(); 
    break;

}

}

/*


if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

//Serial.println("TEST4");
  static int wsPoint = 0;
  RgbColor color1 = Wheel(100);
  RgbColor color2 = Wheel(200);

if (wsPoint == 255) wsPoint = 0; 
 if (millis() > (lasteffectupdate ) ){





HsvFADErgb(color1,color2,wsPoint); 



lasteffectupdate = millis() + WS2812interval ;
wsPoint++; 
}

if (Current_Effect_State == POST_EFFECT) Post_effect(); 
}




*/

void  cache test() {
switch(Current_Effect_State) {

    case PRE_EFFECT:
      Pre_effect();
        for (uint8_t i = 0; i < 1; i++ ) {
       
           RgbColor originalColor = RgbColor(0,0,0); 
           RgbColor color = RgbColor(255,0,0);
            // define the effect to apply, in this case linear blend

            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor;
                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                updatedColor = RgbColor::LinearBlend(originalColor, color, new_progress);
                strip->SetPixelColor(0, updatedColor);
            };

            animator->StartAnimation(0, 5000, animUpdate);

}

    break;
    case RUN_EFFECT:  
  
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }






} // end of test 4

 void cache rainbow() {


switch(Current_Effect_State) {

    case PRE_EFFECT:
    Pre_effect();

    break;
    case RUN_EFFECT:  


      if (millis() > (lasteffectupdate) ){

     static int wsPoint =0;

     for(int i=0; i<pixelCount; i++) {
        //RgbColor tempcolour = Wheel(i+wsPoint);
        strip->SetPixelColor(i, dim(Wheel(i+wsPoint)));
     }
    
//      if (wsPoint == WS2812_Settings.var3) wsPoint = WS2812_Settings.var2; 
       wsPoint++;
       lasteffectupdate = millis() + WS2812_Settings.Timer ;

//    if(WS2812_Settings.var3 == 0) WS2812_Settings.var3 = 255; // MAKE sure that there is a +ve end point for the colour wheel... else wsPoint = var3;

    }
 
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;

}


}

/*

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
// Var 1 = timer interval
// Var 2 = wsPoint Min
// Var 3 = wsPoint Max
// Var 4 = wsPoint step....  
// Var 5 = pixel jump?  ie... instead of i++, do i =+ var 3... ? might need to turn of previous pixel
// Var 6 = wheel multipler... or wheel map..  (wheel takes value.. returns color...)
// try to constrain wspoint which is color generator... 
 if(var3 == 0) var3 = 256; // MAKE sure that there is a +ve end point for the colour wheel... else wsPoint = var3;

 if (millis() > (lasteffectupdate) ){

  static int wsPoint =0;
  //uint16_t i; // , j;
  //pixelsNUM = 60;
  //for(j=0; j<256; j++) { v
    for(int i=0; i<pixelCount; i++) {
    //RgbColor tempcolour = Wheel(i+wsPoint);
    strip->SetPixelColor(i, dim(Wheel(i+wsPoint)));
    }
    
    if (wsPoint==var3) wsPoint=var2; 
    wsPoint++;
    lasteffectupdate = millis() + WS2812interval ;
}
    // Serial.println("Colours Updated..." + String(//strip->numPixels()));

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

}   // END OF RAINBOW


/*
void cache clearpixels() {

    memset(pixelsPOINT, 0, 3 * strip->PixelCount() );   // clear current without... 


}
*/



void cache spiral() {
/*
static uint16_t currentcolor = 0;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (millis() > (lasteffectupdate ) ) 
  { // effect update timer
  uint8_t pitch = 0 ;
  
  if (var7 == 0 ) pitch = 13; else pitch = var7;
  uint8_t total_y = return_total_y(pitch); // get the number of rows.  rounds up...
  uint8_t x,y;

 //clearpixels();

    for (x = 0; x < pitch; x+=2) {
      RgbColor colour = Wheel(( x * 256 / pitch + currentcolor) ); // i * 256 / pixelCount + wsPoint) 
      for (y = 0; y < total_y; y++) {
      strip->SetPixelColor(return_pixel( x, y, pitch), dim(colour));
      }
    }
  

  if (currentcolor == 256) currentcolor = 0; else currentcolor++ ;// cycle through to next colour
  lasteffectupdate = millis() + WS2812interval ;

  } // effect update timer

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}
/*
// FACES ALGO....
void cache test3 () {

  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 5;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {

      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {

      RgbColor colour = RgbColor(random(255),random(255),random(255));

      if (square_size == 10) colour.Darken(random(0,200));

      if (square_size == 10) square_size = random(3,7);


      uint8_t x_rand = random(0, total_x - square_size ) ; 
      uint8_t y_rand = random(0, total_y - square_size ) ;

         


      // strip->LinearFadePixelColor(time, return_shape_square(x_rand, y_rand, 4, square_size, total_y ) , colour);


        

        //Serial.println();

      for (int j =0; j < (square_size * square_size) ; j++) {


        //Serial.print("GRID " + String (square_size * square_size) + "... ");

        int pixel1 = return_shape_face(x_rand, y_rand, j, square_size, total_x ); 

        //Serial.print(pixel1);
        //Serial.print(" ");

        if (pixel1 <= pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));
      
      }


    }

    //Serial.println("------------------");

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }




if (Current_Effect_State == POST_EFFECT) Post_effect(); 
} // end of test

*/


// Working 

void cache fade_to(RgbColor Colour) {
fade_to(Colour,2000,RGB); 
}

void cache fade_to(RgbColor Colour, uint16_t time ) {
fade_to(Colour,time,RGB); 
}

void cache fade_to(RgbColor NewColour, uint16_t time, BlendMethod method ) {
    
     for (uint16_t i = 0; i < pixelCount; i++)
            
            {

        RgbColor original = strip->GetPixelColor(i);
        // NewColour = dim(NewColour);

        AnimUpdateCallback animUpdate = [=](float progress)
        {
          if (method == RGB) { 
            RgbColor updatedColor = RgbColor::LinearBlend(original, NewColour , progress) ;   
            strip->SetPixelColor(i, updatedColor); 
          }; 

           if (method == HSL) { 
            HslColor updatedColor = HslColor::LinearBlend(HslColor(original), HslColor(NewColour), progress); 
            strip->SetPixelColor(i, updatedColor);  
          };         
           
        };

        animator->StartAnimation(i, time, animUpdate);
   }
}




void cache Squares () {

  /*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  static int wsPoint = 0;
  
  if (var8 == 0) var8 = 5;
  if (var7 == 0) var7 = 13;  

  uint8_t numberofpoints = var8; // default 5
  uint8_t x,y;
  uint8_t total_y = return_total_y(var7); 
  uint8_t total_x = var7; 


    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {
      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        //strip->SetPixelColor(i,0);
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {


      uint8_t x_rand = random(1, total_x-2) ; 
      uint8_t y_rand = random(1, total_y-2) ;

      RgbColor colour = Wheel(random(255)) ; //  RgbColor(random(255),random(255),random(255));

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand,y_rand,total_x), dim(colour));

      colour.Darken(50);

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand - 1 , total_x     ), dim(colour));       


    }

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }



//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 
if (Current_Effect_State == POST_EFFECT) Post_effect();  



*/

} // end of Squares






void cache theatreChaseRainbow() {

  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


  static uint16_t colourpoint = 0; 
  static uint8_t animationstate = 0 ; 
  if (millis() > (lasteffectupdate ) ) {


  //for (int j=0; j < 256; j++) {       // cycle all 256 colors in the wheel

        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+animationstate, dim(Wheel(i+colourpoint)));    //turn every third pixel on
        }


        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+ animationstate - 1, 0);    //turn every third pixel on
        }
        animationstate++; 

        if (animationstate == 3) animationstate = 0; // reset animation state. 


  
    colourpoint++; 
    lasteffectupdate = millis() + WS2812_Settings.Timer ;
  }  //  end of timer..... 


//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 
if (Current_Effect_State == POST_EFFECT) Post_effect();  

}



void cache Adalight_Flash() {

    for(uint16_t i=0; i<pixelCount; i++) {
        //animator->StopAnimation(i);
        strip->SetPixelColor(i, RgbColor(255,0,0));
          }
    strip->Show(); 
    delay(200);
        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,255,0));
          }
        strip->Show(); 
    delay(200);
        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,0,255));
          }
        strip->Show(); 
    delay(200);

    strip->ClearTo(RgbColor(0,0,0));

  }


void cache Adalight () {    //  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
 
  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
  static boolean Adalight_configured;
  static uint16_t effectbuf_position = 0;
  enum mode { MODE_INITIALISE = 0, MODE_HEADER, MODE_CHECKSUM, MODE_DATA, MODE_SHOW, MODE_FINISH};
  static mode state = MODE_INITIALISE;
  static int effect_timeout = 0;
  static uint8_t prefixcount = 0;
  static unsigned long ada_sent = 0; 
  static unsigned long pixellatchtime = 0;
  const unsigned long serialTimeout = 15000; // turns LEDs of if nothing recieved for 15 seconds..
  //const unsigned long initializetimeout = 10000; 
  //static unsigned long initializetime = 0; 
    //if (!Adalight_configured) {
    //    Adalight_Flash(); 
    //    Adalight_configured = true;
    //}

//    if (pixellatchtime > 0 && (pixellatchtime + serialTimeout) < millis()) {
//      pixellatchtime = 0; // reset counter / latch to zero should only fire when show pixls has been called!
//      StripOFF();  // turn off pixels... 
//      state = MODE_HEADER;  // resume header search....
//    }

    // if(millis() > initializetime + initializetimeout) state = MODE_INITIALISE; // this goes to initiaase mode...

    if (Current_Effect_State == PRE_EFFECT) { state = MODE_INITIALISE; } ; 
      //Pre_effect();  
    // }

    if (Current_Effect_State == POST_EFFECT) state = MODE_FINISH;  

    //if (Current_Effect_State == POST_EFFECT) state = MODE_FINISH; 


  switch (state) {

    case MODE_INITIALISE:
      Serial.println("Begining of Adalight");
      if(millis() > 60000) Adalight_Flash(); 
      state = MODE_HEADER;
      Pre_effect();  
      //Current_Effect_State = RUN_EFFECT;

    case MODE_HEADER:

      effectbuf_position = 0; // reset the buffer position for DATA collection...

          if(Serial.available()) { // if there is serial available... process it... could be 1  could be 100....
               
            for (int i = 0; i < Serial.available(); i++) {  // go through every character in serial buffer looking for prefix...

              if (Serial.read() == prefix[prefixcount]) { // if character is found... then look for next...
                  prefixcount++;
              } else prefixcount = 0;  //  otherwise reset....  ////

            if (prefixcount == 3) {
            effect_timeout = millis(); // generates START TIME.....
            state = MODE_CHECKSUM;
            prefixcount =0;
            break; 
            } // end of if prefix == 3
            } // end of for loop going through serial....
            } else if (!Serial.available() && (ada_sent + 5000) < millis()) {
                  Serial.print("Ada\n"); // Send "Magic Word" string to host
                  ada_sent = millis(); 
            } // end of serial available....

    break;

    case MODE_CHECKSUM:

        if (Serial.available() >= 3) {
          hi  = Serial.read();
          lo  = Serial.read();
          chk = Serial.read();
          if(chk == (hi ^ lo ^ 0x55)) {
            state = MODE_DATA;
          } else {
            state = MODE_HEADER; // ELSE RESET.......
          }
        }

      if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.

      break;

    case MODE_DATA:

        //  this bit is what might... be causing the flashing... as it extends past memory stuctures....
        while (Serial.available() && effectbuf_position < 3 * strip->PixelCount()) {  // was <=  
          pixelsPOINT[effectbuf_position++] = Serial.read();
          //if (effectbuf_position == 3 * strip->PixelCount()) break; 
        }

      if (effectbuf_position >= 3*pixelCount) { // goto show when buffer has recieved enough data...
        state = MODE_SHOW;
        break;
      } 

        if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.


      break;

    case MODE_SHOW:

      strip->Dirty(); // MUST USE if you're using the direct buffer copy... 
      pixellatchtime = millis();
      state = MODE_HEADER;
      break;

    case MODE_FINISH:

      Serial.print("END OF ADALIGHT..."); 
      //Current_Effect_State = PRE_EFFECT; 
      //opState = HoldingOpState; 
      //state == MODE_INITIALISE; 
      Post_effect();  

    break; 
}

      //strip->Show();


 //initializetime = millis(); // this resets the timer 


//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 

}


// called to change the number of pixels
void cache ChangeNeoPixels(uint16_t count, uint8_t pin)  {
  
 bool commitchanges = false; 
 Current_Effect_State = PRE_EFFECT; // THIS reboots the current effect, if one is running

        uint8_t pixelPINstored = EEPROM.read(PixelPIN_address);    
        uint8_t a = EEPROM.read(PixelCount_address);
        uint8_t b = EEPROM.read(PixelCount_address+1);
        if(isnan(a)) a = 0;
        if(isnan(b)) b = 0;
        uint16_t pixelCountstored = a*256+b;

    if (count != pixelCountstored) {

        Debugln("Pixel count changed..."); 
        int a = pixelCount/256;
        int b = pixelCount % 256;        
        EEPROM.write(PixelCount_address,a);
        EEPROM.write(PixelCount_address+1,b);
        commitchanges = true;
        if (EEPROM.read(PixelCount_enablebyte) != flagvalue) EEPROM.write(PixelCount_enablebyte,flagvalue) ;
    }

    if (pin != pixelPINstored) {
        EEPROM.write(PixelPIN_address, (byte)pin);
        if (EEPROM.read(PixelPIN_enablebyte) != flagvalue) EEPROM.write(PixelPIN_enablebyte,flagvalue) ; 
        commitchanges = true;
    }

    if (commitchanges == true) {
        EEPROM_commit_var = true;
        Debugln("WS2812 Settings Updated."); 
        }



  if (animator != NULL)
  {
    delete animator;
  }
  if (strip != NULL)
  {           
  strip->ClearTo(RgbColor(0,0,0));    // this set the strip to off before changing pixel amount...    
  strip->Show();  
  delete strip;
  }


  strip = new NeoPixelBus(count, pin);
  animator = new NeoPixelAnimator(strip);
  pixelsPOINT = (uint8_t*)strip->Pixels(); ///  used for direct access to pixelbus buffer...
    

  if (strip->PixelCount() < 10) WS2812_Settings.Total_X = 1; // can't remember what this does...  sets x width i think... 


}

void cache UDPfunc () {

int packetSize; 
//static int packetno = 0; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:

      //Serial.println("UDP mode enabled\n"); // Send "Magic Word" string to host
      if(millis() > 60000) Adalight_Flash(); 
      Udp.beginMulticast(WiFi.localIP(), multicast_ip_addr, UDPlightPort); 
      Pre_effect(); 

    break; 

    case RUN_EFFECT:

      packetSize = Udp.parsePacket();

        if  (Udp.available())  {
          //Serial.println(packetno++);
             for (int i = 0; i < packetSize; i = i + 3) {
                if (i > pixelCount * 3) break;         // Stops reading if LED count is reached. 
                    pixelsPOINT[i + 1] = Udp.read();   // direct buffer is GRB, 
                    pixelsPOINT[i]     = Udp.read();
                    pixelsPOINT[i + 2] = Udp.read();
              }
              Udp.flush();
              strip->Dirty(); 
        }
      
      break;

    case POST_EFFECT: 
      //packetno = 0; 
      Udp.stop(); 
      Post_effect(); 
      break; 

      
} 
}

int cache getPixelPower () {
 int brightnesstally = 0;

 for (int i = 0;i < pixelCount; i++) {
  RgbColor colour = strip->GetPixelColor(i);
  int brightness = colour.CalculateBrightness();
  brightness = map(brightness,0,255,0,60);
  brightnesstally = brightnesstally + brightness;
}

 //brightnesstally = brightnesstally / pixelCount;

return brightnesstally;
} 





void cache handle_lights_config() {

bool updateLEDs = false; 

   if (server.args() > 1 || ( server.hasArg("preset") == false  && server.args() == 1) ) { 
      lasteffectupdate = 0; 
      Random_func_timeout = 0; 
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      Effect_Refresh = true;                  // flag current effect that settings have changed 
      Debugln("Reset vars true"); 
      }; 
   if (server.hasArg("var1")) WS2812_Settings.Palette_Choice = (Palette)server.arg("var1").toInt();
   if (server.hasArg("var2")) WS2812_Settings.Palette_Range = server.arg("var2").toInt(); // colour point min
   if (server.hasArg("var3")) WS2812_Settings.Palette_Number = server.arg("var3").toInt(); // colour point max
   if (server.hasArg("var4")) WS2812_Settings.Random = server.arg("var4").toInt();
   if (server.hasArg("var5")) WS2812_Settings.Time_Stretch = server.arg("var5").toInt();
   if (server.hasArg("var6")) WS2812_Settings.Total_X = server.arg("var6").toInt();
   if (server.hasArg("var7")) WS2812_Settings.Effect_Count = server.arg("var7").toInt();
   if (server.hasArg("var8")) WS2812_Settings.Effect_Min_Size = server.arg("var8").toInt();
   if (server.hasArg("var9")) WS2812_Settings.Effect_Max_Size = server.arg("var9").toInt();
   if (server.hasArg("var10"))  WS2812_Settings.Effect_Option = server.arg("var10").toInt();   //  Serial.println("Var 10 updated"); }; 
  
   if (server.hasArg("preset")) Save_LED_Settings(server.arg("preset").toInt() );
  
   // String a = ESP.getFlashChipSizeByChipId(); 
   if (server.arg("reset") == "true") { 
      WS2812_Settings.Palette_Choice = ALL; 
      WS2812_Settings.Palette_Range = 0; 
      WS2812_Settings.Palette_Number = 0; 
      WS2812_Settings.Random = 0; 
      WS2812_Settings.Time_Stretch = 0; 
      WS2812_Settings.Total_X = 0; 
      WS2812_Settings.Effect_Count = 0; 
      WS2812_Settings.Effect_Min_Size = 0; 
      WS2812_Settings.Effect_Max_Size = 0; 
      WS2812_Settings.Effect_Option = 0;};

  if (server.hasArg("leds")) {
    pixelCount = server.arg("leds").toInt();
    updateLEDs = true;
  }

   if (server.hasArg("ledpin")) {
    pixelPIN = server.arg("ledpin").toInt();
    updateLEDs = true;
  }


  String content = F("\
    <!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'>\
    <title> % </title></head><body><h1> % </h1>\
    <br> <a href='/lightsconfig?reset=true'>RESET TO DEFAULTS</a>\
    <form name=form action='/lightsconfig' method='POST'>");

    buf = insertvariable ( content, String(deviceid));
    buf = insertvariable ( buf, String(deviceid));
    
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();
    server.sendContent(buf);
    buf = " "; 

  //buf += "Select Mode <select name='modedrop' onchange='this.form.submit();'>";

  for (int k=0; k < 10; k++ ) {
  //buf += "<option value='" + String(k) + selected + ">" + String(MODE_STRING[k]) + "</option>";
   // buf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";
    String inserted; 

    if (k ==0) inserted = String(WS2812_Settings.Palette_Choice);
    if (k ==1) inserted = String(WS2812_Settings.Palette_Range);
    if (k ==2) inserted = String(WS2812_Settings.Palette_Number);
    if (k ==3) inserted = String(WS2812_Settings.Random);
    if (k ==4) inserted = String(WS2812_Settings.Time_Stretch);
    if (k ==5) inserted = String(WS2812_Settings.Total_X);
    if (k ==6) inserted = String(WS2812_Settings.Effect_Count);
    if (k ==7) inserted = String(WS2812_Settings.Effect_Min_Size);
    if (k ==8) inserted = String(WS2812_Settings.Effect_Max_Size);
    if (k ==9) inserted = String(WS2812_Settings.Effect_Option);


    buf += String(VAR_STRING[k]) + " : <input type='text' id='var" + String(k+1) + "' name='var" + String(k+1) + "' value='"+ inserted +"'><br>";
  }

  buf += F("<input type='submit' value='Submit'>\
          </form></p>\
          <form action='/lightsconfig' method='POST'>\
          <p>Save Preset: <input type='text' id='leds' name='preset' value='%' >\
          <br><input type='submit'  value='Save'/>\
          </form>\
          <form action='/lightsconfig' method='POST'>\
          <p>LEDs: <input type='text' id='leds' name='leds' value='%' >\
          <br>PIN: <input type='text' id='ledpin' name='ledpin' value='%' >\
          <br>  <input type='submit' value='Submit'/>\
          </form>\
          ");

  buf += htmlendstring; 
  buf = insertvariable ( buf, String(current_loaded_preset)); 
  buf = insertvariable ( buf, String(pixelCount)); 
  buf = insertvariable ( buf, String(pixelPIN)); 


  server.sendContent(buf);


if (updateLEDs) { initiateWS2812(); updateLEDs = false;};

  //server.send(200, "text/html", buf);



}



uint16_t cache return_pixel(uint16_t x, uint16_t y, uint8_t pitch) {
  int a = (pitch * y) + x; 
  if ( a > strip->PixelCount() ) a = -1; 
  return a; 
}


uint16_t cache return_total_y(uint8_t pitch) {
 uint16_t y = pixelCount / pitch; 
 float remainder = pixelCount % pitch;
 if (remainder > 0) { y++; } 
  return y;
};


//  Returns a pixel number... starting in bottom left...
int cache return_shape_square(uint16_t first_pixel_x, uint16_t first_pixel_y , uint16_t desired_pixel, uint8_t grid_size, uint16_t total_in_row) {

  int pixel; 
  uint16_t pixel_x, pixel_y;
  uint8_t row, row_left  ;
  row = desired_pixel / grid_size; 
  row_left = desired_pixel % grid_size;
  pixel_y = first_pixel_y + row;  
  pixel_x = first_pixel_x + row_left; 
  pixel = return_pixel(pixel_x, pixel_y, total_in_row);
  if (row >= return_total_y(total_in_row)) pixel = -1; 
  if (row_left >= total_in_row ) pixel = -1; 
  return pixel; 
}



// function to shift pixels in blocks one way or another...

void cache pixelshift(uint16_t start, uint16_t end) {

  pixelshift( start, end,100); 
}


void cache pixelshift(uint16_t start, uint16_t end, uint16_t pixelshift_timer) {

static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
//uint16_t pixelshift_timer = 100; 

if (millis() > (last_pixelshift + pixelshift_timer)) {

  if (end > start) { // move direction right

    for (uint16_t i = end; i > start ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);

    }
  }   else if (start > end) {  // Move direction = left

    for (uint16_t i = end; i < start; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);

    }

  }
  strip->SetPixelColor(start, 0);
  last_pixelshift = millis(); 
}


}


//// function to shift pixels in blocks one way or another...
void cache pixelshift_middle() {


pixelshift_middle(100); 

}


void cache pixelshift_middle(uint16_t pixelshift_timer) {
/*
static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
uint16_t middle = strip->PixelCount() / 2; 
uint8_t remainderodd = 0; 
if  (strip->PixelCount() % 2 != 0) remainderodd = 1 ; 



if (millis() > (last_pixelshift + pixelshift_timer)) {


    for (uint16_t i = 0; i < middle ; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);
    }

    for (uint16_t i = strip->PixelCount(); i > middle + 1 ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);
    } 

  
  strip->SetPixelColor(middle, 0);
  if (remainderodd != 0) strip->SetPixelColor(middle+1, 0);


  last_pixelshift = millis(); 

}

*/
}
 // END of pixel shift




void cache eq1 () {
/*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (millis() > lasteffectupdate  )  {

      strip->SetPixelColor(3, RgbColor(255,0,0));
      strip->SetPixelColor(4, RgbColor(0,255,0));
      //strip->SetPixelColor(4, RgbColor(0,0,255));
      lasteffectupdate = millis() + WS2812interval;
  } 
          
uint8_t direction = random(0,2);

if (direction == 0 ) {

pixelshift(6,0);

} else if (direction == 1) {

pixelshift(0,6);

} else if (direction == 2) {

pixelshift_middle(); 


};
 
// pixelshift(6,0);


if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache Set_Colour_ToptoBottom() {







}

//  This function returns a random colour from palette if no index is specified!  
RgbColor cache Return_Palette (RgbColor Input) {
      uint8_t random_choice = random(1);
      uint8_t Index; 
      if (random_choice == 0) Index = 0;
      if (random_choice == 1) Index = WS2812_Settings.Palette_Number; 
      Return_Palette (Input, Index) ;
}

/////////////////////
//
//      This function depends on all the globals for WS2812_Settings.*  
//      
//
//
/////////////////////

RgbColor cache Return_Palette (RgbColor Input, uint8_t Index) {
float Palette_Range = 10 / float(WS2812_Settings.Palette_Range);
RgbColor Output = RgbColor(0,0,0); 

switch (WS2812_Settings.Palette_Choice) {

          case ALL:                // 0 
                Index = Index % 2; 
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Wheel(random(255)); 
                break;

          case COMPLEMENTARY:      // 1
                Index = Index % 2; 
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Return_Complementary(Input);
                break;

          case MONOCHROMATIC:     //  2
                //Debugln("MONOCHROMATIC"); 
                break;

          case ANALOGOUS:         // 3

                Index = Index % WS2812_Settings.Palette_Number; 
                Output =  Return_Analogous (Input, Index, WS2812_Settings.Palette_Number, Palette_Range );          
                break;

          case SPLITCOMPLEMENTS:  // 4
                //Debugln("SPLITCOMPLEMENTS"); 
                Index = Index % 3; 
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Return_Split_Complementary( Input,  0 , Palette_Range);
                if (Index == 2 ) Output = Return_Split_Complementary( Input,  1 , Palette_Range);

          break;
          case TRIADIC:           // 5
                //Debugln("TRIADIC"); 
                Index = Index % 3; 


          break;
          case TETRADIC:          // 6
                //Debugln("TETRADIC"); 
                Index = Index % 4; 


          break;
      }
      yield();
      return Output; 

}




// Overloaded func for topbottom fade...
void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint16_t count_x) {

  top_bottom_fade(Top,Bottom, count_x, CurrentAnimationSpeed, HSL);
}

void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint16_t count_x,uint16_t time) {

  top_bottom_fade(Top,Bottom, count_x, time, HSL);
}



void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint16_t count_x, uint16_t time, BlendMethod Method) {

uint16_t x,y;
float colour_steps; 
// float colour_stepsF; 
uint16_t total_y = return_total_y(count_x); // get total number of rows
RgbColor colourRGB;
HslColor colourHSL; 
//if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


for (y = 0; y < total_y; y++) {

    colour_steps = (float) y / (float) total_y; 
    if (y == 0) colour_steps = 0.0f; // this stets the top blend.  
    if (y == total_y - 1) colour_steps = 1.0f;  // this sets the bottom blend
  // colour_stepsF = (float)colour_steps / 255.0;  // this converts it to float for new lib...

    if (Method == RGB) { colourRGB = RgbColor::LinearBlend(Top, Bottom, colour_steps); };
    if (Method == HSL) { colourHSL = HslColor::LinearBlend(HslColor(Top), HslColor(Bottom), colour_steps); 
                         colourRGB = RgbColor(colourHSL) ;
                        };
 
    for( x = 0; x < count_x; x++) {
        uint16_t pixel = return_pixel(x,y, count_x);  // which pixel
        RgbColor original = strip->GetPixelColor(pixel); // get the orginal colour of it, RGB
        if (pixel >= strip->PixelCount()) break; // escape if out of bounds...

        AnimUpdateCallback animUpdate = [=](float progress)
            {
             // if (Method == HSL) {
             //   HslColor updatedColor = RgbColor::LinearBlend(HslColor(original), colourHSL ,  progress) ;
             //   strip->SetPixelColor(pixel, updatedColor);
             // } else if (Method == RGB) {
                RgbColor updatedColor = RgbColor::LinearBlend(original, colourRGB, progress) ;
                strip->SetPixelColor(pixel, updatedColor);
             // }; 
            };
            animator->StartAnimation(pixel, time, animUpdate);
    } // END of x ......

}  // END of y......
    
} // END of tom_bottom_fade


  //  case RANDOM_TOP_BOTTOM:
  //    Effect_Top_Bottom(TWOCOLOUR,HSL);
  //    break;
  //  case RANDOM_TOP_BOTTOM_LINEAR:
  //    Effect_Top_Bottom(TWOCOLOUR, RGB);
  //    break;    
  //  case SINGLE_COLOUR_FADE:
  //    Effect_Top_Bottom(SINGLECOLOUR, HSL);
  //    break;  
  //  case RANDOM_COLOUR_FADE:
  //    Effect_Top_Bottom(RANDOMSINGLECOLOUR,HSL);

RgbColor Manipulate_Hue(RgbColor original, float amount ) {

HslColor OriginalHSL = HslColor(original); 

float NewHue = OriginalHSL.H + amount; 

return RgbColor( HslColor(NewHue, OriginalHSL.S, OriginalHSL.L) ); 

}



RgbColor cache Return_Complementary(RgbColor Value) {
    HslColor original = HslColor(Value);
    original.H += 0.5; 
    if (original.H > 1.0) original.H -= 1.0;
    return RgbColor(original);
}

RgbColor cache Return_Split_Complementary(RgbColor Input, uint8_t position, float range) {
    HslColor original = HslColor(Input);
    float HUE = original.H + 0.5;
          HUE = HUE - (range / 2.0);
          HUE = HUE + ( float(position) * range ); 
          if (HUE < 0) HUE += 1;
          if (HUE > 1) HUE -= 1;
          original.H = HUE;   
          return RgbColor(original);
}





RgbColor cache Return_Analogous(RgbColor Value, uint8_t position, uint8_t total, float range) {
    HslColor original = HslColor(Value);
    float HUE = original.H;
    float HUE_lower = HUE - (range / 2.0);
    //float HUE_upper = HUE + (spread / 2.0);
    float steps = range / float(total); 
    HUE = HUE_lower + ( float(position) * float(steps) ); 
    if (HUE < 0) HUE += 1;
    if (HUE > 1) HUE -= 1;
    //Serial.print("")
    original.H = HUE;   
    return RgbColor(original);
}







void test_newanimations() {

uint8_t peak = 128; 


if (Current_Effect_State == PRE_EFFECT) Pre_effect(); 

if (Current_Effect_State == RUN_EFFECT) {

if (millis() - lasteffectupdate > WS2812_Settings.Timer) {

if (effectPosition == 0)
    {
        for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
        {
            uint16_t time = random(800, 1000);
            RgbColor originalColor = strip->GetPixelColor(pixel);
            RgbColor color = RgbColor(random(peak), random(peak), random(peak));

            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, color, progress);
                strip->SetPixelColor(pixel, updatedColor);
            };
            animator->StartAnimation(pixel, time, animUpdate);
        }
    }
    else if (effectPosition == 1)
    {
        for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
        {
            uint16_t time = random(600, 700);
            RgbColor originalColor = strip->GetPixelColor(pixel);

            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, RgbColor(0, 0, 0), progress);
                strip->SetPixelColor(pixel, updatedColor);
            };
            // start the animation
            animator->StartAnimation(pixel, time, animUpdate);
        }
    }
    effectPosition = (effectPosition + 1) % 2; // next effectPosition and keep within the number of effectPositions

lasteffectupdate = millis(); 
}
}

if (Current_Effect_State == POST_EFFECT) Post_effect(); 
}



void cache aaa(RgbColor a) {



}

void cache aaa(HslColor b) {


  
}

void cache aaa(String b) {


}
/*

void cache send_status () {

     if (opState == OFF) { 
    send_mqtt_msg("mode","off");
    send_mqtt_msg("effect","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }

    //delay(10);
    send_mqtt_msg("timer", String(WS2812interval));
    send_mqtt_msg("brightness", String(CurrentBrightness));



}
*/


