void LoadParams()
{
  
  if (EEPROM.read(deviceidAddressbyte) == flagvalue) eeprom_read_string(deviceidAddress, deviceid, BUFSIZE);
  if (EEPROM.read(ssidAddressbyte) == flagvalue) eeprom_read_string(ssidAddress, ssid , BUFSIZE);
  if (EEPROM.read(passwordAddressbyte) == flagvalue) eeprom_read_string(passwordAddress, password, BUFSIZE);
  if (EEPROM.read(mqttAddressbyte) == flagvalue) 
  {
    //EepromUtil::eeprom_read_string(mqttAddress, mqttserver, BUFSIZE);
     for (int i=0; i<4; i++) {  // SAVE NEW TO EEPROM
    MQTTserver[i] = EEPROM.read( mqttAddress+i);
   }
   
   mqttserver_string = String(MQTTserver[0]) + "." + String(MQTTserver[1]) + "." + String(MQTTserver[2]) + "." + String(MQTTserver[3]) ; 

   //Serial.print("SAVED MqqttServer: ");
   //Serial.println(mqttserver);
  }
  
  if (EEPROM.read(MQTTenabledbyte) == flagvalue) {
  	MQTT_enabled = true;
  } else {
  	MQTT_enabled = false;
  }





  if (EEPROM.read(DEBUGenabledbyte) != 0) {
  		if (EEPROM.read(DEBUGenabledbyte) == 1) {
  			debug_command("timer");
  		}
  
	} else {
	  	DEBUG = false;

	}

}


void EEPROM_wipe() {

for (int i = 0; i < 512; i++) EEPROM.write(i,0);
EEPROM.commit();
Serial.print("EEPROM WIPED");
delay(100);
}


void Load_LED_Defaults (uint8_t location) {
uint8_t temp,tempb; 
// START_address_settings

uint16_t address = START_address_settings + (32 * location); 


  // START_address_settings = 160; 

// 1 --------------------------------   LAST OP STATE ------------------------
     temp = EEPROM.read(address++);

    if (isnan(temp)) temp = 0;

    if (temp != 0 ) {  LastOpState = (operatingState)temp; };  // Stops last opstate being over written by an OFF..
// 2 --------------------------------   Timer ---------------------------------
    
     temp = EEPROM.read(address++);
     tempb = EEPROM.read(address++);
        if(isnan(temp)) temp = 0;
        if(isnan(tempb)) temp = 0;
        WS2812interval = temp*256+tempb;
        if (isnan(WS2812interval) || WS2812interval == 0) WS2812interval = 2000;

// 3 --------------------------------  Animation speed -------------------------

     temp = EEPROM.read(address++);
     tempb = EEPROM.read(address++);

        if(isnan(temp)) temp = 0;
        if(isnan(tempb)) temp = 0;
      CurrentAnimationSpeed = temp*256+tempb;
        
        if (isnan(CurrentAnimationSpeed) || CurrentAnimationSpeed == 0) CurrentAnimationSpeed = 2000;


// 4--------------------------------  Brightness -------------------------

        CurrentBrightness = EEPROM.read(address++);
        if (isnan(CurrentBrightness) || CurrentBrightness == 0) CurrentBrightness = 255;

// 5--------------------------------  R G B  -------------------------

      uint8_t R = EEPROM.read(address++);
      uint8_t G = EEPROM.read(address++);
      uint8_t B = EEPROM.read(address++);


      String Rstring, Gstring, Bstring;

      NewColour = RgbColor(R,G,B);

      if (R == 0) { Rstring = "00" ;} else  Rstring = String(R,HEX); 
      if (G == 0) { Gstring = "00" ;} else  Gstring = String(G,HEX);       
      if (B == 0) { Bstring = "00" ;} else  Bstring = String(B,HEX); 


      WebRGBcolour = Rstring + Gstring + Bstring; 

      WebRGBcolour.toUpperCase();

 
     
// 6--------------------------------  Vars 1 - 10   -------------------------

                    var1 = EEPROM.read(address++); 
                    var2 = EEPROM.read(address++); 
                    var3 = EEPROM.read(address++); 
                    var4 = EEPROM.read(address++); 
      IntervalMultiplier = EEPROM.read(address++); // var5
                    var6 = EEPROM.read(address++); 
                    var7 = EEPROM.read(address++); 
                    var8 = EEPROM.read(address++); 
                    var9 = EEPROM.read(address++); 
                   var10 = EEPROM.read(address++); 

   // Serial.print("Settings Loaded for : ");
    // Serial.println(location);
if (IntervalMultiplier == 0) IntervalMultiplier = 1; 


if (location != 0) {

    //send_mqtt_msg("loadpreset", String("0")); // not sure about this!

    String msg  = String(location) + " Loaded";
    send_mqtt_msg("Status", msg); 
    send_current_settings();

}


}



void Save_LED_Settings (uint8_t location) {

  if ( location < 0 || location > 10) return; 

  uint16_t address = START_address_settings + (32 * location); 

//Serial.print("Saving LED data, address = ");
//Serial.println(address);

  // START_address_settings = 160; 

// 1 --------------------------------   LAST OP STATE ------------------------
     EEPROM.write(address++, LastOpState);

// 2 --------------------------------   Timer ---------------------------------
    
     EEPROM.write(address++, WS2812interval / 256 );
     EEPROM.write(address++, WS2812interval % 256 );


// 3 --------------------------------  Animation speed -------------------------

     EEPROM.write(address++, CurrentAnimationSpeed / 256 );
     EEPROM.write(address++, CurrentAnimationSpeed % 256 );

// 4--------------------------------  Brightness -------------------------


    EEPROM.write(address++, CurrentBrightness );

// 5--------------------------------  R G B  -------------------------

      EEPROM.write(address++, NewColour.R);
      EEPROM.write(address++, NewColour.G);
      EEPROM.write(address++, NewColour.B);
     
// 6--------------------------------  Vars 1 - 10   -------------------------

     EEPROM.write(address++, var1);
     EEPROM.write(address++, var2);
     EEPROM.write(address++, var3);
     EEPROM.write(address++, var4);
     EEPROM.write(address++, IntervalMultiplier);
     EEPROM.write(address++, var6);
     EEPROM.write(address++, var7);
     EEPROM.write(address++, var8);
     EEPROM.write(address++, var9);
     EEPROM.write(address++, var10);

     EEPROM.commit();
     //Serial.print("Settings Saved for : ");
     //Serial.println(location);

     String msg  = String(location) + " Saved";

     if (location != 0)   { 
      send_mqtt_msg("Status", msg); 
      current_loaded_preset_changed = false; 
      current_loaded_preset = location; 
    }

}



void cache eeprom_write_string(uint16_t address, char* string) {

  uint16_t length = strlen(string) + 1;
  for (uint16_t i = 0; i < length; i++) {
    EEPROM.write(address + i, string[i]);
  }

}


void cache eeprom_read_string(uint16_t address, char* buffer, uint16_t length) {

  byte buf;
  uint16_t position = 0;

  while ( (buf != 0x00) && (position < length) ) {
    buf = EEPROM.read(address + position);
    buffer[position++] = buf;
  }

  if ((buf != 0x00) && (position >= 1)) {
    buffer[position - 1] = 0;
  }

}


