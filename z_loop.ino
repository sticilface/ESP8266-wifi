void loop( void ) {
  // put your main code here, to run repeatedly:
if (!isOTAupdate) {

  mqttclient.loop();
  server.handleClient();
  timer.run();
  loop_Plugin();
//if (wifimode == 1) mdns.update();  

};

 
OTA_UPDATE();

yield(); 

}

