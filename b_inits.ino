//#define PIN 15 // not sure what this is for...
ESP8266WebServer server(80);

WiFiClient wifiClient;
//PubSubClient mqttclient(mqttserver, 1883, callback, wifiClient);
 //     IPAddress testserver(192, 168, 1, 24);
// int* testserver[4] = {192,168,1,24};

PubSubClient mqttclient(wifiClient) ; 

// multicast DNS responder
//MDNSResponder mdns; // not needed anymore... still compiled though!  

 /// set timer....
SimpleTimer timer;

//AnimatedObject animatedobject(strip, animator);


  //NeoPixelBus strip = NeoPixelBus(120, 2);

  NeoPixelBus* strip = NULL; // dynamic
  NeoPixelAnimator* animator = NULL;
  uint8_t* pixelsPOINT = NULL;

//AnimatedObject* animatedobject = NULL;   All moved to dynamic functions... better that way.. 

E131 e131; // = NULL;


WiFiUDP Udp;

//WiFiUDP listener;
WiFiServer TelnetServer(aport);

WiFiClient Telnet;
WiFiUDP OTA;

// web pages....

// #include "page_jscolour.h"
// #include "page_microajax.js.h"
// #include "page_root.h"
// #include "page_style.css.h"
