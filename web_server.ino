



/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Byte DUMP EEPROM to HTML.... Main Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

void cache handle_power() {


   //    File f = SPIFFS.open("/power.htm", "r");
   //    Debugf("FileSize = %u \n", f.size());
   //    server.send (200, "text", f.readString());
   //    f.close();
   //server.send(200, "text/html", "");



}


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
 
 if (server.arg("ssid").length() !=0 ) ssid_command(server.arg("ssid"));
 if (server.arg("password").length() !=0 ) password_command(server.arg("password"));
 if (server.arg("deviceid").length() !=0 ) deviceid_command(server.arg("deviceid"));
 if (server.arg("mqttserver").length() !=0 ) mqttserver_command(server.arg("mqttserver")); 
 if (server.arg("scan").length() !=0 ) scannetworks();
 if (server.arg("reboot").length() !=0 ) ESP.reset(); // abort();

  
  
  
  //bool mqttconnected = mqttclient.connected();
  

  buf = F("<!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head><meta name='viewport' content='initial-scale=1'><title>Wifi Configuration</title></head><body><h1>Wifi Config</h1>");
  buf += "<p>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
  buf += "<br>Current SSID is: " + String(ssid);
  buf += "<br>Current MQTT Server is: " + mqttserver_string + "..." + ((mqttclient.connected())?"Connected":"Disconnected");
  buf += "<br>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  buf += F("<br><form action='/wifi' method='POST'>");
  buf += F("New Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>");
  buf += F("MQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>");

  //buf += "<input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<\p>"; 
  buf += "\n\n<p>Please Select Wifi Network...</p>";

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();
    server.sendContent(buf);
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
  
  //buf += "\n\nSSID: <input type='text' id='ssid' name='ssid' value=''><br/>";
  buf += F("Password: <input type='text' name='password' value=''><br/></p>");
  //buf += "<input type='submit' value='Submit'></form>"; 
  buf += F("<input type='submit' name='reboot' value='Reboot!'/>");
  buf += F("  <input type='submit' name ='scan' value='Scan'/>");   
    // working buf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  buf += F("  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>") ;
  buf += F("  <input type='submit' value='Submit'/>") ; 
  buf += F("</form></p>"); 

  buf += htmlendstring; 


    server.sendContent(buf);

    //server.send(200, "text/html", buf);

     
    if (networkrestart) restartNetworking(); 
    if (mqttreload) mqttreloadfunc();

               
} // END of config_ handle





void ICACHE_FLASH_ATTR handle_io() {
  
    server.send(200, "text", "IO comming soon");

}


void ICACHE_FLASH_ATTR handle_test() {

  /*
String buf; 
    byte addressarray[8] = {PIN_1,PIN_2,PIN_3,PIN_4,PIN_5,PIN_6,PIN_7,PIN_8};

       if (server.arg("binary") == "test") 
           {
              Serial.println("binary TEST request recieved");
              for (int i = 0; i < 8; i++)
                   {
                         Serial.println(String(i) + " = " + String(addressarray[i], BIN));
                   }
       
           } /// end of if, binary
           
          for (int i = 0; i <8; i++)
          {
            String str=String(i); //converting integer into a string
            char b[2]; 
            str.toCharArray(b,2); 
            //Serial.print("integer loop : ");
            //Serial.println(b);
            
             if ( (server.arg(b)).length() != 0   )
             //server.arg(root_string_names_actual[i]).length() != 0)
                  {
                  
              //    Serial.println("Writing " + String(  addressarray[i], BIN) + " to " + server.arg(b));
                   // EepromUtil::eeprom_write_bytes( int startAddr, const byte* array, int numBytes ) 
              //         String value = server.arg(b); 
              //         char buf[value.length()];
              //         value.toCharArray(buf,50);
              //         int ValueInt = atoi(buf);
                       //byte ValueByte = 
                   
              //      EepromUtil::eeprom_write_bytes(ValueInt, &(addressarray[i]), 1);
              //      EEPROM.commit();

                  }
          }
         
         int memlocation = 0;
         if (server.arg("location").length() != 0) 
          {
            String memlocationString = server.arg("location");
            char buf[memlocationString.length()];
            memlocationString.toCharArray(buf,50);
            memlocation = atoi(buf); 
           Serial.print("Memlocation: "); 
             Serial.println(memlocation);      
          }
          
          
         if (server.arg("compare").length() != 0) 
         { 
         for (int i = 0; i <8; i++)
         {

          if (server.arg("compare") == String(i))
              {
                Serial.print("Compare String.." + String(i) + ":  ");
                Serial.print(addressarray[i], BIN);
                Serial.print("  with ");
                //char *readchar[1]; //] = "";
                byte test = EEPROM.read(memlocation);
                
                //EepromUtil::eeprom_read_bytes(memlocation, &test, 2);
                Serial.print(test,BIN);
                Serial.print(" ,at location... ");
                Serial.println(memlocation);
                //byte readbytes[1]= (byte)readchar[1];
                byte addressbyte = addressarray[i];
                
                if (addressbyte & test)
                {
                  Serial.println("Match Found");
                } else {
                  Serial.println("Match NOT Found");
                } 

                
                
              }
         }
         }
         
         if (server.arg("write").length() != 0) {
           
          String value = server.arg("write"); 
           char buf[value.length()];
           value.toCharArray(buf,2);
           int valueInt = value.toInt();
           Serial.print("BYTE to write: ");
           Serial.print(valueInt);
           Serial.print(" , in binary: ");
           Serial.println(valueInt,BIN);
           //byte bytetowrite = buf[0];
           EEPROM.write(memlocation,valueInt);
           EEPROM.commit();
 
         }
         
         if (server.arg("enabled") == "YES")
         {
         Serial.println("YES MQTT is enabled");
         } else if (server.arg("enabled") == "NO")
         {
          Serial.println("NO MQTT is enabled");
         }
  
  
    buf = F("<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>MQTT Configuration</title></head>\n<body><h1>MQTT Page of ESP</h1>\n");
    buf += F("<form action='/test' method='POST'> ENABLE: <input type='radio' onChange='this.form.submit();' name='enabled' value='NO' checked> NO <input type='radio' onChange='this.form.submit();' name='enabled' value='YES'> YES") ;  
    buf += F("</form>");

    buf += F("<form>");
    buf += F("<label for='flip-checkbox-1'>Flip toggle switch checkbox:</label>");
    buf += F("<input type='checkbox' data-role='flipswitch' name='flip-checkbox-1' id='flip-checkbox-1'>");
    buf += F("</form>");


  
    buf += htmlendstring; 
  
    server.send(200, "text/html", buf);
    */
}


void handle_misc ()

{
   //Serial.println();
  //Serial.print("Page hit...(");
  //Serial.print(ESP.getFreeHeap());
  //Serial.print(")...");
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
//<meta http-equiv='refresh' content='30'/>

  // 1 = String(deviceid)
  // 2 = String(ESP.getFreeHeap())
  // 3 = String(ESP.getFlashChipSize())
  // 4 = String(ESP.getFlashChipSizeByChipId())
  // 5 = String(ESP.getFlashChipId())
  // 6 = String(ESP.getChipId())
  // 7 = String(millis())
  // 8 = 
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int Vcc = analogRead(A0); 
    
  char Up_time[20]; 
  snprintf ( Up_time, 20, "%02d:%02d:%02d", hr, min % 60, sec % 60 );


  String content0 = F("\
  <!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta http-equiv='refresh' content='30'> <meta name='viewport' content='initial-scale=1'><title> % ESP Melvide</title></head><body><h1> Misc Functions</h1>\
  <p> Version = % \
  <br> Compile Time = % \
  <br> Heap Size = % \
  <br> Flash Size = % \
  <br> Flash Size by ID = % \
  <br> Flash ID = % \
  <br> Chip ID = % \
  <br> Sketch Size = % \
  <br> Free Space = % \
  <br> Millis = % \
  <br> Up Time = % \
  <br> VCC = % \
  <br> RSSI = % \
  <p><form action='/misc' method='POST'>\
  <p> Select Speed <select name='serial' onchange='this.form.submit();'>\
  ");



  buf = insertvariable (content0, String(deviceid)); 
  buf = insertvariable ( buf, version);  
  buf = insertvariable (buf, String(compile_date));
  buf = insertvariable ( buf, String(ESP.getFreeHeap()));
  buf = insertvariable ( buf, String(ESP.getFlashChipSize()));
  buf = insertvariable ( buf, String(ESP.getFlashChipSizeByChipId()));
  buf = insertvariable ( buf, String(ESP.getFlashChipId()));
  buf = insertvariable ( buf, String(ESP.getChipId()));
  buf = insertvariable ( buf, String(ESP.getSketchSize()  ));
  buf = insertvariable ( buf, String(ESP.getFreeSketchSpace()));
  buf = insertvariable ( buf, String(millis()));
  buf = insertvariable  (buf, String(Up_time));
  buf = insertvariable  (buf, String(ESP.getVcc()));
  buf = insertvariable (buf, String(WiFi.RSSI()));
  
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

  String content1 = F("\
  </select>\
  </form></p>\
  <p><a href='/bytedump'> EEPROM DUMP </a>\
  <br><a href='/misc?eeprom=bytedump'> EEPROM DUMP BYTES </a>\
  <br><a href='/misc?eeprom=wipe'> EEPROM FORMAT </a>"); 
    
  server.sendContent(content1);

  //buf = htmlendstring; 

  server.sendContent(htmlendstring); 

  //server.send(200, "text/html", buf);

  if (updateEEPROMflag) { 
  EEPROM.write(SERIALspeedbyte, currentspeed);
  EEPROM_commit_var = true;
  updateEEPROMflag = false;
          }; 
  //Serial.println("Page finished.");
  //Serial.print(ESP.getFreeHeap());
  //Serial.print(")");

}  



