


void cache handle_root () {

if (WiFi.status() == WL_CONNECTED)  { 
  handle_WS2812(); 
 } else {
  handle_wifi();
  }


}



//  handles MQTT response for plugin... 

void cache mqtt_plugin_handle( const MQTT::Publish& pub ) {


if ((pub.topic()).indexOf("/effect/set") > 0) WS2812_effect_string(pub.payload_string());
if ((pub.topic()).indexOf("/mode/set") > 0) WS2812_mode_string(pub.payload_string());
if ((pub.topic()).indexOf("/timer/set") > 0) WS2812timer_command_string(pub.payload_string());
if ((pub.topic()).indexOf("/brightness/set") > 0) WS2812_dim_string(pub.payload_string());
if ((pub.topic()).indexOf("/colour/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/color/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/loadpreset/set") > 0) WS2812_preset_string(pub.payload_string());
if ((pub.topic()).indexOf("/savepreset/set") > 0) { Save_LED_Settings(pub.payload_string().toInt()); };
if ((pub.topic()).indexOf("/toggle/set") > 0) WS2812_toggle_string(pub.payload_string());
if ((pub.topic()).indexOf("/autorestart/set") > 0) WS2812_autorestart_string(pub.payload_string());



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

Debugf("AutoRestartValue = %u \n", AutoRestartValue);
Debugf("Last ON/OFF = %u \n", Last);

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