#define HSL_FLOAT
#define MDNSSERVICE
//#define DEBUG_YES
//#define STAGING



#include <WifiUDP.h>

#ifdef MDNSSERVICE
	#include <ESP8266mDNS.h>
#endif


#include <ESP8266WiFi.h>

 #ifdef STAGING
 	#include <FS.h>
 #endif


#include <ESP8266WebServer.h>

#include <PubSubClient.h>
#include <EEPROM.h>
#include <SimpleTimer.h>
#include <NeoPixelBus.h>



#include "enumerates.h"
#include "a_0plugin_headers.h"
#include "eeprom_writeanything.h"

#ifdef STAGING
	//ADC_MODE(ADC_VCC);
#endif
