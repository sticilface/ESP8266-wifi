//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//											Core here
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define MDNSSERVICE

// ---------------------------   ODD Defines for Debuging ----------------------------
//#define DEBUG_YES
//#define HEAP_TIMER
//#define STAGING
//#define WIP // This is work in progress... so probably will break things...
//#define USE_COLOUR_OPERATORS  This doesn't work 

// ----------------------------------------------------------------------------------- 


#include <ESP8266WiFi.h>
#include <WifiUDP.h>

#ifdef MDNSSERVICE
	#include <ESP8266mDNS.h>
#endif

#include <ESP8266WebServer.h>

#include <PubSubClient.h>
#include <EEPROM.h>
#include <SimpleTimer.h>


#include "eeprom_writeanything.h"
#include "F_Strings.h"


#define cache  ICACHE_FLASH_ATTR

#ifdef DEBUG_YES
	#define Debug(x)    Serial.print(x)
	#define Debugln(x)  Serial.println(x)
	#define Debugf(...) Serial.printf(__VA_ARGS__)
	#define Debugflush  Serial.flush
#else
	#define Debug(x)    {}
	#define Debugln(x)  {}
	#define Debugf(...) {}
	#define Debugflush  {}
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//											Plugin stuff below here
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <NeoPixelBus.h>
#include <E131.h>

#include "enumerates.h"


#define HSL_FLOAT
#define GAMMA_CORRECTION
#define ESPUARTWS2812 // USE UART to drive pixels, required until bitbanging is fixed. 
#define ANIMATION_LIMIT 250 // limit for number of pixels where animations work.. otherwise it crashes. 
#define MINIMUM_REFRESH 30 // minimum update frequency for WS2812



