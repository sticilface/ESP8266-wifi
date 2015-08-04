void loop( void ) {
  // put your main code here, to run repeatedly:


//	static uint32_t loop_time = 0, loop_count = 0, loop_timeout = 0, loop_cycle = 0 ; 

if (!isOTAupdate) {


  mqttclient.loop();



  server.handleClient();




  timer.run();



  loop_Plugin();
//if (wifimode == 1) mdns.update();  

if (EEPROM_commit_var == true) {

EEPROM.commit();
EEPROM_commit_var = false; 

}
	
};

/*
loop_count++;

if (millis() - loop_timeout > 10000) {

 uint32_t temp = (ESP.getCycleCount() - loop_cycle ) / loop_count ; 

loop_count = loop_count / 10 ; 

 Serial.printf( "Loops per second = %u,  Average cycles per loop = %u \n", loop_count , temp  ); 


 loop_cycle = ESP.getCycleCount(); 
 loop_count = 0;
 loop_timeout = millis();
}
*/
 
OTA_UPDATE();

yield(); 

}

