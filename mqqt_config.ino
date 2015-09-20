



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
//if ((pub.topic()).indexOf("/animationspeed/set") > 0) AnimationSpeed_command_string (    pub.payload_string()  );
if ((pub.topic()).indexOf("/colour/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/color/set") > 0) WS2812_Set_New_Colour(pub.payload_string());
if ((pub.topic()).indexOf("/loadpreset/set") > 0) WS2812_preset_string(pub.payload_string());
if ((pub.topic()).indexOf("/savepreset/set") > 0) { Save_LED_Settings(pub.payload_string().toInt()); };
if ((pub.topic()).indexOf("/toggle/set") > 0) WS2812_toggle_string(pub.payload_string());
if ((pub.topic()).indexOf("/autorestart/set") > 0) WS2812_autorestart_string(pub.payload_string());



} // --- end of mqtt callback



void cache initiatemqqt ()
    {
      if (!MQTT_enabled) return; 
      
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

  if (!MQTT_enabled) return; 
  if ( type == 1 ) topic = String(deviceid) +  "/" + topic;
  if ( type == 2 ) topic = "esp/" + topic;
 
  if (mqttclient.connected()) 
    {
          mqttclient.publish(MQTT::Publish((char*)topic.c_str(), (char*)message.c_str())
          .set_qos(2)
          );        
    }
    
}



void  send_mqtt_msg (String topic, String message ) // overloading functon to allow default msg send...
{  
  send_mqtt_msg ( topic, message, 1);  
}


void cache handle_mqtt() {
  
 if (server.hasArg("mqttserver")) mqttserver_command(server.arg("mqttserver")); 
 if (server.hasArg("reboot")) ESP.reset(); // abort();

  if (server.hasArg("form_MQTT_enabled")) 
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

  String form_Uptime_enabled_yes = "checked" ;
  String form_Uptime_enabled_no = " ";

  int mqttconnected = mqttclient.connected();

  if (MQTT_enabled == true) {

  MQTT_enabled_checked_yes = "checked" ;
  MQTT_enabled_checked_no = " ";
  }
  else {
  MQTT_enabled_checked_yes = " " ;
  MQTT_enabled_checked_no = "checked";
   }

  buf = insertvariable ( FPSTR(webpage_mqtt_1) , String(deviceid));
  buf = insertvariable ( buf, String(deviceid));
  buf = insertvariable ( buf,  MQTT_enabled_checked_no);
  buf = insertvariable ( buf,  MQTT_enabled_checked_yes);


    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();

    server.client().print(buf); 
    buf = " ";

     if(MQTT_enabled) {
      
          buf += "<br>MQTT Server is: " + String(mqttserver_string) + "..." + ((mqttconnected)?"<font color='green'> Connected </font>":"<font color='red'> Disconnected </font>");
          server.client().print(buf); 
          buf = insertvariable ( FPSTR(webpage_mqtt_2), String(deviceid));
          buf = insertvariable ( buf, String(deviceid));
          buf = insertvariable ( buf, form_Uptime_enabled_no);
          buf = insertvariable ( buf, form_Uptime_enabled_yes);
          server.client().print(buf); 
      }

      server.client().print(htmlendstring); 


      if (networkrestart) restartNetworking(); 
      if (mqttreload) mqttreloadfunc();


}
