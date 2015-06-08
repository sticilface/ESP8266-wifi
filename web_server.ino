



/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Byte DUMP EEPROM to HTML.... Main Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

void ICACHE_FLASH_ATTR handle_bytedump() {
  // your 32s are : 31,63,95,127,159,191,223,255,287,319,351,383,415,447,479,511
  Serial.println("Begining html dump of EEPROM.");

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

   server.send(200, "text/html", bytedump);

   //bytedump = " ";

  bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 128; i < 256; i++) // was 128 -- > 256
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


    server.send(200, "", bytedump);
    //server.write(*bytedump, sizeof bytedump); 

    //bytedump = " ";

 bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 256; i < 384; i++) // was 128 -- > 256
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

   // bytedump += "</table>";

    server.send(200, "", bytedump);

 bytedump = "<td></tr><tr>";
   j = 0;
   for (int i = 384; i < 512; i++) // was 128 -- > 256
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

   bytedump += "</table>";

    server.send(200, "", bytedump);




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

 
 if (server.arg("ssid").length() != 0) ssid_command(server.arg("ssid"));
 if (server.arg("password").length() != 0) password_command(server.arg("password"));
 if (server.arg("deviceid").length() != 0) deviceid_command(server.arg("deviceid"));
 if (server.arg("mqttserver").length() != 0) mqttserver_command(server.arg("mqttserver")); 
 if (server.arg("scan").length() != 0) scannetworks();
 if (server.arg("reboot").length() != 0) ESP.reset(); // abort();

  
  
  
  int mqttconnected = mqttclient.connected();
  

  httpbuf = F("<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>Wifi Configuration</title></head>\n<body><h1>Wifi Config</h1>\n");
  httpbuf += "<p>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
  httpbuf += "<br>Current SSID is: " + String(ssid);
  httpbuf += "<br>Current MQTT Server is: " + mqttserver_string + "..." + ((mqttconnected)?"Connected":"Disconnected");
  httpbuf += "<br>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  httpbuf += F("<br><form action='/wifi' method='POST'>\n");
  httpbuf += F("\n\nNew Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>");
  httpbuf += F("\n\nMQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>");

  //httpbuf += "<input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<\p>"; 
  httpbuf += "\n\n<p>Please Select Wifi Network...</p>";
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
  httpbuf += "<input type='radio' name='ssid' value='" + String(WiFi.SSID(i)) + "'" + checked + ">" + String(WiFi.SSID(i)) + "(" + String(WiFi.RSSI(i)) + ") <br/>\n";
  }
  
  //httpbuf += "\n\nSSID: <input type='text' id='ssid' name='ssid' value=''><br/>";
  httpbuf += F("\nPassword: <input type='text' name='password' value=''><br/></p>");
  //httpbuf += "<input type='submit' value='Submit'></form>"; 
  httpbuf += F("<input type='submit' name='reboot' value='Reboot!'/>\n");
  httpbuf += F("  <input type='submit' name ='scan' value='Scan'/>");   
    // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  httpbuf += F("  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n") ;
  httpbuf += F("  <input type='submit' value='Submit'/>") ; 
  httpbuf += F("</form></p>"); 

  httpbuf += htmlendstring; 



    server.send(200, "text/html", httpbuf);

     
    if (networkrestart) restartNetworking(); 

    if (mqttreload) mqttreloadfunc();

               
} // END of config_ handle





void ICACHE_FLASH_ATTR handle_io() {
  
    server.send(200, "text", "IO comming soon");

}


void ICACHE_FLASH_ATTR handle_test() {

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
                  
                  Serial.println("Writing " + String(  addressarray[i], BIN) + " to " + server.arg(b));
                   // EepromUtil::eeprom_write_bytes( int startAddr, const byte* array, int numBytes ) 
                       String value = server.arg(b); 
                       char buf[value.length()];
                       value.toCharArray(buf,50);
                       int ValueInt = atoi(buf);
                       //byte ValueByte = 
                   
                    EepromUtil::eeprom_write_bytes(ValueInt, &(addressarray[i]), 1);
                    EEPROM.commit();

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
  
  
    httpbuf = F("<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>MQTT Configuration</title></head>\n<body><h1>MQTT Page of ESP</h1>\n");
    httpbuf += F("<form action='/test' method='POST'> ENABLE: <input type='radio' onChange='this.form.submit();' name='enabled' value='NO' checked> NO <input type='radio' onChange='this.form.submit();' name='enabled' value='YES'> YES") ;  
    httpbuf += F("</form>");

    httpbuf += F("<form>");
    httpbuf += F("<label for='flip-checkbox-1'>Flip toggle switch checkbox:</label>");
    httpbuf += F("<input type='checkbox' data-role='flipswitch' name='flip-checkbox-1' id='flip-checkbox-1'>");
    httpbuf += F("</form>");


  
    httpbuf += htmlendstring; 
  
    server.send(200, "text/html", httpbuf);
    
}


void ICACHE_FLASH_ATTR handle_misc ()

{
          //  EEPROM WIPE 
         if (server.arg("eeprom") == "wipe") EEPROM_wipe();
          // UPDATE SERIAL SPEED...  requires reboot to work...
         if (server.arg("serial").length() != 0) {
            String a = server.arg("serial");
            Serial.println("Serial mode recieved: " + a);
            int selected = a.toInt();
            EEPROM.write(SERIALspeedbyte, selected);
            EEPROM.commit();
            Serial.println("Serial speed updated to: " + String(baudrates[selected]));
         }
         
  int currentspeed = EEPROM.read(SERIALspeedbyte); //SERIALspeedbyte
  //currentspeed = 4;
  String selected = " "; 

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Misc Functions</h1>\n";
  httpbuf += "<p> Heap Size = " + String(ESP.getFreeHeap()) ; // + "</br>";
  httpbuf += "<br> Flash Size = " + String(ESP.getFlashChipSize()) ;
  httpbuf += "<br> Flash Size by ID = " + String(ESP.getFlashChipSizeByChipId()) ;

  httpbuf += "<br> Flash ID = " + String(ESP.getFlashChipId()) ;
  httpbuf += "<br> Chip ID = " + String(ESP.getChipId()) + "</p>";
  httpbuf += "<p><form action='/misc' method='POST'>\n";
  httpbuf += "<p> Select Speed <select name='serial' onchange='this.form.submit();'>";
  for (int i=0; i < numberofbaudrates; i ++ ) {
    if (currentspeed == i) { 
        selected = "' selected "; 
      } else selected = "' "; 
    httpbuf += "<option value='" + String(i) + selected + ">" + String(baudrates[i]) + "</option>";
  }
  httpbuf += "</select>";
  httpbuf += "</form></p>";

  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/misc?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/misc?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);


}
