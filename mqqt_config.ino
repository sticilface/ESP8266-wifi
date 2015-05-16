




//void callback(char* mqtttopic, byte* payload, unsigned int length) {

void callback(const MQTT::Publish& pub) {

  Serial.print("MQTT Message Recieved: ");
  Serial.print(pub.topic());
  Serial.print(" => ");
  Serial.println(pub.payload_string());


  //  mqttbufcharclear (); // Clear the MQTT buffer..
/*
    char messagerecieved[length];

      for (int i = 0; i < length; i++)
        {
            messagerecieved[i] = payload[i];
        }

    String topicrecieved = String(mqtttopic);

    
      for (int i = 0; i < length; i++)
        {
            mqttcharbuf[i] = payload[i];
        }
        
        mqttbuf = String(mqttcharbuf);  
        Serial.println(mqttbuf);
    
        const char identify[] = "identify";
*/
    String topicrecieved = pub.topic();
            mqttbuf = pub.payload_string();


/*
   if (topicrecieved.indexOf("/WS2812period") > 0) WS2812timer_command_string(mqttbuf);
   if (topicrecieved.indexOf("/WS2812mode") > 0) WS2812_mode_string(mqttbuf);
   if (topicrecieved.indexOf("/WS2812dim") > 0) WS2812_dim_string(mqttbuf);
   if (topicrecieved.indexOf("/WS2812") > 0) WS2812_command_string(mqttbuf);
  */


   
 // -- 1) Identify ESP commands
 
 if (mqttbuf == "identify") 
      {
                Serial.print("Identify Request Recieved: ");
                String temp = "esp/" + String(deviceid);
                temp.toCharArray(mqttcharbuf,200);
                char iparray[20];
                LocalIP.toCharArray(iparray,20) ; // WiFi.localIP();
                if(mqttclient.publish(mqttcharbuf, iparray)) Serial.println("Sent"); 
      } 
      
if (mqttbuf == "reboot") ESP.reset(); //system_restart(); // abort();


if (mqttbuf.indexOf('=') > 0) 
      {
       String instruction = mqttbuf.substring(0,mqttbuf.indexOf('=') );
       String value = mqttbuf.substring(mqttbuf.indexOf('=') +1 ,mqttbuf.length());
       
       // Command_Handler (instruction,value);
         if (instruction == "ssid") ssid_command(value);
         if (instruction == "password") password_command(value);
         if (instruction == "deviceid") deviceid_command(value);
         if (instruction == "mqttserver") mqttserver_command(value);
         if (instruction == "serial") Serial_Command(value);

         if (instruction == "debug") debug_command(value);       
        
       //action_handler ();
        if (networkrestart) restartNetworking(); 
        if (mqttreload) mqttreloadfunc();
       
       
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// -----   EPROM UPDATE CALL ------- ////////////////////////////////////////////////////////////////////////
  /* if  (eeprom_updates_pending)
               {
                 SaveParams();
                 eeprom_updates_pending = false;
                 //Current_Request_no = 0;
                 Serial.println("(MQTT) Updates made, and saved to EEPROM");
               }  else  // else if (Current_Request_no == totalargcount) 
               {
                 Serial.println("(MQTT) End Request Reached but NO updates Pending");
                 //eeprom_updates_pending = false;
                 //Current_Request_no = 0;                
               }  */
      } 
      



} // --- end of mqtt callback



void initiatemqqt ()
    {
      //IPAddress MQTTserver(0,0,0,0);

      //PubSubClient mqttclient(MQTTserver);
        mqttclient.set_server(MQTTserver);


        mqttclient.set_callback(callback);


      Serial.print("Initiating MQTT Connection: ");
      if (EEPROM.read(mqttAddressbyte) == flagvalue) 
        {                   
      if (!mqttclient.connected()) 
        {             // - check to see if connected 
          
          Serial.print("Connecting: (") ;
            Serial.print(MQTTserver);
            Serial.print(")....");
          if(mqttclient.connect(deviceid)) 
          
                {     //  - if not connected then connect 
                Serial.println("Success");
   
// - subscribe                
                mqttclient.subscribe(deviceid);       // - Then subscribe to device messages 
                mqttclient.subscribe(mqttesptopic);    // ---  subscribe to all esp messages 

                send_mqtt_msg( String(deviceid), LocalIP,2); // the 2 signifies that it publishes under the esp/ topic and not device
                delay(10);
                send_mqtt_msg( "IP", LocalIP);                
                delay(10);
                send_mqtt_msg( "Version", version);                
                delay(10);
                send_mqtt_msg( "Status", "Device Ready");
              
            } else
                {
                Serial.println("Failed");

                }
  
        } else {
                Serial.println("Connected already");
         } 
        } else {
                Serial.println("No MQTT Server is Defined");
        }
     } 
  
  void mqttbufcharclear ()
  {
    for (int i = 0; i <200; i++)
    { 
      mqttcharbuf[i] = 0;
    }
    
  } 


void send_mqtt_msg (String topic, String message, int type )

{
  if (!MQTT_enabled) return;

  char mqtttopicbuf[200];
 
 if ( type == 1 ) topic = String(deviceid) +"/" + topic;
 if ( type == 2 ) topic = "esp/" + topic;
 
 
     
   topic.toCharArray(mqtttopicbuf,200);
   message.toCharArray(mqttcharbuf,200);
 
  if (mqttclient.connected()) 
    {
       if(mqttclient.publish(mqtttopicbuf, mqttcharbuf))
       {
            Serial.println("MQTT msg SENT: " + topic + ", Message: " + message);
        } else {
            Serial.println("MQTT msg Failed: " + topic + ", Message: " + message);
        }
        
    }
    
}

void send_mqtt_msg (String topic, String message ) // overloading functon to allow default msg send...
{
  
  send_mqtt_msg ( topic, message, 1); 
  
}


void handle_mqtt() {
 if (server.arg("mqttserver").length() != 0) mqttserver_command(server.arg("mqttserver")); 
 if (server.arg("reboot").length() != 0) ESP.reset(); // abort();

  if (server.arg("form_MQTT_enabled").length() != 0) 
  {

    if (server.arg("form_MQTT_enabled") == "YES") {
      MQTT_enabled = true;
      if (EEPROM.read(MQTTenabledbyte) != flagvalue) EEPROM.write(MQTTenabledbyte, flagvalue); // write mqtt enabled byte....
      EEPROM.commit();
      Serial.println("MQQT ENABLED");
 
      initiatemqqt();
    }

    if (server.arg("form_MQTT_enabled") == "NO") {
    MQTT_enabled = false;
    if (EEPROM.read(MQTTenabledbyte) != 0) EEPROM.write(MQTTenabledbyte, 0); // write mqtt enabled byte....
    EEPROM.commit();
    mqttclient.disconnect();      
    Serial.println("MQQT DISABLED");

    }

  }



  String MQTT_enabled_checked_yes;
  String MQTT_enabled_checked_no;

  String form_Uptime_enabled_yes;
  String form_Uptime_enabled_no;

  int mqttconnected = mqttclient.connected();

  if (MQTT_enabled == true) {

  MQTT_enabled_checked_yes = "checked" ;
  MQTT_enabled_checked_no = " ";
  }
  else {
  MQTT_enabled_checked_yes = " " ;
  MQTT_enabled_checked_no = "checked";
   }

  //int MQTT_enabled_checked = MQTT_enabled;
  
  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head><meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>MQTT Configuration</title></head>\n<body><h1>MQTT Config</h1>\n";
  
  httpbuf += "<form action='/mqtt' method='POST'> ENABLED: <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='NO'" + MQTT_enabled_checked_no  + "> NO <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='YES'"+ MQTT_enabled_checked_yes +  "> YES" ;  
  httpbuf += "</form>";
 if(MQTT_enabled) {
  
  httpbuf += "<br>MQTT Server is: " + String(mqttserver) + "..." + ((mqttconnected)?"<font color='green'> Connected </font>":"<font color='red'> Disconnected </font>");
  httpbuf += "<br>Current device name is: <a href='http://" + String(deviceid) + ".local'>" + String(deviceid) + ".local</a>";
  httpbuf += "<br><form action='/mqtt' method='POST'>\n";
  httpbuf += "\n\nNew Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>";
  httpbuf += "\n\nMQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>";
  httpbuf += "\n\nEnable Uptime <input type='radio' name='form_Uptime_enabled' value='NO'" + form_Uptime_enabled_no  + "> NO <input type='radio' name='form_Uptime_enabled' value='YES'"+ form_Uptime_enabled_yes +  "> YES" ;

  //httpbuf += "<input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<\p>"; 
  //httpbuf += "\n\nSSID: <input type='text' id='ssid' name='ssid' value=''><br/>";
  //httpbuf += "\nPassword: <input type='text' name='password' value=''><br/></p>";
  //httpbuf += "<input type='submit' value='Submit'></form>"; 
  httpbuf += "<p><input type='submit' name='reboot' value='Reboot!'/>\n";
  //httpbuf += "  <input type='submit' name ='scan' value='Scan'/>";   
    // working httpbuf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  //httpbuf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  httpbuf += "  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 

}

  httpbuf += htmlendstring; 



    server.send(200, "text/html", httpbuf);

     
    if (networkrestart) restartNetworking(); 

    if (mqttreload) mqttreloadfunc();

    //server.send(200, "text/plain", httpbuf);
  //server.send(200, "text/plain", String("MQTT Coming soon....."));
}
