#define HSL_FLOAT
#define MDNSSERVICE

#define DEBUG_YES


//#define STAGING
#define GAMMA_CORRECTION

#define ESPUARTWS2812 // USE UART to blast them out...

#define ANIMATION_LIMIT 300 

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
#include <E131.h>


#include "enumerates.h"
#include "a_0plugin_headers.h"
#include "eeprom_writeanything.h"



#ifdef STAGING
	//ADC_MODE(ADC_VCC);
#endif
