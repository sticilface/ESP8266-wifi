



//void callback(char* mqtttopic, byte* payload, unsigned int length) {

void MQTTcallback (const MQTT::Publish& pub) {

  //Serial.print("MQTT Message Recieved: ");
  //Serial.print(pub.topic());
  //Serial.print(" => ");
  //Serial.println(pub.payload_string());


    //String topicrecieved = pub.topic();
    
  //  mqttbuf = pub.payload_string();

 // -- 1) Identify ESP commands
 if (pub.payload_string() == "identify") 
      {
                Serial.print("Identify Request Recieved: ");
                String temp = "esp/" + String(deviceid);
                //temp.toCharArray(mqttcharbuf,200);
                //char iparray[20];
                //LocalIP.toCharArray(iparray,20) ; // WiFi.localIP();
                
                if(mqttclient.publish(temp, LocalIP)) Serial.println("Sent"); 
      } 
      
if (pub.payload_string() == "reboot" || pub.payload_string() == "restart") ESP.reset(); //system_restart(); // abort();


//  NEW METHODS.... 
if ((pub.topic()).indexOf("/effect/set") > 0) WS2812_effect_string(pub.payload_string());
if ((pub.topic()).indexOf("/mode/set") > 0) WS2812_mode_string(pub.payload_string());
if ((pub.topic()).indexOf("/timer/set") > 0) WS2812timer_command_string(pub.payload_string());
if ((pub.topic()).indexOf("/brightness/set") > 0) WS2812_dim_string(pub.payload_string());
if ((pub.topic()).indexOf("/animationspeed/set") > 0) AnimationSpeed_command_string (    pub.payload_string()  );
if ((pub.topic()).indexOf("/colour/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/color/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/loadpreset/set") > 0) WS2812_preset_string(pub.payload_string());
if ((pub.topic()).indexOf("/savepreset/set") > 0) { Save_LED_Settings(pub.payload_string().toInt()); };
if ((pub.topic()).indexOf("/toggle/set") > 0) WS2812_toggle_string(pub.payload_string());
if ((pub.topic()).indexOf("/autorestart/set") > 0) WS2812_autorestart_string(pub.payload_string());


/*

if ((pub.payload_string()).indexOf('=') > 0) 
      {
       String instruction = (pub.payload_string()).substring(0,mqttbuf.indexOf('=') );

       String value = (pub.payload_string()).substring((pub.payload_string()).indexOf('=') +1 ,(pub.payload_string()).length());
       
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
       

       // if (instruction == "mode")  WS2812_mode_string(value);
       // if (instruction == "timer")  WS2812timer_command_string(value);
       // if (instruction == "brightness")  WS2812_dim_string(value);
       // if (instruction == "animationspeed") CurrentAnimationSpeed = value.toInt();

       
      } 
      
*/


} // --- end of mqtt callback



void cache initiatemqqt ()
    {

        mqttclient.set_server(MQTTserver);
        mqttclient.set_callback(MQTTcallback);

        // generate topic...

        String topicString = String(deviceid) + "/Status";
        char topic[50]; 
        topicString.toCharArray(topic,50);
        String MainTopicString = String(deviceid) + "/+/+";

      Serial.print("Initiating MQTT Connection: ");

      if (EEPROM.read(mqttAddressbyte) == flagvalue) 
        {                   
      if (!mqttclient.connected()) 
        {             // - check to see if connected 
          
          Serial.print("Connecting: ") ;
            //Serial.print(MQTTserver);
            //Serial.print(")....");

          if (  mqttclient.connect(MQTT::Connect(deviceid)
               .set_clean_session()
               .set_will(topic, "Connection Lost!")
               .set_keepalive(60)
              ) ) {

                Serial.println("Success");
                MainTopicString.toCharArray(topic,50);

            mqttclient.subscribe(MQTT::Subscribe()
                   .add_topic(topic, 2) // this is the main topic deviceid/#
                   .add_topic(mqttesptopic, 2)
                   .add_topic(deviceid,2)  // this is the esp topic... 
                   );
                //delay(5);
                send_mqtt_msg( String(deviceid), LocalIP,2); // the 2 signifies that it publishes under the esp/ topic and not device
                //delay(5);
                send_mqtt_msg( "IP", LocalIP);   
                //delay(5);
                send_mqtt_msg( "Version", version);                
                //delay(5);

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
  
void cache mqttbufcharclear ()
  {
    for (int i = 0; i <200; i++)
    { 
      mqttcharbuf[i] = 0;
    }
    
  } 


void  send_mqtt_msg (String topic, String message, int type )

{
  //static long lastmessage; 

  if (!MQTT_enabled) return;

  //char mqtttopicbuf[1000];
 
 if ( type == 1 ) topic = String(deviceid) +  "/" + topic;
 if ( type == 2 ) topic = "esp/" + topic;
 
 
     
  // topic.toCharArray(mqtttopicbuf,200);
  // message.toCharArray(mqttcharbuf,200);
 
  if (mqttclient.connected()) 
    {
       //if(mqttclient.publish((char*)topic.c_str(), (char*)message.c_str()))
     // if(mqttclient.publish(topic, message))
     //  {
          //  Serial.println("MQTT msg SENT: " + topic + ", Message: " + message);
     //   } else {
     //       Serial.println("MQTT msg Failed: " + topic + ", Message: " + message);
     //   }
//animator->Pause();

  mqttclient.publish(MQTT::Publish((char*)topic.c_str(), (char*)message.c_str())
                .set_qos(2)
               );        
    }
    //delay(2);

//animator->Resume();
    
}

void  send_mqtt_msg (String topic, String message ) // overloading functon to allow default msg send...
{
  
  send_mqtt_msg ( topic, message, 1); 
  
}


void cache handle_mqtt() {

  String buf; 
  
 if (server.arg("mqttserver").length() != 0) mqttserver_command(server.arg("mqttserver")); 
 if (server.arg("reboot").length() != 0) ESP.reset(); // abort();

  if (server.arg("form_MQTT_enabled").length() != 0) 
  {

    if (server.arg("form_MQTT_enabled") == "YES") {
      MQTT_enabled = true;
      if (EEPROM.read(MQTTenabledbyte) != flagvalue) EEPROM.write(MQTTenabledbyte, flagvalue); // write mqtt enabled byte....
      EEPROM_commit_var = true;
      Serial.println("MQQT ENABLED");
      initiatemqqt();
    }

    if (server.arg("form_MQTT_enabled") == "NO") {
    MQTT_enabled = false;
    if (EEPROM.read(MQTTenabledbyte) != 0) EEPROM.write(MQTTenabledbyte, 0); // write mqtt enabled byte....
    EEPROM_commit_var = true;
    mqttclient.disconnect();      
    Serial.println("MQQT DISABLED");

    }

  }



  String MQTT_enabled_checked_yes;
  String MQTT_enabled_checked_no;

  String form_Uptime_enabled_yes = F("checked") ;
  String form_Uptime_enabled_no = F(" ");

  int mqttconnected = mqttclient.connected();

  if (MQTT_enabled == true) {

  MQTT_enabled_checked_yes = F("checked") ;
  MQTT_enabled_checked_no = F(" ");
  }
  else {
  MQTT_enabled_checked_yes = F(" ") ;
  MQTT_enabled_checked_no = F("checked");
   }

  //int MQTT_enabled_checked = MQTT_enabled;
  
  String content = F("\
      <!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'><title> % </title></head><body><h1> % </h1>\
      <title>MQTT Configuration</title></head><body><h1>MQTT Config</h1>\
      <form action='/mqtt' method='POST'> ENABLED:\
      <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='NO' % > NO \
      <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='YES' %> YES \
      </form>");

  buf = insertvariable ( content, String(deviceid));
  buf = insertvariable ( buf, String(deviceid));
  buf = insertvariable ( buf,  MQTT_enabled_checked_no);
  buf = insertvariable ( buf,  MQTT_enabled_checked_yes);


    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();

   server.sendContent(buf);
   buf = F(" ");
// 1 = String(mqttserver_string)

 if(MQTT_enabled) {
  buf += "<br>MQTT Server is: " + String(mqttserver_string) + "..." + ((mqttconnected)?"<font color='green'> Connected </font>":"<font color='red'> Disconnected </font>");

  //buf = "<br>MQTT Server is: %..." + ((mqttconnected)?"<font color='green'> Connected </font>":"<font color='red'> Disconnected </font>";
  //buf = insertvariable ( buf, String(deviceid));
  server.sendContent(buf);


  content = F("<br>Current device name is: <a href='http://%.local'>%</a>\
  <br><form action='/mqtt' method='POST'>\
  New Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>\
  MQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>\
  Enable Uptime <input type='radio' name='form_Uptime_enabled' value='NO' %> NO <input type='radio' name='form_Uptime_enabled' value='YES' %> YES\
  <p><input type='submit' name='reboot' value='Reboot!'/>\
  <input type='submit' value='Submit'/>\
  </form></p>"); 

  buf = insertvariable ( content, String(deviceid));
  buf = insertvariable ( buf, String(deviceid));
  buf = insertvariable ( buf, form_Uptime_enabled_no);
  buf = insertvariable ( buf, form_Uptime_enabled_yes);


  //buf += "<input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off<\p>"; 
  //buf += "\n\nSSID: <input type='text' id='ssid' name='ssid' value=''><br/>";
  //buf += "\nPassword: <input type='text' name='password' value=''><br/></p>";
  //buf += "<input type='submit' value='Submit'></form>"; 
  //buf += "<p><input type='submit' name='reboot' value='Reboot!'/>\n";
  //buf += "  <input type='submit' name ='scan' value='Scan'/>";   
    // working buf += "  <input type='button' onClick='window.location.reload()' value='Refresh'/>\n" ;
  //buf += "  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>\n" ;
  //buf += "  <input type='submit' value='Submit'/>" ; 
  //buf += "</form></p>"; 
   server.sendContent(buf);

}
  server.sendContent(htmlendstring);

  //buf += htmlendstring; 



  //  server.send(200, "text/html", buf);

     
    if (networkrestart) restartNetworking(); 

    if (mqttreload) mqttreloadfunc();

    //server.send(200, "text/plain", buf);
  //server.send(200, "text/plain", String("MQTT Coming soon....."));
}
