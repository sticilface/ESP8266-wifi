void loop() {
  // put your main code here, to run repeatedly:
  mdns.update();
  
 //  if(!mqttclient.loop()) initiatemqqt();  //---- Do not use this... causes shit... call every 10 min maybe instead...
  
  delay(20);

  mqttclient.loop();
  
  delay(20);

  server.handleClient();
  
  delay(20);
  
  if ((millis() > AP_STA_timer + APtimeout) && (!APtimeout_done))
      {
        deactivateAP();
        APtimeout_done = true;
      }
 delay(5);
 
  if(millis() > MQTT_connect_timer + MQTTtimeout) 
      {
        MQTT_connect_timer = millis();
        initiatemqqt();
      }
 delay(5);

   /* if ((millis() > AP_STA_timer + APtimeout) && (!APtimeout_done))
      {
        deactivateAP();
        APtimeout_done = true;
      } */
 


  //delay(5);
 
  /* if(millis() > MQTT_connect_timer + MQTTtimeout) 
      {
        MQTT_connect_timer = millis();
        initiatemqqt();
      } */
  



  //delay(5);



  /*if (millis() > Uptime_timer + Uptimer_timeout)
>>>>>>> Stashed changes
      {
        //long uptimetemp = millis()/60000
        send_mqtt_msg( "Uptime", String(millis()/60000));
        Uptime_timer = millis();
<<<<<<< Updated upstream
      }
=======
      } */

 ////////////// -------- PLUGIN CONTROLs -----------------------
   timer.run();

 
loop_Plugin();
 
    //CPU_clock_count();

 
}
