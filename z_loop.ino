void loop( void ) {
  // put your main code here, to run repeatedly:

#ifdef LOOPDEBUG
	if (temp_bug_track) Serial.print("6,");
#endif
	static uint32_t loop_time = 0, loop_count = 0, loop_timeout = 0, loop_cycle = 0 ; 

if (!isOTAupdate) {

#ifdef LOOPDEBUG
	if (temp_bug_track) Serial.print("7,");
#endif

  mqttclient.loop();

#ifdef LOOPDEBUG
	if (temp_bug_track) Serial.print("8,");
#endif

  server.handleClient();

#ifdef LOOPDEBUG
	 if (temp_bug_track) Serial.print("9,");
#endif


  timer.run();

#ifdef LOOPDEBUG
  if (temp_bug_track) Serial.print("10,");
#endif

  loop_Plugin();
//if (wifimode == 1) mdns.update();  
#ifdef LOOPDEBUG
	if (temp_bug_track) Serial.print("5,");
#endif
	
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

