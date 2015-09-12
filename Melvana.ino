#define HSL_FLOAT
#define MDNSSERVICE

#define DEBUG_YES

//#define HEAP_TIMER

//#define STAGING
#define GAMMA_CORRECTION

#define ESPUARTWS2812 // USE UART to drive pixels, required until bitbanging is fixed. 

#define ANIMATION_LIMIT 250 // limit for number of pixels where animations work.. otherwise it crashes. 
#define MINIMUM_REFRESH 30 // minimum update frequency for WS2812

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
