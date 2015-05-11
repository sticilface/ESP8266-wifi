


//#include "MelvinoPlugins.h"


#include <stdlib.h>
#include <stdio.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>


#include <EEPROM.h>
#include <EepromUtil.h>


//#include <Ticker.h>
//#include <Esp_WS2812.h>


#include <ESP.h> // has to be below stdlib???

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

#include <Ticker.h>

#include "DHT.h"

#include <SimpleTimer.h>

