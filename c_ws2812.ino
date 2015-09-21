

void cache ws2812 ()  // put switch cases here...
{

static unsigned long update_strip_time = 0;  //  keeps track of pixel refresh rate... limits updates to 33 Hrz.....
static unsigned long HoldingState_Failover = 0; 
static unsigned long timer_PixelPower = 0, timer_effect_tick = 0, send_fail_count = 0 ; 

if (millis() - timer_effect_tick >= timer_effect_tick_timeout ) { // timer_effect_tick_timeout is global, default 10msec

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
   case SNAKES:
      Snakes(0);
      break;
   case SNAKES_OVERLAP:
      Snakes(1);
   case STROBE:
      Strobe();
      break;  
    case UDP:
      UDPfunc();
      break;
    case RAINBOWCYCLE:
      Rainbowcycle();
      break;
    case SQUARESRANDOM:
      Squares(1);
      break;
    case SQUARES:
      Squares(0);
      break;
    case EQ1:
      eq1();
      break;
    case RANDOM:
      Random_colour();
      break;
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
    case DMX:
      DMXfunc();
      break;
   }

timer_effect_tick = millis();

} // end of if timer for effect generation.... 




if (  (millis() - update_strip_time > Pixel_Update_Freq) && ( opState != ADALIGHT || opState != UDP ) ) {
  
  if (Enable_Animations) {
    if ( animator->IsAnimating() ) animator->UpdateAnimations(100); 
  }

    strip->Show();  // takes 6ms with 200, take 12ms with 400 ----> so 100 takes 3ms. 
    update_strip_time = millis();
  
  } 



  if (millis() - timer_PixelPower > 10000) { 
       power = getPixelPower();
       timer_PixelPower = millis();
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
    Debugln(F("HoldingState Failover activated"));
  }

}

if(LED_Settings_Changed) {
  Save_LED_Settings(0);                    // Save current settings to EEPROM
  current_loaded_preset_changed = true;    // Change flag to say current effect has changed
  send_mqtt_msg(F("Preset"), "0");            // Update the MQTT preset selection to 0
  LED_Settings_Changed = false;            // Reset settings changed. 
}

//  Deal with pause... only when effect is in the run phase... 
if (paused && Current_Effect_State == RUN_EFFECT) {
      Current_Effect_State = EFFECT_PAUSED ;
      if (Enable_Animations) animator->Pause();
    } else if (!paused && Current_Effect_State == EFFECT_PAUSED) {
      Current_Effect_State = RUN_EFFECT ;
      if (Enable_Animations) animator->Resume(); 
    }


} // end of ws2812 function 


void  cache handle_WS2812 () { // handles the web commands...
//animator->Pause();
String paused_string, selected; 
bool updateLEDs = false;
String randomcolours = " " ; 
 // Serial.print("Current Preset = ");
 // Serial.println(CurrentPreset);
 if (server.args() > 0) {
 if (server.hasArg("mode")) WS2812_mode_string(server.arg("mode"));
 if (server.hasArg("preset")) WS2812_preset_string(server.arg("preset"));
 if ((server.arg("dim") != String(WS2812_Settings.Brightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812_Settings.Timer)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 //if ((server.arg("anispeed") != String(CurrentAnimationSpeed)) && (server.arg("anispeed").length() != 0))  AnimationSpeed_command_string(server.arg("anispeed"));
 if (server.hasArg("paused")) {
      paused = (server.arg("paused")).toInt();
      if (Enable_Animations) {  
        if (paused) animator->Pause(); 
        if (!paused) animator->Resume(); 
        }
      }

 if (server.hasArg("rgbpicker"))  { 
    WS2812_Set_New_Colour(server.arg("rgbpicker"));
  }

  if (server.hasArg("presetsave")) Save_LED_Settings(  server.arg("presetsave").toInt() );
  if (server.hasArg("modedrop"))  WS2812_mode_string(server.arg("modedrop"));
  if (server.hasArg("palettedrop"))   { 
      WS2812_Settings.Palette_Choice = server.arg("palettedrop").toInt(); 
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      Effect_Refresh = true;         // refresh the LEDs... 
      current_loaded_preset_changed = 1; // so user knows settings have changed. 
      
    }

    if (server.arg("random") == "1")  { WS2812_Settings.Random = 1 ; LED_Settings_Changed = true; } ;
    if (server.hasArg("plain"))  { WS2812_Settings.Random = 0 ; LED_Settings_Changed = true; } ; 



  } // END of Args for web page..
      //----  having this under here works better as the page gets updated before the request data is fired back!

      if (paused) { 
        paused_string = F("<a href='/ws2812?paused=0'>PLAY</a> " ); 
      } else {
        paused_string = F("<a href='/ws2812?paused=1'>PAUSE</a>" ); 
      }

  buf = insertvariable ( FPSTR(webpage_ws2812_main_1), String(deviceid));
  buf = insertvariable ( buf, String(deviceid));
  buf = insertvariable ( buf, F("<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>"));
  buf = insertvariable ( buf, paused_string);

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, F("text/html"), "");
    WiFiClient client = server.client();

    //server.sendContent(buf);
    server.client().print(buf);

 buf = " ";

//  MODE
for (uint8_t k=0; k < numberofmodes; k++ ) {
    if (HoldingOpState == k) { 
        selected = "' selected "; 
      } else selected = "' "; 
    buf += "<option value='" + String(k) + selected + ">" + String(k) + ". " + MODE_STRING[k] + "</option>";
    }

    buf += F("</select>");
    buf += F("</form>");
    server.client().print(buf);
    buf = " "; 



    server.client().print(FPSTR(webpage_ws2812_main_2));

for (uint8_t k = 0; k < numberofpalettes; k++ ) {
    if (WS2812_Settings.Palette_Choice == k) { 
        selected = F("' selected "); 
      } else selected = F("' "); 

  buf += "<option value='" + String(k) + selected + ">" + String(k) + ". " + PALETTE_STRING[k] + "</option>";
   // httpbuf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";
  }

  buf += F("</select>");
  buf += F("</form>");


    //server.sendContent(buf);

    server.client().print(buf);

    if (WS2812_Settings.Random == 1 ) randomcolours = "checked"; else randomcolours = " ";  


  buf = insertvariable ( FPSTR(webpage_ws2812_main_3), randomcolours);  //WebRGBcolour
  
  server.client().print(buf);

  buf = insertvariable ( FPSTR(webpage_ws2812_main_4), WebRGBcolour);  //WebRGBcolour
  
    //server.sendContent(buf);
    server.client().print(buf);

    buf = insertvariable ( FPSTR(webpage_ws2812_main_5), String(WS2812_Settings.Brightness)); 
    buf = insertvariable ( buf, String(WS2812_Settings.Timer)); 

    String loaded_var; 

  if (current_loaded_preset == 0)  { loaded_var = F("none") ; } else { loaded_var = String(current_loaded_preset) ; } ; 
  if (current_loaded_preset_changed) { loaded_var += F(" (unsaved changes)") ; }; 


  buf = insertvariable ( buf, loaded_var ); 
  buf = insertvariable ( buf, String(current_loaded_preset) ); 
  buf = insertvariable ( buf, String(power) ); 
  buf += FPSTR(htmlendstring); 
 
  server.client().print(buf);

}


void cache WS2812_autorestart_string(String Value) {
  
  Value.toLowerCase();

  bool CurrentRestartValue = EEPROM.read(AutoRestartEffectAddress);
  bool changed = false; 

if ( (Value == "on" || Value == "yes") && CurrentRestartValue == false ) 
    { 
      EEPROM.write(AutoRestartEffectAddress, 1) ;
      EEPROM_commit_var = true; 
      Serial.print(F("Auto Restart Set to True"));
      send_mqtt_msg(F("AutoRestart"),F( "Auto restart set to true")); 
      changed = true; 
    }

if ( (Value == "off" || Value == "no") && CurrentRestartValue == true ) 
    { 
      EEPROM.write(AutoRestartEffectAddress, 0) ;
      EEPROM_commit_var = true; 
      Serial.print(F("Auto Restart Set to False"));
      send_mqtt_msg(F("AutoRestart"), F("Auto restart set to false")); 
      changed = true; 
    }
 
if (changed == false) send_mqtt_msg(F("AutoRestart"), F("Auto restart unchanged")); 


}


void cache WS2812_toggle_string(String Value) {


      if (Value == "toggle") {

       lasteffectupdate = 0;
       current_loaded_preset_changed = true; 


        if (opState == OFF)  {
          //Serial.println("OFF, so turned ON"); 
          HoldingOpState = LastOpState;
          Current_Effect_State = POST_EFFECT;
        } else {

          current_loaded_preset++;  // advance to next saved.... 

          if (current_loaded_preset > 10) current_loaded_preset = 1;

          for (uint8_t i = 0; i < 10; i++ ) { // go through 10 saved ONLY.....
              uint16_t address = START_address_settings + (32 * current_loaded_preset); 
              uint8_t saved = EEPROM.read(address);
              if (saved != 0) break; 
              current_loaded_preset++;        
              if (current_loaded_preset > 10) current_loaded_preset = 1;
          }

          WS2812_preset_string(String(current_loaded_preset));
        }
    }
}




void cache WS2812_preset_string(String Value) {

        lasteffectupdate = 0;
        uint8_t preset = Value.toInt();
        if (preset > 10) preset = 1;
        if (preset < 0 ) preset = 1; 
        Load_LED_Defaults (preset) ; 
        current_loaded_preset = preset;
        current_loaded_preset_changed = false; 
        HoldingOpState =  LastOpState;
        Current_Effect_State = POST_EFFECT; 
        send_current_settings();  // this prevents settings being sent IMMEDIATELY after boot...
        Save_LED_Settings(0);         // this will save the ON state to EEPROM... and the last state.... 
}

void cache send_current_settings () {

   if (HoldingOpState == OFF) { 
    send_mqtt_msg(F("mode"),F("off"));
  } else { 
    send_mqtt_msg(F("mode"), F("on"));
  }

    send_mqtt_msg(F("timer"), String(WS2812_Settings.Timer));
    send_mqtt_msg(F("brightness"), String(WS2812_Settings.Brightness));
    send_mqtt_msg(F("Preset"), String(current_loaded_preset)); 
    send_mqtt_msg(F("effect"), MODE_STRING[HoldingOpState] ); // has to be the effect of the future.. not the current one :)

}



void  cache  WS2812_dim_string (const String Value)
{

      int a = Value.toInt();
      if (a > 255) a = 255;
      if (a < 0) a = 0;
      WS2812_Settings.Brightness = a; 
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      Effect_Refresh = true;                  // flag current effect that settings have changed
      Debug(F("Brightness set to: "));
      Debugln ( String(WS2812_Settings.Brightness) );
      send_mqtt_msg(F("brightness"), String(WS2812_Settings.Brightness));
}


void cache WS2812_effect_string (String request) {

      for (uint8_t i = 0; i < numberofmodes; i++ ) {

        if (request == String(MODE_STRING[i])) {
          HoldingOpState = LastOpState = (operatingState)i; // assign selection to holdingopstate... 
          Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
          LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
          break; 
        }
      } 
      send_current_settings(); 
}

void  cache WS2812_mode_string (String Value)

{

  if(Value == "refresh" ) {     Effect_Refresh = true;  return; } ; 
  if (paused) paused = false; // this sets it back to play, if paused when a mode change occurs...
  if (Value.toInt() != 0) {  // if the numerical mode does not equal 0 which is off....

      uint8_t chosen_mode = Value.toInt(); // turn it to number...
      HoldingOpState = (operatingState)chosen_mode; // assign selection to holdingopstate... 
      Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
      LastOpState = (operatingState)chosen_mode;
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 

  } else {

      Value.toLowerCase();

  if (Value == "off") { 
      if (opState != OFF) { 
          Current_Effect_State = POST_EFFECT; //  Set this to TERMINATE current effect.... 
          HoldingOpState = OFF;           
      } 
  }

  if (Value == "on") { 
      HoldingOpState = LastOpState; 
      Current_Effect_State = POST_EFFECT; 
  } ; 

  if (Value == "pause") paused = true;
  if (Value == "play") paused = false; 

  } // end of else not a number... .... 

    if(HoldingOpState == OFF) { 
      EEPROM.write(ON_OFF_State_Address,0); 
      //Debugln("ON_OFF_State_Address (write) = 0");
    } else {
      EEPROM.write(ON_OFF_State_Address,1); 
      //Debugln("ON_OFF_State_Address (write) = 1");
    } 


   if (HoldingOpState == OFF) { 
    send_mqtt_msg(F("mode"),"off");
  } else { 
    send_mqtt_msg(F("mode"), "on");
  }

      

    //send_mqtt_msg("effect", MODE_STRING[HoldingOpState]); 
    EEPROM_commit_var = true;
    //if (millis() > 11000) 
    send_current_settings(); 
}


void cache WS2812_Set_New_Colour (String instruction) {

    //  lasteffectupdate = 0; 
      WebRGBcolour = instruction;
      WS2812_Settings.Color = HEXtoRGB(instruction);
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      Effect_Refresh = true;         // flag current effect that settings have changed 
      current_loaded_preset_changed = 1; 
      
      send_mqtt_msg(F("colour"), WebRGBcolour); 


}


void cache WS2812timer_command_string (String Value)

{
    Effect_Refresh = true;                  // flag current effect that settings have changed 
    current_loaded_preset_changed = 1; 

  //  lasteffectupdate = 0;
    WS2812_Settings.Timer = Value.toInt();
    send_mqtt_msg(F("timer"), Value); 
    LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
    Debugf("Timer = %u \n", WS2812_Settings.Timer);
}



RgbColor cache dim(RgbColor original) {

  if (WS2812_Settings.Brightness == 255) return original;
  if (WS2812_Settings.Brightness == 0) return RgbColor(0,0,0);

  #ifdef GAMMA_CORRECTION
    uint8_t modified_brightness = GAMMA_2811[WS2812_Settings.Brightness]; 
  #else
    uint8_t modified_brightness = WS2812_Settings.Brightness ;
  #endif

    HslColor originalHSL = HslColor(original); 

    originalHSL.L =  originalHSL.L   * ( float(modified_brightness) / 255.0 ) ; 
    
    return RgbColor( HslColor(originalHSL.H, originalHSL.S, originalHSL.L )  );


}








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



void cache Pre_effect() {

    Current_Effect_State = RUN_EFFECT; 
   // lasteffectupdate = 0; 
   // Effect_Refresh = false; 
    effectPosition = 0; // reset effect position... 
}

void cache Post_effect() {

    if (Enable_Animations) {
      if ( animator->IsAnimating() ) {
         for (uint16_t i = 0; i < strip->PixelCount();i++ ) {
           animator->StopAnimation(i);
          }
      }
    } else strip->ClearTo(0,0,0); 

    Current_Effect_State = PRE_EFFECT; 
    opState = HoldingOpState; 

}





void cache initiateWS2812 ()
{
  ChangeNeoPixels(pixelCount, pixelPIN); // initial setup
  Pixel_Update_Freq = 10 + ( pixelCount * 30 ) / 1000 ;   
  if (Pixel_Update_Freq < MINIMUM_REFRESH ) Pixel_Update_Freq = MINIMUM_REFRESH;  
  Debugf("Update frequency = %u\n", Pixel_Update_Freq);
  strip->Begin();
  SetRandomSeed();
  
}






void cache SetRandomSeed()
{

  randomSeed(ESP.getCycleCount());

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
      animator->FadeTo( 2000,  dim(Wheel(random(0,255) ) ) ) ;
      lasteffectupdate = millis(); 
};


    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
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

        Debugln(F("Pixel count changed...")); 
        int a = pixelCount / 256;
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
        //EEPROM_commit_var = true;
        EEPROM.commit(); // actually save changes to avoid having a boot loop....  and unable to exit... 
        Debugln(F("WS2812 Settings Updated.")); 
        }


  if (strip != NULL)
  {           
  strip->ClearTo(RgbColor(0,0,0));    // this set the strip to off before changing pixel amount...    
  strip->Show();  
  delete strip;
  
  }
//Debugln("2");

  strip = new NeoPixelBus(count, pin);
//Debugln("3");

    if (count > ANIMATION_LIMIT) { 
      Enable_Animations = false; 
      //animator = NULL; 
      Debugln(F("Animations Disabled"));
    } else {

      if (animator != NULL) delete animator; // has to be here...
      animator = new NeoPixelAnimator(strip); //  this allows for massive strings with NO animator...
      Enable_Animations = true; 
      Debugln(F("Animations Enabled"));

    }
//Debugln("4");

  pixelsPOINT = (uint8_t*)strip->Pixels(); ///  used for direct access to pixelbus buffer...


  if (strip->PixelCount() < 10) WS2812_Settings.Total_X = 1; // can't remember what this does...  sets x width i think... 

}



int cache getPixelPower () {
 int brightnesstally = 0;

 for (int i = 0;i < pixelCount; i++) {
  RgbColor colour = strip->GetPixelColor(i);
  int brightness = colour.CalculateBrightness();
  brightness = map(brightness,0,255,0,60);
  brightnesstally = brightnesstally + brightness;
}

return brightnesstally;
} 





void cache handle_lights_config() {

bool updateLEDs = false; 

   if (server.args() > 1 || ( server.hasArg("preset") == false && server.args() == 1) ) { 
   //   lasteffectupdate = 0; 
   //   Random_func_timeout = 0; 
      LED_Settings_Changed = true;   // Calls the modifier to save all the current settings to EEPROM... 
      current_loaded_preset_changed = 1; 
      
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
   if (server.hasArg("var10"))WS2812_Settings.Effect_Option = server.arg("var10").toInt();   //  Serial.println("Var 10 updated"); }; 
  
   if (server.hasArg("preset")) Save_LED_Settings(server.arg("preset").toInt() );
  
   // String a = ESP.getFlashChipSizeByChipId(); 
   if (server.arg("reset") == "true") { Set_Defaults();   EEPROM_commit_var = true; } 
   if (server.arg("resetall") == "true") Save_All_Defaults();


  if (server.hasArg("leds")) {
    pixelCount = server.arg("leds").toInt();
    updateLEDs = true;
  }

   if (server.hasArg("ledpin")) {
    pixelPIN = server.arg("ledpin").toInt();
    updateLEDs = true;
  }



    buf = insertvariable ( FPSTR(webpage_ws2812_config_1), String(deviceid));
    buf = insertvariable ( buf, String(deviceid));
    
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");

    WiFiClient client = server.client();
    //server.sendContent(buf);
    server.client().print(buf); 
    buf = " "; 

  //buf += "Select Mode <select name='modedrop' onchange='this.form.submit();'>";

  for (uint8_t k=0; k < 10; k++ ) {
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
  
    //server.sendContent(buf);
    server.client().print(buf); 
    
  buf = FPSTR(webpage_ws2812_config_2);
  buf += FPSTR(htmlendstring); 
  buf = insertvariable ( buf, String(current_loaded_preset)); 
  buf = insertvariable ( buf, String(pixelCount)); 
  //buf = insertvariable ( buf, String(pixelPIN));  // removed until other method of driving pixels is found. 


    //server.sendContent(buf);
    server.client().print(buf); 

if (updateLEDs) { initiateWS2812(); updateLEDs = false;};




}



int16_t cache return_pixel(const uint16_t x, const uint16_t y, const uint16_t total_in_x) {
  uint16_t a = (total_in_x * y) + x ;  // the added 1 is to allow a void pixel to equal 0. 
  if ( a > strip->PixelCount()  ) return -1;   // need the one otherwise it will stop one short of the end..
  return a; 
}


uint16_t cache return_total_y(const uint16_t total_in_x) {
  uint16_t y = pixelCount / total_in_x; 
  uint8_t remainder = pixelCount % total_in_x;
  if (remainder > 0) { y++; } 
  return y;
};


//  Returns a pixel number... starting in bottom left...
int16_t cache return_shape_square(const uint16_t first_pixel_x, const uint16_t first_pixel_y , const uint16_t desired_pixel, const uint8_t grid_size, const uint16_t total_in_x) {

  int16_t pixel ; 
  uint16_t pixel_x, pixel_y, total_y;
  uint8_t row, row_left  ;
  row = desired_pixel / grid_size;  //     which row in square
  row_left = desired_pixel % grid_size; // how many pixels are left to go in the row.
  pixel_y = first_pixel_y + row;   //      where y pixel is on whole grid
  pixel_x = first_pixel_x + row_left;  //  where x pixel is on whole grid

  if (pixel_y >= return_total_y(total_in_x)) return  -1 ; 
  if (pixel_x >= total_in_x ) return -1 ; 

  pixel = return_pixel(pixel_x, pixel_y, total_in_x);

  //Debugf("(%u,%u) ? %u -> row = %u, row_left = %u ==> (%u,%u) ==> #%u  | %u ?x %u, %u ?y %u \n", first_pixel_x, first_pixel_y, desired_pixel, row, row_left, pixel_x, pixel_y, pixel, pixel_x, total_in_x, pixel_y, total_y) ;

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



}




RgbColor cache Return_Palette (RgbColor Input) {

      uint8_t random_choice; 
      static uint8_t last_random_choice; 
      uint8_t number_of_choices; 

switch (WS2812_Settings.Palette_Choice) {
          case ALL:                // 0 
            number_of_choices = 255; 
           // return Wheel(random(255)); 
            break;
          case COMPLEMENTARY:      // 1
            number_of_choices = 2; 
            break;
          case MONOCHROMATIC:     //  2
                //Debugln("MONOCHROMATIC"); 
            number_of_choices = 2; 
            break;
          case ANALOGOUS:         // 3
            number_of_choices = WS2812_Settings.Palette_Number; 
            break; 
          case SPLITCOMPLEMENTS:  // 4
            number_of_choices = 3; 
            break;
          case TRIADIC:           // 5
            number_of_choices = 3; 
          break;
          case TETRADIC:          // 6
            number_of_choices = 4;
          break;
          case MULTI:          // 7
            number_of_choices = WS2812_Settings.Palette_Number; 
          break;
      }

do {
    random_choice = random(0, number_of_choices );
  } while (random_choice == last_random_choice) ;
  
  last_random_choice = random_choice ; 

//Debugf("Random Pallete Choice = %u \n", random_choice);

      Return_Palette (Input, random_choice) ;
}

/////////////////////
//
//      This function depends on all the globals for WS2812_Settings.*  
//      
//
//
/////////////////////

RgbColor cache Return_Palette (RgbColor Input, uint16_t Index) {
float Palette_Range = 10 / float(WS2812_Settings.Palette_Range);
RgbColor Output = RgbColor(0,0,0); 

switch (WS2812_Settings.Palette_Choice) {

          case ALL:                // 0 
                Index = Index % 255; 
           //     if (Index == 0 ) Output = Input; 
             //   if (Index == 1 ) 
         //       do { 
                  Output = Wheel(Index); 
          //      } while ( Output == Input)
                
                break;

          case COMPLEMENTARY:      // 1
                Index = Index % 2; 
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Return_Complementary(Input);
                break;

          case MONOCHROMATIC:     //  2
                //Debugln("MONOCHROMATIC"); 
                Output = Input; 
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
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Return_Multiple( Input,  1 , 3);
                if (Index == 2 ) Output = Return_Multiple( Input,  2 , 3);


          break;

          case TETRADIC:          // 6
                //Debugln("TETRADIC"); 
                Index = Index % 4; 
                if (Index == 0 ) Output = Input; 
                if (Index == 1 ) Output = Return_Multiple( Input,  1 , 4);
                if (Index == 2 ) Output = Return_Multiple( Input,  2 , 4);
                if (Index == 2 ) Output = Return_Multiple( Input,  3 , 4);

          break;

          case MULTI:          // 7
                //Debugln("TETRADIC"); 
                Index = Index % WS2812_Settings.Palette_Number; 
                //if (Index == 0 ) Output = Input; 
                Output = Return_Multiple( Input,  Index , WS2812_Settings.Palette_Number );

          break;

          case WHEEL:          // 8
                Index = Index % 255;  

                Output = Wheel(Index);

          break;




      }
      //yield();
      return Output; 

}




// Overloaded func for topbottom fade...
void cache top_bottom_fade( const RgbColor Top, const RgbColor Bottom, const uint16_t count_x) {

  uint16_t timer = map ( WS2812_Settings.Timer , 1, 2000, 500 , 65000 );

  top_bottom_fade(Top,Bottom, count_x, timer, HSL);
}

void cache top_bottom_fade( const RgbColor Top, const RgbColor Bottom, const uint16_t count_x, const uint16_t time) {

  top_bottom_fade(Top,Bottom, count_x, time, HSL);
}



void cache top_bottom_fade( const RgbColor Top, const RgbColor Bottom, const uint16_t count_x, const uint16_t animation_time, const  BlendMethod Method) {

uint16_t x,y;
float colour_steps; 
uint16_t total_y = return_total_y(count_x); // get total number of rows
RgbColor colourRGB;
HslColor colourHSL; 

for (y = 0; y < total_y; y++) {

    colour_steps = float(y) / float(total_y);    // give the % complete of blend.... 

    if (y == 0) colour_steps = 0.0f; // this stets the top blend.  to ensure a 0%
    if (y == total_y - 1) colour_steps = 1.0f;  // this sets the bottom blend to ensure it hits 100% 

    if (Method == RGB) { colourRGB = RgbColor::LinearBlend(Top, Bottom, colour_steps); };
    if (Method == HSL) { colourRGB = HslColor::LinearBlend(Top, Bottom, colour_steps); };


    for( x = 0; x < count_x; x++) {

        int16_t pixel = return_pixel(x, y, count_x);  // which pixel  


        if (pixel > -1) { // is the pixel valid


        RgbColor original = strip->GetPixelColor(pixel); // get the orginal colour of it, RGB
        //if (pixel >= strip->PixelCount()) break; // escape if out of bounds...

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
            animator->StartAnimation(pixel, animation_time, animUpdate);

        } // end of valid pixel....
    } // END of x ......
//    Debugln();
}  // END of y......
    
} // END of tom_bottom_fade




RgbColor cache Return_Complementary(RgbColor Value) {
    HslColor original = HslColor(Value);
    original.H += 0.5; 
    if (original.H > 1.0) original.H -= 1.0;
    return RgbColor(original);
}

RgbColor cache Return_Split_Complementary(RgbColor Input, uint16_t position, float range) {
    HslColor original = HslColor(Input);
    float HUE = original.H + 0.5;
          HUE = HUE - (range / 2.0);
          HUE = HUE + ( float(position) * range ); 
          if (HUE < 0) HUE += 1;
          if (HUE > 1) HUE -= 1;
          original.H = HUE;   
          return RgbColor(original);
}

RgbColor cache Return_Analogous(RgbColor Value, uint16_t position, uint16_t total, float range) {

    HslColor original = HslColor(Value);
    float HUE = original.H;
    float HUE_lower = HUE - (range / 2.0);
    float steps = range / float(total); 
    HUE = HUE_lower + ( float(position) * float(steps) ); 
    if (HUE < 0) HUE += 1;
    if (HUE > 1) HUE -= 1;
    original.H = HUE;   
    return RgbColor(original);
}

RgbColor cache Return_Multiple(RgbColor Value, uint16_t position, uint16_t total) {

    HslColor original = HslColor(Value);
    float HUE = original.H;
    float HUE_gap = 1.0 / float(total); // HUE - (range / 2.0);
    HUE = HUE + (position * HUE_gap); 
    if (HUE > 1) HUE -= 1;
    original.H = HUE;   
    return RgbColor(original);   

}








void cache Save_All_Defaults() {

  Set_Defaults(); 

  for (uint8_t location = 0; location < 11; location++) {

      uint16_t address = START_address_settings + (32 * location); 
      uint8_t byteswritten = EEPROM_writeAnything( address, WS2812_Settings); 

  }

  Serial.println(F("Default settings saved"));
  EEPROM_commit_var = true; 
}


void cache Set_Defaults() {

   WS2812_Settings.SavedOpState     = 0 ;
   WS2812_Settings.Timer            = 1000 ;
   WS2812_Settings.Animationspeed   = 0 ; 
   WS2812_Settings.Brightness       = 255;
   WS2812_Settings.Color            = RgbColor(255,0,0); 
   WS2812_Settings.Palette_Choice   = 0; 
   WS2812_Settings.Palette_Range    = 20;  
   WS2812_Settings.Palette_Number   = 5 ;   
   WS2812_Settings.Random           = 1 ;  
   WS2812_Settings.Time_Stretch     = 1;   
   WS2812_Settings.Total_X          = 1;  
   WS2812_Settings.Effect_Count     = 1;  
   WS2812_Settings.Effect_Min_Size  = 1; 
   WS2812_Settings.Effect_Max_Size  = 1;  
   WS2812_Settings.Effect_Option    = 1; 
   WS2812_Settings.CheckSum         = 0; 

}



XY return_adjacent(const XY &Input) { ; // , const uint8_t direction) {
XY Output; 
bool OK;
const uint16_t Total_Y = return_total_y ( WS2812_Settings.Total_X ); 
const uint16_t Total_X = WS2812_Settings.Total_X; 

            do {  
                uint16_t X = Input.x;
                uint16_t Y = Input.y;
                const uint8_t direction = random(8); 
                OK = false; 

                if (direction == 0 || direction == 3 || direction == 5 )  (X > 0)? X-- : X = Total_X - 1  ;
                if (direction == 0 || direction == 1 || direction == 2 )  (Y < Total_Y - 1) ? Y++ : Y = 0 ;
                if (direction == 2 || direction == 4 || direction == 7 )  (X < Total_X - 1) ? X++ : X = 0 ; 
                if (direction == 5 || direction == 6 || direction == 7 )  (Y > 0) ? Y-- : Y = Total_Y - 1 ; 

               // if ()
                //  direction generated...  Now check if it is valid, assigns it to output.... 
                if ( X  <  Total_X && 
                     Y  <  Total_Y && 
                    return_pixel(X, Y, Total_X) > -1 )
                {
                  OK = true;
                  Output.x = X; 
                  Output.y = Y;
                }
              
            } while ( !OK ) ; // 

return Output; 

}



XY cache toXY(uint8_t x, uint8_t y ) {

    XY grid;
    grid.x = x;
    grid.y = y;

  return grid; 
}

bool random_colour_timer (const uint32_t _time) {

  if (!WS2812_Settings.Random) return false;  // return if random colour selection is disabled.  

  static uint32_t _last = 0; 

    if (millis() - _last > _time) {

        RgbColor oldcolour = WS2812_Settings.Color;

#ifdef USE_COLOUR_OPERATORS
        do {
          WS2812_Settings.Color = Wheel(random(255)); 
        } while (WS2812_Settings.Color == oldcolour) ;
#else
        WS2812_Settings.Color = Wheel(random(255)); 

#endif

        _last = millis();   

        return true;
    }

    return false; 

}



void cache Save_LED_Settings (uint8_t location) {

  WS2812_Settings.SavedOpState = (uint8_t)LastOpState; 
  Debugf("Saved opState = %u \n", WS2812_Settings.SavedOpState); 
  
  if ( location < 0 || location > 10) return;  // check valid storage range. 

  uint16_t address = START_address_settings + (32 * location); 
  uint8_t byteswritten = EEPROM_writeAnything( address, WS2812_Settings); 

     if (location == 0 ) EEPROM.write(LastOpState_Address, 0); // if were no longer on preset.. but defualt back to 0

     EEPROM_commit_var = true;

     if (location != 0)   { 
      current_loaded_preset_changed = false; 
      current_loaded_preset = location; 
    }

}


void cache Load_LED_Defaults (uint8_t location) {

if ( location < 0 || location > 10) location = 0; 
uint16_t address = START_address_settings + (32 * location); 
int bytes_read = EEPROM_readAnything(address, WS2812_Settings);

if (WS2812_Settings.SavedOpState != 0 ) { LastOpState = (operatingState)WS2812_Settings.SavedOpState; };  // Stops last opstate being over written by an OFF..



Serial.print(F("Settings loaded ------- "));
Serial.print(bytes_read) ;
Serial.print(F(" Bytes read"));
Serial.print(F(" ------- \n") ); 

  Debug(F("Op State        ==> "));
Debugln(WS2812_Settings.SavedOpState);
  Debug(F("Timer           ==> "));
Debugln(WS2812_Settings.Timer);
  Debug(F("Animation speed ==> "));
Debugln(WS2812_Settings.Animationspeed);
  Debug(F("Brightness      ==> "));
Debugln(WS2812_Settings.Brightness);
  Debug(F("Colour          ==> "));
Debug(WS2812_Settings.Color.R);
Debug(F("  ")); 
Debug(WS2812_Settings.Color.G);
Debug(F("  ")); 
Debugln(WS2812_Settings.Color.B);
  Debug(F("Palette         ==> "));
Debugln(WS2812_Settings.Palette_Choice);
  Debug(F("Palette Range   ==> "));
Debugln(WS2812_Settings.Palette_Range);
  Debug(F("Number Colours  ==> "));  
Debugln(WS2812_Settings.Palette_Number);
  Debug(F("Random          ==> "));  
Debugln(WS2812_Settings.Random);
  Debug(F("Time Stretch    ==> "));
Debugln(WS2812_Settings.Time_Stretch);
  Debug(F("Total X         ==> "));
Debugln(WS2812_Settings.Total_X);
  Debug(F("Effect Count    ==> "));
Debugln(WS2812_Settings.Effect_Count);
  Debug(F("Effect Min Size ==> "));
Debugln(WS2812_Settings.Effect_Min_Size);
  Debug(F("Effect Max Size ==> "));
Debugln(WS2812_Settings.Effect_Max_Size);
  Debug(F("Effect Option   ==> "));
Debugln(WS2812_Settings.Effect_Option);
Debugln(F("End-------------------------")); 

        String Rstring, Gstring, Bstring;
        uint8_t R, G, B; 
        R = WS2812_Settings.Color.R;
        G = WS2812_Settings.Color.G;
        B = WS2812_Settings.Color.B;


      if (R == 0) { Rstring = "00" ;
        } else if  ( R < 16 && R > 0 ) {
           Rstring = "0" + String(R,HEX); 
        } else {
           Rstring = String(R,HEX); 
        } 

      if (G == 0) { Gstring = "00" ;
        } else if  ( G < 16 && G > 0 ) {
           Gstring = "0" + String(G,HEX); 
        } else {
           Gstring = String(G,HEX); 
        } 

      if (B == 0) { Bstring = "00" ;
        } else if  ( B < 16 && B > 0 ) {
           Bstring = "0" + String(B,HEX); 
        } else {
           Bstring = String(B,HEX); 
        } 


      Debug(F("Colour debugging (String) R=")) ; 
      Debug(Rstring); 
      Debug(F(", G="));
      Debug(Gstring);
      Debug(F(", B="));
      Debug(Bstring); 
      Debug(F(" ==> ")); 
      WebRGBcolour = Rstring + Gstring + Bstring; 
      WebRGBcolour.toUpperCase();

      Debugln(WebRGBcolour); 


      if (WS2812_Settings.Time_Stretch == 0) WS2812_Settings.Time_Stretch = 1; 


if (location != 0) {

    EEPROM.write(LastOpState_Address, location);    
    EEPROM_commit_var = true; 
}


}


void SendMQTTsettings () {
  
  String message = F("effect:"); 

  for (uint8_t i = 0; i < numberofmodes; i++)  {
    message += String(MODE_STRING[i]); 
    if (i < (numberofmodes - 1)) { message += ","; }; 
   }

    send_mqtt_msg(F("effectlist"), message); 

    send_current_settings(); 

  
}
