


void cache handle_root () {

if (WiFi.status() == WL_CONNECTED)  { 
  handle_WS2812(); 
 } else {
  handle_wifi();
  }


}

void cache setup_Plugin () {

  server.on("/ws2812", handle_WS2812);
  
  server.on("/lightsconfig", handle_lights_config);

uint8_t temp,tempb; 



    if (EEPROM.read(PixelPIN_enablebyte) == flagvalue) {
      pixelPIN = EEPROM.read(PixelPIN_address);
      if (isnan(pixelPIN)) pixelPIN = 2;
      }



    if (EEPROM.read(PixelCount_enablebyte) == flagvalue) 
     {     

         temp = EEPROM.read(PixelCount_address);
         tempb = EEPROM.read(PixelCount_address+1);

        if(isnan(temp)) temp = 0;
        if(isnan(tempb)) tempb = 0;

        pixelCount = temp*256+tempb;

      } 

      //Serial.print("/n Saved Colours are: ");

// SAVED SETTINGS..... 
Debug(F("Loading last op state from EEPROM = "));
current_loaded_preset = EEPROM.read(LastOpState_Address);
Debugln(current_loaded_preset);
if(current_loaded_preset > 10) current_loaded_preset = 0;

Load_LED_Defaults(current_loaded_preset); 


// initiate PLUGIN

initiateWS2812();

bool AutoRestartValue = EEPROM.read(AutoRestartEffectAddress);
bool Last = EEPROM.read(ON_OFF_State_Address); 

Debugf(F("AutoRestartValue = %u \n"), AutoRestartValue);
Debugf(F("Last ON/OFF = %u \n"), Last);

if ( AutoRestartValue == true ) 
    { 
       if (Last == true) { 
          //WS2812_mode_string("on");
          HoldingOpState = LastOpState; 
          Current_Effect_State = POST_EFFECT;
          Debugln(F("MODE returned to ON"));
        } else {
          //WS2812_mode_string("off");
          Current_Effect_State = POST_EFFECT; //  Set this to TERMINATE current effect.... 
          HoldingOpState = OFF; 
          Debugln(F("MODE returned to OFF"));
        }
    }


  Debugln(F("Loaded"));
  
  timer.setTimeout(10000, SendMQTTsettings);


}



void loop_Plugin () {

      ws2812();


}