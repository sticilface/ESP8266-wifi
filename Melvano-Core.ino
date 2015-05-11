// -MELVANA - MELVIDE

/*  

Core Master --- This version does not contain any of the Melvide Code... A blank Canvas if you wish.....

DEv Melvarium FIX only

Second fix working only on this branch.. want this to stay here

Add in PUSH to all.... woohoooo

*/

const String version = "Melvarium Beta";

//#include "MelvinoPlugins.h"

#include <PubSubClient.h>

#include <stdlib.h>

#include <ESP8266mDNS.h>

#include <EepromUtil.h>

#include <ESP8266mDNS.h>
#include <stdio.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <WiFiUdp.h>
#include <EEPROM.h>

//#include <Ticker.h>
//#include <Esp_WS2812.h>


#include <ESP.h>
extern "C" {
#include "ets_sys.h"
#include "c_types.h"
#include "osapi.h"
#include "user_interface.h"
}


/// Plugins....

//#include <DallasTemperature.h>
//#include <OneWire.h>
#include <PID_v1.h>
//#include <PID_AutoTune_v0.h>
#include <Ticker.h>

#include "DHT.h"

#include <SimpleTimer.h>

