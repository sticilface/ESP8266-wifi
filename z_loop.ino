void loop( void ) {
  // put your main code here, to run repeatedly:

//yield();
  mqttclient.loop();

//delay(1);
//yield();

 server.handleClient();
     

//delay(1);

 ////////////// -------- PLUGIN CONTROLs -----------------------
//yield();

timer.run();
//yield();

 
loop_Plugin();
 
//if (wifimode == 1) mdns.update();  

 
//OTA_UPDATE();

yield(); 

}

