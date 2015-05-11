       
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Root Main Page
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
void handle_temperature() {


  //Serial.print("TEST: Number of items in array: "+ String(sizeof(*config_string_names_actual)));
  //Serial.println("Web Server Page entered!");
/*  for (int i=0;i < number_of_strings; i++)
  {
    //Serial.println("Processing...." +   String((root_string_names_actual[i])) );

    if (server.arg(root_string_names_actual[i]).length() != 0)
       {
        
        Command_Handler(root_string_names_actual[i], server.arg(root_string_names_actual[i]));
       }  
  } */
 if (server.arg("state").length() != 0) melvide_state_command(server.arg("state"));
 
  if (server.arg("setpoint").length() != 0) double_pct_command (server.arg("setpoint"), Setpoint, SpAddress, SpAddressbyte, "Setpoint"); 
 if (server.arg("kp").length() != 0) double_pct_command (server.arg("kp"), Kp, KpAddress, KpAddressbyte, "Kp"); 
 if (server.arg("ki").length() != 0) double_pct_command (server.arg("ki"), Ki, KiAddress, KiAddressbyte, "Ki"); 
 if (server.arg("kd").length() != 0) double_pct_command (server.arg("kd"), Kd, KdAddress, KdAddressbyte, "Kd"); 
 if (server.arg("window").length() != 0) int_pct_command (server.arg("window"), WindowSize , WindowSizeAddress, WindowSizeAddressbyte, "WindowSize"); 


  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Temperature </h1>\n";
  //httpbuf += "<p>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  //httpbuf += "<br>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
 
  if(isrunning) 
    {
      Status = "on";
    } else {   
      Status = "off";  
    }
  httpbuf += "<form action='/temperature' method='POST'>     System is:  <font size='5' color='red'>" + Status + "</font>    <input type='submit' name='state' value='ON'>    <input type='submit' name='state' value='OFF'></form>"; 
  httpbuf += "<p>Current Temperature is: " + String(Input);
  //httpbuf += "<br>Current Humidity is: " + String(Humidity);
  httpbuf += "<br>Current Set Temperature: " + String(Setpoint) ; // + ". Status is " + mqttconnected ;
  httpbuf += "<br>Current Output: " + String(pct) ; // + ". Status is " + mqttconnected ;
  httpbuf += "<br><form action='/temperature' method='POST'>\n";
  httpbuf += "\n\nTemp: <input type='text' id='setpoint' name='setpoint' value='" + String(Setpoint) + "'><br>";
  httpbuf += "\n\nKp: <input type='text' id='kp' name='kp' value='" + String(Kp) + "'>  (850)<br>";
  httpbuf += "\n\nKi: <input type='text' id='kp' name='ki' value='" + String(Ki) + "'>  (0.5)<br>";
  httpbuf += "\n\nKd: <input type='text' id='kp' name='kd' value='" + String(Kd) + "'>  (0.1)<br>";  
  httpbuf += "\n\nWindow: <input type='text' id='window' name='window' value='" + String(WindowSize) + "'>  (10000)<br>";    
  // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  httpbuf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  httpbuf += "  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 

  //httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  //httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);
  


               
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Byte DUMP EEPROM to HTML.... Main Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */
          
  








void handle_root() {


  //Serial.print("TEST: Number of items in array: "+ String(sizeof(*config_string_names_actual)));
  //Serial.println("Web Server Page entered!");
/*  for (int i=0;i < number_of_strings; i++)
  {
    //Serial.println("Processing...." +   String((root_string_names_actual[i])) );

    if (server.arg(root_string_names_actual[i]).length() != 0)
       {
        
        Command_Handler(root_string_names_actual[i], server.arg(root_string_names_actual[i]));
       }  
  } */
 if (server.arg("state").length() != 0) melvide_state_command(server.arg("state"));
 if (server.arg("humidity").length() != 0) humidity_state_command(server.arg("humidity"));
 if (server.arg("lights").length() != 0) plugin_lights_command(server.arg("lights"));
 if (server.arg("debug").length() != 0) debug_command(server.arg("debug"));

 //if (server.arg("setpoint").length() != 0) melvide_setpoint_command(server.arg("setpoint")); 
 //if (server.arg("kp").length() != 0) melvide_kp_command(server.arg("kp"));
 //if (server.arg("ki").length() != 0) melvide_ki_command(server.arg("ki"));
 //if (server.arg("kd").length() != 0) melvide_kd_command(server.arg("kd"));
 //if (server.arg("window").length() != 0) melvide_window_command(server.arg("window"));
          
          
          
  /* for (int i=0;i < numberofcommands; ++i)
          {
              if (server.arg(commands[i]).length() != 0) Command_Handler(commands[i], server.arg(commands[i]));
          } */
  
  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + "</title></head>\n<body><h1>" + version + "</h1>\n";
  //httpbuf += "<p>Current device name is: " + String(deviceid) + " (<a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>)";
  //httpbuf += "<br>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
 String status_color;
 String humidity_status_color;
 String temp_color;
 String humidity_color;
 String humidity_Status;
 String temp_actual;
 String humidity_actual;
 String light_status_string;

 if (light_status)
  {
    light_status_string = "green'> ON";
  } else 
  {
    
  light_status_string = "red'> OFF";

  }

  if(isrunning) 
    {
      Status = "ON";
      status_color = "green";
      //temp_actual = String(Input);
      //humidity_actual = String(Humidity);

      if ((Input < Setpoint - 2) || (Input > Setpoint + 2))
      {
        temp_color = "red";
      } else
      { temp_color = "green";}
      
      /*if ((Input <= Setpoint - 2) || (Input >= Setpoint + 2))
      {
        temp_color = "red";
      } else
      { temp_color = "green";} */ 

    } else {   
      Status = "OFF";  
      status_color = "red";

      //temp_actual = String(Input);
      //humidity_actual = String(Humidity);

      //temp_actual = " ";
      //humidity_actual = " ";
    }


    ////// - humidity control

      if(isrunning_Humidity) 
    {
      humidity_Status = "ON";
      humidity_status_color = "green";
      //humidity_actual = String(Input);
      //humidity_actual = String(Humidity);

      if ((Humidity < Setpoint_Humidity - 2) || (Humidity > Setpoint_Humidity + 2))
      {
        humidity_color = "red";
      } else
      { humidity_color = "green";}
      
      /*if ((Input <= Setpoint - 2) || (Input >= Setpoint + 2))
      {
        temp_color = "red";
      } else
      { temp_color = "green";} */ 

    } else {   
      humidity_Status = "OFF";  
      humidity_status_color = "red";
      humidity_color = "black";
      // temp_actual = String(Input);
      //humidity_actual = String(Humidity);

      //temp_actual = " ";
      //humidity_actual = " ";
    }


  httpbuf += "<form action='/' method='POST' style='display: inline;'>     Temperature Control is:  <font size='5' color='"+status_color+"'>" + Status + "</font>    <input type='submit' name='state' value='ON'>    <input type='submit' name='state' value='OFF'></form>"; 
  httpbuf += "<br>Set Temperature: <font size='5'>" + String(Setpoint) +"</font>" ; // + ". Status is " + mqttconnected ;
  httpbuf += "<br>Temperature is: <font size='5' color=' "+ temp_color + "'>" + String(Input) + "</font>";
  
  httpbuf += "<p><form action='/' method='POST' style='display: inline;'>     Humidity Control is:  <font size='5' color='" + humidity_status_color + "'>" + humidity_Status + "</font>    <input type='submit' name='humidity' value='ON'>    <input type='submit' name='humidity' value='OFF'></form>"; 
  httpbuf += "<br>Set Humidity: <font size='5'>" + String(Setpoint_Humidity) +"</font>" ; // + ". Status is " + mqttconnected ;

  httpbuf += "<br>Humidity is: <font size='5' color='"+ humidity_color + "'>" + String(Humidity) + "</font></br>";

  //httpbuf += "<br> Output: " + String(pct) ; // + ". Status is " + mqttconnected ;
 
  //httpbuf += "<form action='/' method='POST'>     Humidity Control is:  <font size='5' color='" + humidity_status_string + " </font>    <input type='submit' name='humidity' value='ON'>    <input type='submit' name='humidity' value='OFF'></form>";   
  httpbuf += "<br><form action='/' method='POST'>     Lights are :  <font size='5' color='" + light_status_string + " </font>    <input type='submit' name='lights' value='ON'>    <input type='submit' name='lights' value='OFF'></form>";   
  
  //httpbuf += "<br><form action='/' method='POST'>\n";
  //httpbuf += "\n\nTemp: <input type='text' id='setpoint' name='setpoint' value='" + String(Setpoint) + "'><br>";
  //httpbuf += "\n\nKp: <input type='text' id='kp' name='kp' value='" + String(Kp) + "'>  (850)<br>";
  //httpbuf += "\n\nKi: <input type='text' id='kp' name='ki' value='" + String(Ki) + "'>  (0.5)<br>";
  //httpbuf += "\n\nKd: <input type='text' id='kp' name='kd' value='" + String(Kd) + "'>  (0.1)<br>";  
  //httpbuf += "\n\nWindow: <input type='text' id='window' name='window' value='" + String(WindowSize) + "'>  (10000)<br>";    
  // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  //httpbuf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  //httpbuf += "  <input type='submit' value='Submit'/>" ; 
  //httpbuf += "</form></p>"; 
  //httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  //httpbuf += "<p> Up Time = " + String(millis()/60000) + " Minutes</p>";
   // httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size());
  //httpbuf += "<br> Up Time = " + String(millis()/60000) + " Minutes";
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);
  


               
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Byte DUMP EEPROM to HTML.... Main Page 
------------------------------------------------------------------------------------------------------------------------------------------------------------------ */

void handle_bytedump() {
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


void handle_wifi() {

 
 if (server.arg("ssid").length() != 0) ssid_command(server.arg("ssid"));
 if (server.arg("password").length() != 0) password_command(server.arg("password"));
 if (server.arg("deviceid").length() != 0) deviceid_command(server.arg("deviceid"));
 if (server.arg("mqttserver").length() != 0) mqttserver_command(server.arg("mqttserver")); 
 if (server.arg("scan").length() != 0) scannetworks();
 if (server.arg("reboot").length() != 0) system_restart(); // abort();

  
  
  
  int mqttconnected = mqttclient.connected();
  

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>Wifi Configuration</title></head>\n<body><h1>Wifi Config</h1>\n";
  httpbuf += "<p>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
  httpbuf += "<br>Current SSID is: " + String(ssid);
  httpbuf += "<br>Current MQTT Server is: " + String(mqttserver) + "..." + ((mqttconnected)?"Connected":"Disconnected");
  httpbuf += "<br>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  httpbuf += "<br><form action='/wifi' method='POST'>\n";
  httpbuf += "\n\nNew Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>";
  httpbuf += "\n\nMQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>";

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
  httpbuf += "\nPassword: <input type='text' name='password' value=''><br/></p>";
  //httpbuf += "<input type='submit' value='Submit'></form>"; 
  httpbuf += "<input type='submit' name='reboot' value='Reboot!'/>\n";
  httpbuf += "  <input type='submit' name ='scan' value='Scan'/>";   
    // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  httpbuf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  httpbuf += "  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 

  httpbuf += htmlendstring; 



    server.send(200, "text/html", httpbuf);

     
    if (networkrestart) restartNetworking(); 

    if (mqttreload) mqttreloadfunc();

               
} // END of config_ handle





void handle_io() {
  
    server.send(200, "text", "IO comming soon");

}


void handle_test() {
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
         
         int memlocation;
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
  
  
    httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>MQTT Configuration</title></head>\n<body><h1>MQTT Page of ESP</h1>\n";
    httpbuf += "<form action='/test' method='POST'> ENABLE: <input type='radio' onChange='this.form.submit();' name='enabled' value='NO' checked> NO <input type='radio' onChange='this.form.submit();' name='enabled' value='YES'> YES" ;  
    httpbuf += "</form>";

    httpbuf += "<form>";
    httpbuf += "<label for='flip-checkbox-1'>Flip toggle switch checkbox:</label>";
    httpbuf += "<input type='checkbox' data-role='flipswitch' name='flip-checkbox-1' id='flip-checkbox-1'>";
    httpbuf += "</form>";


  
    httpbuf += htmlendstring; 
  
    server.send(200, "text/html", httpbuf);
    
}




void handle_humidity() {



 if (server.arg("state").length() != 0) humidity_state_command(server.arg("state"));
 if (server.arg("setpoint").length() != 0) double_pct_command (server.arg("setpoint"), Setpoint_Humidity, Humidity_SpAddress, Humidity_SpAddressbyte, "Setpoint_Humidity"); 
 if (server.arg("kp").length() != 0) double_pct_command (server.arg("kp"), Kp_Humidity, Humidity_KpAddress, Humidity_KpAddressbyte, "Kp_Humidity"); 
 if (server.arg("ki").length() != 0) double_pct_command (server.arg("ki"), Ki_Humidity, Humidity_KiAddress, Humidity_KiAddressbyte, "Ki_Humidity"); 
 if (server.arg("kd").length() != 0) double_pct_command (server.arg("kd"), Kd_Humidity, Humidity_KdAddress, Humidity_KdAddressbyte, "Kd_Humidity"); 
 if (server.arg("window").length() != 0) int_pct_command (server.arg("window"), WindowSize_Humidity , Humidity_WindowSizeAddress, Humidity_WindowSizeAddressbyte, "WindowSize_Humidity"); 

  
  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Humidity</h1>\n";
  //httpbuf += "<p>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  //httpbuf += "<br>Current IP address is: <a href='http://" + LocalIP + "'>" + LocalIP + "</a>"; // <a href="http://www.w3schools.com">Visit W3Schools.com!</a>
 
  if(isrunning_Humidity) 
    {
      Status = "on";
    } else {   
      Status = "off";  
    }
  httpbuf += "<form action='/humidity' method='POST'>     Humidity is:  <font size='5' color='red'>" + Status + "</font>    <input type='submit' name='state' value='ON'>    <input type='submit' name='state' value='OFF'></form>"; 
  //httpbuf += "<p>Current Temperature is: " + String(Humidity);
  httpbuf += "<br>Current Humidity is: " + String(Humidity);
  httpbuf += "<br>Current Set Humidity: " + String(Setpoint_Humidity) ; // + ". Status is " + mqttconnected ;
  httpbuf += "<br>Current Output: " + String(pct_Humidity) + "</br>"; // + ". Status is " + mqttconnected ;
  httpbuf += "<br><form action='/humidity' method='POST'>\n";
  httpbuf += "\n\nHumidity: <input type='text' id='setpoint' name='setpoint' value='" + String(Setpoint_Humidity) + "'><br>";
  httpbuf += "\n\nKp: <input type='text' id='kp' name='kp' value='" + String(Kp_Humidity) + "'>  (850)<br>";
  httpbuf += "\n\nKi: <input type='text' id='kp' name='ki' value='" + String(Ki_Humidity) + "'>  (0.5)<br>";
  httpbuf += "\n\nKd: <input type='text' id='kp' name='kd' value='" + String(Kd_Humidity) + "'>  (0.1)<br>";  
  httpbuf += "\n\nWindow: <input type='text' id='window' name='window' value='" + String(WindowSize_Humidity) + "'>  (10000)<br>";    
  // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  httpbuf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  httpbuf += "  <input type='submit' value='Submit'/>" ; 
  httpbuf += "  </form></p>"; 

  //httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  //httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  //httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);


}


void handle_misc ()

{

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Misc Functions</h1>\n";

  httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);


}
