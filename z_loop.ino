void loop( void ) {
  // put your main code here, to run repeatedly:


  mqttclient.loop();

//delay(1);

 server.handleClient();
     

//delay(1);

 ////////////// -------- PLUGIN CONTROLs -----------------------

timer.run();

 
loop_Plugin();
 
//if (wifimode == 1) mdns.update();  

 
//OTA_UPDATE();



}

