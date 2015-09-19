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


// void Load_LED_Defaults_old (uint8_t location) {
// uint8_t temp,tempb; 
// // START_address_settings

// if ( location < 0 || location > 10) location = 0; 


// uint16_t address = START_address_settings + (32 * location); 


//   // START_address_settings = 160; 

// // 1 --------------------------------   LAST OP STATE ------------------------
//      temp = EEPROM.read(address++);

//     if (isnan(temp)) temp = 0;

//     if (temp != 0 ) {  LastOpState = (operatingState)temp; };  // Stops last opstate being over written by an OFF..
// // 2 --------------------------------   Timer ---------------------------------
    
//      temp = EEPROM.read(address++);
//      tempb = EEPROM.read(address++);
//         if(isnan(temp)) temp = 0;
//         if(isnan(tempb)) temp = 0;
//         WS2812interval = temp*256+tempb;
//         if (WS2812interval == 0 ) WS2812interval = 1; // Keep it to a minimum of 1
//         //if (isnan(WS2812interval)) WS2812interval = 2000;

//         //Serial.printf("Retrieved EEPROM timer = %u \n", WS2812interval); 

// // 3 --------------------------------  Animation speed -------------------------

//      temp = EEPROM.read(address++);
//      tempb = EEPROM.read(address++);

//         if(isnan(temp)) temp = 0;
//         if(isnan(tempb)) temp = 0;
//         CurrentAnimationSpeed = temp*256+tempb;
//         if (CurrentAnimationSpeed ==0 ) CurrentAnimationSpeed = 1; // keep to a minimum of 1
//     //    if (isnan(CurrentAnimationSpeed) || CurrentAnimationSpeed == 0) CurrentAnimationSpeed = 2000;


// // 4--------------------------------  Brightness -------------------------

//         CurrentBrightness = EEPROM.read(address++);
//         if (isnan(CurrentBrightness) || CurrentBrightness == 0) CurrentBrightness = 255;

// // 5--------------------------------  R G B  -------------------------

//       uint8_t R = EEPROM.read(address++);
//       uint8_t G = EEPROM.read(address++);
//       uint8_t B = EEPROM.read(address++);

//       Debugf("Colour debugging (EEPROM) R=%u, G=%u, B=%u \n", R, G, B);


//       String Rstring, Gstring, Bstring;

//       NewColour = RgbColor(R,G,B);

//       if (R == 0) { Rstring = "00" ;
//         } else if  ( R < 16 && R > 0 ) {
//            Rstring = "0" + String(R,HEX); 
//         } else {
//            Rstring = String(R,HEX); 
//         } 

//       if (G == 0) { Gstring = "00" ;
//         } else if  ( G < 16 && G > 0 ) {
//            Gstring = "0" + String(G,HEX); 
//         } else {
//            Gstring = String(G,HEX); 
//         } 

//       if (B == 0) { Bstring = "00" ;
//         } else if  ( B < 16 && B > 0 ) {
//            Bstring = "0" + String(B,HEX); 
//         } else {
//            Bstring = String(B,HEX); 
//         } 

//   //    if (G == 0) { Gstring = "00" ;} else  Gstring = String(G,HEX);       
//   //    if (B == 0) { Bstring = "00" ;} else  Bstring = String(B,HEX); 

//       Debug("Colour debugging (String) R=") ; 
//       Debug(Rstring); 
//       Debug(", G=");
//       Debug(Gstring);
//       Debug(", B=");
//       Debugln(Bstring); 

//       WebRGBcolour = Rstring + Gstring + Bstring; 

//   //    if (R < 16 && R > 0) WebRGBcolour = "0" + WebRGBcolour; 


//       WebRGBcolour.toUpperCase();

//       Debug("Final result = "); 
//       Debugln(WebRGBcolour); 

     
// // 6--------------------------------  Vars 1 - 10   -------------------------

//                     var1 = EEPROM.read(address++); 
//                     var2 = EEPROM.read(address++); 
//                     var3 = EEPROM.read(address++); 
//                     var4 = EEPROM.read(address++); 
//       IntervalMultiplier = EEPROM.read(address++); // var5
//                     var6 = EEPROM.read(address++); 
//                     var7 = EEPROM.read(address++); 
//                     var8 = EEPROM.read(address++); 
//                     var9 = EEPROM.read(address++); 
//                    var10 = EEPROM.read(address++); 

//    // Serial.print("Settings Loaded for : ");
//     // Serial.println(location);
// if (IntervalMultiplier == 0) IntervalMultiplier = 1; 


// if (location != 0) {

//     //send_mqtt_msg("loadpreset", String("0")); // not sure about this!

//   //  String msg  = String(location) + " Loaded";
//   //  send_mqtt_msg("Status", msg); 
//   //  send_current_settings();
//     // This line ensures that upon reboot if a preset was loaded, that is NOT 0, it gets reloaded
//     // if it is a loaded on that has been changed.  not needed.... 
//     EEPROM.write(LastOpState_Address, location);
    
//     EEPROM_commit_var = true; 
// }




// }



// void Save_LED_Settings_old (uint8_t location) {
    
//   if ( location < 0 || location > 10) return; 




//   uint16_t address = START_address_settings + (32 * location); 

// //Serial.print("Saving LED data, address = ");
// //Serial.println(address);

//   // START_address_settings = 160; 

// // 1 --------------------------------   LAST OP STATE ------------------------
//      EEPROM.write(address++, LastOpState);

// // 2 --------------------------------   Timer ---------------------------------
    
//      EEPROM.write(address++, WS2812interval / 256 );
//      EEPROM.write(address++, WS2812interval % 256 );


// // 3 --------------------------------  Animation speed -------------------------

//      EEPROM.write(address++, CurrentAnimationSpeed / 256 );
//      EEPROM.write(address++, CurrentAnimationSpeed % 256 );
//      //EEPROM.commit();
//      //uint8_t testb = EEPROM.read(address-1); 
//      //Debugf("Animation speed saved =%u \n",CurrentAnimationSpeed);
//      //int reread = (testa * 256) + testb;
//      //Debugf("Animation speed read back =%u \n",reread);


// // 4--------------------------------  Brightness -------------------------


//     EEPROM.write(address++, CurrentBrightness );

// // 5--------------------------------  R G B  -------------------------

//       EEPROM.write(address++, NewColour.R);
//       EEPROM.write(address++, NewColour.G);
//       EEPROM.write(address++, NewColour.B);
     
// // 6--------------------------------  Vars 1 - 10   -------------------------

//      EEPROM.write(address++, var1);
//      EEPROM.write(address++, var2);
//      EEPROM.write(address++, var3);
//      EEPROM.write(address++, var4);
//      EEPROM.write(address++, IntervalMultiplier);
//      EEPROM.write(address++, var6);
//      EEPROM.write(address++, var7);
//      EEPROM.write(address++, var8);
//      EEPROM.write(address++, var9);
//      EEPROM.write(address++, var10);

//      if (location == 0 ) EEPROM.write(LastOpState_Address, 0); // if were no longer on preset.. but defualt back to 0

//      EEPROM_commit_var = true;
//      //Serial.print("Settings Saved for : ");
//      //Serial.println(location);

//    //  String msg  = String(location) + " Saved";

//      if (location != 0)   { 
//      // send_mqtt_msg("Status", msg); 
//       current_loaded_preset_changed = false; 
//       current_loaded_preset = location; 
//     }

// }



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


void cache Save_LED_Settings (uint8_t location) {

//WS2812_Settings.LastOpState = LastOpState ; 
//WS2812_Settings.Timer = WS2812interval;
//WS2812_Settings.Animationspeed = CurrentAnimationSpeed;  // trying to get rid of this !  
//WS2812_Settings.Brightness = CurrentBrightness;
//WS2812_Settings.Color = NewColour; 


/*WS2812_Settings.var1 = var1; 
WS2812_Settings.var2 = var2;
WS2812_Settings.var3 = var3;
WS2812_Settings.var4 = var4;
WS2812_Settings.var5 = IntervalMultiplier;
WS2812_Settings.var6 = var6;
WS2812_Settings.var7 = var7;
WS2812_Settings.var8 = var8;
WS2812_Settings.var9 = var9;
WS2812_Settings.var10 = var10; */
WS2812_Settings.SavedOpState = (uint8_t)LastOpState; 
Debugf("Saved opState = %u \n", WS2812_Settings.SavedOpState); 
  
  if ( location < 0 || location > 10) return;  // check valid storage range. 





  uint16_t address = START_address_settings + (32 * location); 
  uint8_t byteswritten = EEPROM_writeAnything( address, WS2812_Settings); 

  // uint8_t chksum = 0;
  // for (uint16_t i = 0; i < byteswritten - 1 ; i++) {
  //   chksum += EEPROM.read(address+i); 
  // }

  // EEPROM.write( address + byteswritten , chksum); 
  // Debugf("Checksum = %u\n",chksum);

  ///////

     if (location == 0 ) EEPROM.write(LastOpState_Address, 0); // if were no longer on preset.. but defualt back to 0

     EEPROM_commit_var = true;
     //Serial.print("Settings Saved for : ");
     //Serial.println(location);

   //  String msg  = String(location) + " Saved";

     if (location != 0)   { 
     // send_mqtt_msg("Status", msg); 
      current_loaded_preset_changed = false; 
      current_loaded_preset = location; 
    }
//////



}


void cache Load_LED_Defaults (uint8_t location) {

// START_address_settings
if ( location < 0 || location > 10) location = 0; 
uint16_t address = START_address_settings + (32 * location); 
int bytes_read = EEPROM_readAnything(address, WS2812_Settings);

  //LastOpState = WS2812_Settings.LastOpState; 

//  if (WS2812_Settings.LastOpState != 0 ) {  LastOpState = (operatingState)WS2812_Settings.LastOpState; };  // Stops last opstate being over written by an OFF..
  if (WS2812_Settings.SavedOpState != 0 ) { LastOpState = (operatingState)WS2812_Settings.SavedOpState; };  // Stops last opstate being over written by an OFF..


  //WS2812interval = WS2812_Settings.Timer;
  CurrentAnimationSpeed = WS2812_Settings.Animationspeed; 
  //CurrentBrightness = WS2812_Settings.Brightness;
  //NewColour = WS2812_Settings.Color; 


// uint8_t chksum = 0;

// for (uint16_t i = 0; i < bytes_read - 1 ; i++) {
//     chksum += EEPROM.read(address+i); 
// }



Serial.print("Settings loaded ------- ");
Serial.print(bytes_read) ;
Serial.print(" Bytes read");
Serial.print(" ------- \n" ); 
// Serial.print(chksum);
// Serial.print(" = ");
// Serial.print(WS2812_Settings.CheckSum);
// Serial.print( " ==> ");
// if (chksum == WS2812_Settings.CheckSum) { Serial.println("OK"); } else { Serial.println("CRC Failed"); };

  Debug("Op State        ==> ");
Debugln(WS2812_Settings.SavedOpState);
  Debug("Timer           ==> ");
Debugln(WS2812_Settings.Timer);
  Debug("Animation speed ==> ");
Debugln(WS2812_Settings.Animationspeed);
  Debug("Brightness      ==> ");
Debugln(WS2812_Settings.Brightness);
  Debug("Colour          ==> ");
Debug(WS2812_Settings.Color.R);
Debug("  "); 
Debug(WS2812_Settings.Color.G);
Debug("  "); 
Debugln(WS2812_Settings.Color.B);
  Debug("Palette         ==> ");
Debugln(WS2812_Settings.Palette_Choice);
  Debug("Palette Range   ==> ");
Debugln(WS2812_Settings.Palette_Range);
  Debug("Number Colours  ==> ");  
Debugln(WS2812_Settings.Palette_Number);
  Debug("Random          ==> ");  
Debugln(WS2812_Settings.Random);
  Debug("Time Stretch    ==> ");
Debugln(WS2812_Settings.Time_Stretch);
  Debug("Total X         ==> ");
Debugln(WS2812_Settings.Total_X);
  Debug("Effect Count    ==> ");
Debugln(WS2812_Settings.Effect_Count);
  Debug("Effect Min Size ==> ");
Debugln(WS2812_Settings.Effect_Min_Size);
  Debug("Effect Max Size ==> ");
Debugln(WS2812_Settings.Effect_Max_Size);
  Debug("Effect Option   ==> ");
Debugln(WS2812_Settings.Effect_Option);
//Serial.println(WS2812_Settings.var8);
//Serial.println(WS2812_Settings.var9);
//Serial.println(WS2812_Settings.var10);
Debugln("End-------------------------"); 

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

  //    if (G == 0) { Gstring = "00" ;} else  Gstring = String(G,HEX);       
  //    if (B == 0) { Bstring = "00" ;} else  Bstring = String(B,HEX); 

      Debug("Colour debugging (String) R=") ; 
      Debug(Rstring); 
      Debug(", G=");
      Debug(Gstring);
      Debug(", B=");
      Debug(Bstring); 
      Debug(" ==> "); 
      WebRGBcolour = Rstring + Gstring + Bstring; 

  //    if (R < 16 && R > 0) WebRGBcolour = "0" + WebRGBcolour; 

      WebRGBcolour.toUpperCase();

      //Debug("Final result = "); 
      Debugln(WebRGBcolour); 
//////

      if (IntervalMultiplier == 0) IntervalMultiplier = 1; 


if (location != 0) {

    //send_mqtt_msg("loadpreset", String("0")); // not sure about this!

  //  String msg  = String(location) + " Loaded";
  //  send_mqtt_msg("Status", msg); 
  //  send_current_settings();
    // This line ensures that upon reboot if a preset was loaded, that is NOT 0, it gets reloaded
    // if it is a loaded on that has been changed.  not needed.... 
    EEPROM.write(LastOpState_Address, location);
    
    EEPROM_commit_var = true; 

    //////


}


}


