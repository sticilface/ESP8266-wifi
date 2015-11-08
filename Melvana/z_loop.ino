void loop( void ) {


#ifdef HEAP_TIMER
  static uint32_t  loop_timeout = 0; //loop_cycle = 0 ; 
#endif

if (!isOTAupdate) {

 	  mqttclient.loop();
 	  yield();  	

  	server.handleClient();
  	yield();
  	
  	timer.run();
  	yield();

  	loop_Plugin();
  	yield();


//  Async EEPROM write... wait 500mSec

  	if (EEPROM_commit_var == true) {
	   	static unsigned long eepromcommit_timer = 0;
	  	if (eepromcommit_timer == 0 ) eepromcommit_timer = millis();
	   		if (millis() - eepromcommit_timer > 200) { // wait two seconds to save changes...
		  	EEPROM.commit(); // takes 35msec to perform. 
			  eepromcommit_timer = 0;
			  EEPROM_commit_var = false;
			}	 
	 }

  }; // END of NOT OTA check!  


#ifdef HEAP_TIMER
 if (millis() - loop_timeout > 500) {
   Serial.println(ESP.getFreeHeap());
    loop_timeout = millis();
 };
#endif

 
OTA_UPDATE();

yield(); 

}

