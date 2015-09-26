////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//                  MQTT config, callback, webpage....
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void cache MQTTcallback (const MQTT::Publish& pub) {

 if (pub.payload_string() == F("identify") )
      {
                Serial.print(F("Identify Request Recieved: "));
                String temp = F("esp/"); 
                temp += String(deviceid);
                
                if(mqttclient.publish(temp, IPtoString(WiFi.localIP()) )) Serial.println(F("Sent")); 
      } 
      
if (pub.payload_string() == F("reboot") || pub.payload_string() == F("restart")) ESP.reset(); //system_restart(); // abort();


mqtt_plugin_handle( pub ); // passes it on to function in plugin_commands  

} // --- end of mqtt callback



void cache initiatemqqt ()
    {
      if (!MQTT_enabled) return; 
      
        mqttclient.set_server(MQTTserver);
        mqttclient.set_callback(MQTTcallback);

        // generate topic...

        String topicString = String(deviceid) + F("/Status");
        char topic[50]; 
        topicString.toCharArray(topic,50);
        String MainTopicString = String(deviceid) + F("/+/+");

      Serial.print(F("Initiating MQTT Connection: "));

      if (EEPROM.read(mqttAddressbyte) == flagvalue) 
        {                   
      if (!mqttclient.connected()) 
        {             // - check to see if connected 
          
          Serial.print(F("Connecting: ") );
            //Serial.print(MQTTserver);
            //Serial.print(")....");

          if (  mqttclient.connect(MQTT::Connect(deviceid)
               .set_clean_session()
               .set_will(topic, F("Connection Lost!"))
               .set_keepalive(60)
              ) ) {

                Serial.println(F("Success" )) ;
                MainTopicString.toCharArray(topic,50);

            mqttclient.subscribe(MQTT::Subscribe()
                   .add_topic(topic, 2) // this is the main topic deviceid/#
                   .add_topic(mqttesptopic, 2)
                   .add_topic(deviceid,2)  // this is the esp topic... 
                   );
                send_mqtt_msg( String(deviceid), IPtoString(WiFi.localIP()) ,2); // the 2 signifies that it publishes under the esp/ topic and not device
                send_mqtt_msg( F("IP"), IPtoString(WiFi.localIP() ));   
                send_mqtt_msg( F("Version"), version);                
                send_mqtt_msg( F("Status"), F("Device Ready"));
              
            } else
                {
                Serial.println(F("Failed") );
                }
  
        } else {
                Serial.println(F("Connected already"));
         } 
        } else {
                Serial.println(F("No MQTT Server is Defined"));
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


// MQTT webpage
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
    Serial.println(F("MQQT DISABLED"));

    }

  }

  String MQTT_enabled_checked_yes;
  String MQTT_enabled_checked_no;

  String form_Uptime_enabled_yes = F("checked") ;
  String form_Uptime_enabled_no = " ";

  int mqttconnected = mqttclient.connected();

  if (MQTT_enabled == true) {

  MQTT_enabled_checked_yes = F("checked") ;
  MQTT_enabled_checked_no = " ";
  }
  else {
  MQTT_enabled_checked_yes = " " ;
  MQTT_enabled_checked_no = F("checked");
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
      
          buf += F("<br>MQTT Server is: ");
          buf += IPtoString(MQTTserver) + "..."; 
          (mqttconnected)? buf += F("<font color='green'> Connected </font>"): buf += F("<font color='red'> Disconnected </font>");
          server.client().print(buf); 
          buf = insertvariable ( FPSTR(webpage_mqtt_2), String(deviceid));
          buf = insertvariable ( buf, String(deviceid));
          buf = insertvariable ( buf, form_Uptime_enabled_no);
          buf = insertvariable ( buf, form_Uptime_enabled_yes);
          server.client().print(buf); 
      }

      server.client().print(FPSTR(htmlendstring)); 


      if (networkrestart) restartNetworking(); 
      if (mqttreload) mqttreloadfunc();


}
