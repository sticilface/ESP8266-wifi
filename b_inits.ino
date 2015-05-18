#define PIN 15
ESP8266WebServer server(80);

WiFiClient wifiClient;
//PubSubClient mqttclient(mqttserver, 1883, callback, wifiClient);
      //IPAddress testserver(192, 168, 1, 24);
// int* testserver[4] = {192,168,1,24};


PubSubClient mqttclient;


//ESP_WS2812 strip = ESP_WS2812(60, PIN, NEO_GRB + NEO_KHZ800);
//ESP_WS2812 strip = ESP_WS2812();

 Ticker flipper;


 /// set timer....
SimpleTimer timer;

const String version = "Melvarium Beta";


  