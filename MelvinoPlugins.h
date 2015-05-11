
// Inlude files here...

//-------  Include DEvice specific libs here   -----

//               -----  Melvide -------


#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
//#include <PID_AutoTune_v0.h>
#include <Ticker.h>

#include "DHT.h"

#define DHTPIN 12     // what pin we're connected to


#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE,15);
