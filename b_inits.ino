#define PIN 15
ESP8266WebServer server(80);

WiFiClient wifiClient;
PubSubClient mqttclient(mqttserver, 1883, callback, wifiClient);

//ESP_WS2812 strip = ESP_WS2812(60, PIN, NEO_GRB + NEO_KHZ800);
//ESP_WS2812 strip = ESP_WS2812();

 Ticker flipper;


 /// set timer....
SimpleTimer timer;

const String version = "Melvarium Beta";

//Usage.....
/*
  // timed actions setup
  timer.setInterval(15000, RepeatTask);
  timer.setTimeout(10000, OnceOnlyTask);
  timer.setInterval(1000, DigitalClockDisplay);
  timer.setTimer(1200, TenTimesTask, 10);
  */
  