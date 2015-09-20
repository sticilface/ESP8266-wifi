



/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Byte DUMP EEPROM to HTML.... Main Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */


void ICACHE_FLASH_ATTR handle_bytedump() {
  String bytedump; 
  // your 32s are : 31,63,95,127,159,191,223,255,287,319,351,383,415,447,479,511
  Serial.println("Begining html dump of EEPROM.");

server.setContentLength(CONTENT_LENGTH_UNKNOWN);


  server.send(200, "text/html", "");

  WiFiClient client = server.client();


   int j = 0;
   bytedump = "<p><table style='width:50%'><tr>";
   for (int i = 0; i < 128; i++) // was 128
   {
   char value = EEPROM.read(i);
   bytedump += "<td>" + String(value) + "</td>";
   if (j == 31) 
   { 
     bytedump += "<td>" + String(i) + "<td></tr><tr>";
     j = -1;
   }
     j += 1; 
   }
  server.sendContent(bytedump);


   //bytedump = " ";

  bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 128; i < 256; i++) // was 128 -- > 256
   {
   //char value = ;
   char value[4]; 
   snprintf ( value, 4, "%03d", EEPROM.read(i) );

   bytedump += "<td>" + String(value) + "</td>";
   if (j == 31) 
   { 
     bytedump += "<td>" + String(i) + "<td></tr><tr>";
     j = -1;
   }
     j += 1; 
   }


  server.sendContent(bytedump);
    //server.write(*bytedump, sizeof bytedump); 

    //bytedump = " ";

 bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 256; i < 384; i++) // was 128 -- > 256
   {
   char value[4]; 
   snprintf ( value, 4, "%03d", EEPROM.read(i) );
   bytedump += "<td>" + String(value) + "</td>";
   if (j == 31) 
   { 
     bytedump += "<td>" + String(i) + "<td></tr><tr>";
     j = -1;
   }
     j += 1; 
   }

   // bytedump += "</table>";

  server.sendContent(bytedump);

 bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 384; i < 512; i++) // was 128 -- > 256
   {
   char value[4]; 
   snprintf ( value, 4, "%03d", EEPROM.read(i) );
   bytedump += "<td>" + String(value) + "</td>";
   if (j == 31) 
   { 
     bytedump += "<td>" + String(i) + "<td></tr><tr>";
     j = -1;
   }
     j += 1; 
   }

   bytedump += "</table>";

  server.sendContent(bytedump);




/*
String bytedump4 = "<td></tr><tr>";
   j = 0;
   for (int i = 384; i < 512; i++) // was 128 -- > 256
   {
   char value = EEPROM.read(i);
   bytedump4 += "<td>" + String(value) + "</td>";
   if (j == 31) 
   { 
     bytedump4 += "<td>" + String(i) + "<td></tr><tr>";
     j = -1;
   }
     j += 1; 
   } */

   //bytedump2 += "</table>";

     //server.send(200, "text/html", bytedump + bytedump2);

  
  }


/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Wifi Config Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */


void ICACHE_FLASH_ATTR handle_wifi() {

  uint8_t wifinetworksfound = WiFi.scanNetworks();
 
 if (server.arg("ssid").length() !=0 ) ssid_command(server.arg("ssid"));
 if (server.arg("password").length() !=0 ) password_command(server.arg("password"));
 if (server.arg("deviceid").length() !=0 ) deviceid_command(server.arg("deviceid"));
 if (server.arg("mqttserver").length() !=0 ) mqttserver_command(server.arg("mqttserver")); 
 //if (server.arg("scan").length() !=0 ) scannetworks();
 if (server.arg("reboot").length() !=0 ) ESP.reset(); // abort();


   buf = insertvariable ( FPSTR(webpage_handlewifi_1) , IPtoString(WiFi.localIP()) ); 
   buf = insertvariable ( buf, IPtoString(WiFi.localIP() ) ); 
   buf = insertvariable ( buf, String(ssid) ); 
   buf = insertvariable ( buf, IPtoString(MQTTserver) ); 
   if (mqttclient.connected()) buf = insertvariable ( buf, "Connected" ) ; else buf = insertvariable ( buf, "Disconnected" ) ; 
   buf = insertvariable ( buf,   String(deviceid) ); 
   buf = insertvariable ( buf,   String(deviceid) ); 



  server.setContentLength(CONTENT_LENGTH_UNKNOWN);

    server.send(200, "text/html", "");
    WiFiClient client = server.client();
    //server.sendContent(buf);
    server.client().print(buf); 

        buf = " ";


  String checked;
  for (int i = 0; i < wifinetworksfound; ++i)
  {
    if(WiFi.status() == WL_CONNECTED && strcmp(ssid, WiFi.SSID(i)) == 0)
    {
     checked = " checked";
    } else
    {
     checked = " ";
    }
  buf += "<input type='radio' name='ssid' value='" + String(WiFi.SSID(i)) + "'" + checked + ">" + String(WiFi.SSID(i)) + "(" + String(WiFi.RSSI(i)) + ") <br/>";
  }

  buf += FPSTR(webpage_handlewifi_2); 

  buf += FPSTR(htmlendstring); 


    //server.sendContent(buf);
    server.client().print(buf); 

    //server.send(200, "text/html", buf);

     
    if (networkrestart) restartNetworking(); 

               
} // END of config_ handle





void ICACHE_FLASH_ATTR handle_io() {
  
    server.send(200, "text", "IO comming soon");

}


void ICACHE_FLASH_ATTR handle_test() {


}


void handle_misc ()

{

  bool updateEEPROMflag = false; 
  String newhttpbuf; 

          //  EEPROM WIPE 
  if (server.arg("eeprom") == "wipe") { EEPROM_wipe(); } ; 

          // UPDATE SERIAL SPEED...  requires reboot to work...
  if (server.hasArg("serial")) {
            Serial.println("NEW SERIAL RECIEVED..");
            Serial.end();
            delay(10); 
            currentspeed = (server.arg("serial")).toInt() + 1; 
            serialspeed = baudrates[currentspeed - 1] ; 
            Serial.begin(serialspeed); // 921600 460800 115200
            delay(10);
            Serial.print("Serial mode recieved: ");
            Serial.println(currentspeed);
            updateEEPROMflag = true; 
        } ; 

  String selectedhere;  
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int Vcc = analogRead(A0); 
    
  char Up_time[20]; 
  snprintf ( Up_time, 20, "%02d:%02d:%02d", hr, min % 60, sec % 60 );

  buf = insertvariable (FPSTR(webpage_misc_1), String(deviceid)); 
  buf = insertvariable ( buf, version);  
  buf = insertvariable ( buf, String(compile_date));
  buf = insertvariable ( buf, String(ESP.getSdkVersion()));
  buf = insertvariable ( buf, String(ESP.getFreeHeap()));
  buf = insertvariable ( buf, String(ESP.getFlashChipSize()));
  buf = insertvariable ( buf, String(ESP.getFlashChipSizeByChipId()));
  buf = insertvariable ( buf, String(ESP.getFlashChipId()));
  buf = insertvariable ( buf, String(ESP.getChipId()));
  buf = insertvariable ( buf, String(ESP.getSketchSize()  ));
  buf = insertvariable ( buf, String(ESP.getFreeSketchSpace()));
  buf = insertvariable ( buf, String(millis()));
  buf = insertvariable ( buf, String(Up_time));
  buf = insertvariable ( buf, String(ESP.getVcc()));
  buf = insertvariable ( buf, String(WiFi.RSSI()));
  buf = insertvariable ( buf, String(ESP.getCpuFreqMHz())); 
  //server.sendHeader("Content-Length", "1000");
  //server.setContentLength(buf.length());

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);

  server.send(200, "text/html", "");
  WiFiClient client = server.client();

  server.sendContent(buf);
  buf = " "; 
  
  for (uint8_t i = 0; i < numberofbaudrates; i ++ ) {
    if ((currentspeed - 1) == i) { 
        selectedhere = "' selected "; 
      } else { selectedhere = "' "; }

    buf += "<option value='" + String(i) + selectedhere + ">" + String(baudrates[i]) + "</option>";
  }

  server.sendContent(buf);
  server.sendContent(FPSTR(webpage_misc_2));

  server.sendContent(FPSTR(htmlendstring)); 

  //server.send(200, "text/html", buf);

  if (updateEEPROMflag) { 
  EEPROM.write(SERIALspeedbyte, currentspeed);
  EEPROM_commit_var = true;
  updateEEPROMflag = false;
          }; 


}  



