




// Wifi parameters
const int BUFSIZE = 32; //was 16 
const String version =  "WS2812-OTA";
const char compile_date[] = __DATE__ " " __TIME__;
String buf; // use for HTTP 

char ssid[BUFSIZE] ;// = "fyffest";
char password[BUFSIZE] ;//  = "wellcometrust";

//const char *ssidb = "fyffest";
//const char *passwordb = "wellcometrust";

char deviceid[BUFSIZE];// = "";
//String deviceid;

String clientName = " ";
String LocalIP = " ";

const uint16_t aport = 8266; // tvlights
//const uint16_t aport = 8267; // lamp3
//const uint16_t aport = 8268; // bathroom
//const uint16_t aport = 8269; // NODE-MCU

const unsigned int UDPlightPort = 8888; // 6454; //8888;      // local port to listen on

/*  NOT IN  USE... but should be... 
*
*   //const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
*
*const char serverIndex[] PROGMEM = const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
*FPSTR(serverIndex) to use this...
*/

char bufchar[BUFSIZE];


bool isOTAupdate = false; 
char mqttcharbuf[200];

String mqttbuf = " ";

char mqttesptopic[BUFSIZE] = "esp";  //Required to define the GLOBAL MQTT RESPONSE!

uint8_t wifinetworksfound = 0;
uint8_t wifimode = 0;

const int APtimeout = 300000;
boolean APtimeout_done = false;
const int MQTTtimeout = 600000;
const int MQTTtemptimeout = 60000;
const int Uptimer_timeout = 1000;

// Action boolean perams .... memory functions
boolean networkrestart = false;
boolean paramsreload = false;
boolean mqttreloadvar2 = false;
boolean mqttreload = false;


const byte flagvalue = 199;  // Used to identify which settings are saved. // NOT a memory location... 

const uint16_t deviceidAddress = 32;
const uint16_t ssidAddress = 64;
const uint16_t passwordAddress = 96;
const uint16_t mqttAddress = 128;  // ends at 144 so begin new data at 208?  gives space for another 2 variables 
const uint16_t serialspeed_address = 132;  // next slot 240
const uint16_t PixelPIN_address = 133;
const uint16_t PixelCount_address = 134;  //( to 135)   double so requires 2..
const uint16_t LastOpState_Address = 136;  // NOT NEEDED ANY MORE BUT check... might use to signify loaded settings, so it goes back to that..
const uint16_t AutoRestartEffectAddress = 137; 
const uint16_t ON_OFF_State_Address = 138; 


const uint16_t START_address_settings = 160;


const uint8_t deviceidAddressbyte = 1;
const uint8_t ssidAddressbyte = 2;
const uint8_t passwordAddressbyte = 3;
const uint8_t mqttAddressbyte = 4;
const uint8_t APbyte = 5; // Used to set emergency access mode...
const uint8_t MQTTenabledbyte = 6;
const uint8_t DEBUGenabledbyte = 7;
const uint8_t SERIALspeedbyte = 8;

const uint8_t PixelPIN_enablebyte = 9; 
const uint8_t PixelCount_enablebyte = 10;




//boolean MQTTenabled;

long AP_STA_timer = 0;
long MQTT_connect_timer = 0;
long request_timeout = 0; 
long Uptime_timer = 0;




// Features Enables

boolean MQTT_enabled = false;
//boolean ThingSpeak_enable;
//boolean Prowl_Enable;
int DEBUG = false;
boolean Plugin_enable = false;


IPAddress MQTTserver(0,0,0,0);
const IPAddress multicast_ip_addr(224,0,0,0);
////////////////////-----------MQTT---------------///////////////////

char mqtttopic[BUFSIZE];// = "esp";
// char mqttserver[BUFSIZE];// = "192.168.1.24";
String mqttserver_string = " "; 

/////// ------- COMMAND ARGS -------
//const int numberofcommands = 13;
//const char* commands[numberofcommands] = {"setpoint","kd","ki","kp","state","ssid","password","eeprom","deviceid","mqttserver","restart", "send","test"};


const String htmlendstring = "<p><a href='/'>Home</a>  <a href='/wifi'>WiFi</a>  <a href='/mqtt'>MQTT</a>  <a href='/misc'>MISC</a>  <br><a href='/io'>Input/Output</a> <a href='/test'>Test</a> <a href='/ws2812'>WS2812</a>";
#define numberofbaudrates 6
const long baudrates[numberofbaudrates] = {9600,115200,256000,460800,921600,2000000};
uint8_t currentspeed = 2; 
long serialspeed = 115200; 


boolean light_status = false;

/////// ---- bitwise testing

const int bitaddress = 500;

#define PIN_1 0x01
#define PIN_2 0x02
#define PIN_3 0x04
#define PIN_4 0x08
#define PIN_5 0x10
#define PIN_6 0x20
#define PIN_7 0x40
#define PIN_8 0x80


BlendMethod Blend = HSL; 

enum operatingState { 
OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, 
ADALIGHT, TEST, LOOPAROUND,PICKRANDOM,FADEINFADEOUT,
COOLBLOBS,UDP,RAINBOWCYCLE,SPIRAL,SQUARES2,
SQUARESRANDOM,TEST4,SQUARES,EQ1,RANDOM,
RANDOMFUNC, ARTNET, RANDOM_TOP_BOTTOM, RANDOM_TOP_BOTTOM_LINEAR, SINGLE_COLOUR_FADE,
RANDOM_COLOUR_FADE, HSICYCLE, NEWANIMATIONS
};
operatingState opState = OFF;
operatingState LastOpState = OFF;
operatingState HoldingOpState = OFF; 
static const char *MODE_STRING[] = {
"Off", "Rainbow", "Color", "Chase-Rainbow", "Fade", 
"AdaLight", "test", "Looparound", "Pick-Random", "Fadein-FadeOut", 
"Cool-Blobs", "UDP", "Rainbow-Cycle","Spiral","Squares2",
"Squares-Random","Test4","Squares","EQ1","Random-Colour", 
"Random-Function", "Art-Net", "HSV-Top-Bottom", "Linear-Top-Bottom", "Single-Colour-Fade", 
"Random-Colour-Fade", "HSIcycle", "New-Animations"
};
#define numberofmodes 28 /////// DONT FORGET THIS....


uint32_t lasteffectupdate = 0; 
uint16_t WS2812interval = 2000; 
uint32_t power = 0; 

uint8_t current_loaded_preset = 0; 
bool current_loaded_preset_changed = false; 
bool EEPROM_commit_var = false; 

//uint8_t CurrentPreset = 0;


uint16_t pixelCount = 40;
uint8_t pixelPIN = 2;
uint8_t CurrentBrightness = 255; 
bool paused = false; 
bool LED_Settings_Changed = false; 
uint16_t CurrentAnimationSpeed = 2000; 
uint8_t IntervalMultiplier = 1; 

RgbColor NewColour = RgbColor(0,0,0);

String WebRGBcolour = "000000"; // This is for the WEBPAGE... takes the value when colour is changed...



enum effectState { PRE_EFFECT = 0, RUN_EFFECT, POST_EFFECT};
effectState Current_Effect_State = PRE_EFFECT; 

unsigned long Random_func_timeout = 0;

uint16_t effectPosition = 0;

uint16_t 
var1 = 0,var2 = 0,var3 = 0,var4 = 0,var5 = 0,
var6 = 0,var7 = 0,var8 = 0,var9 = 0,var10 = 0;

static const char *VAR_STRING[] = {
"Ceiling           ", // var 1
"Floor             ", // var 2
"Var3              ", // var 3
"Var4              ", // var 4
"IntervalMultiplier", // var 5
"Effect Option     ", // var 6
"Total_X           ", // var 7
"Number of effects ", // var 8
"Var9              ", // var 9
"Size of effect    "};// var 10


#ifdef LOOPDEBUG
bool temp_bug_track = false ; 
#endif

uint32_t temp_unfinished = 0;
uint32_t temp_lastunfinished = 0; 

//////////////////////////// --  WEB PAGES -----


const char webpage_ws2812[] PROGMEM = "\
<!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'><title> % </title></head><body><h1> % </h1>\
%\
<br> <a href='/ws2812?mode=off'>OFF</a> | <a href='/ws2812?mode=on'>ON</a>  | % | <a href='/ws2812?mode=refresh'>REFRESH</a> | <a href='/lightsconfig'>CONFIG</a>\
<br> PRESET: <a href='/ws2812?preset=1'>1</a> | <a href='/ws2812?preset=2'>2</a> | <a href='/ws2812?preset=3'>3</a> | <a href='/ws2812?preset=4'>4</a> | <a href='/ws2812?preset=5'>5</a> | <a href='/ws2812?preset=6'>6</a> | <a href='/ws2812?preset=7'>7</a> | <a href='/ws2812?preset=8'>8</a> | <a href='/ws2812?preset=8'>8</a> | <a href='/ws2812?preset=9'>9</a> | <a href='/ws2812?preset=10'>10</a>\
<form name=frmTest action='/ws2812' method='POST'>\
Select Mode <select name='modedrop' onchange='this.form.submit();'>\
";




/*
 static const char* _ssdp_schema_template = 
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/xml\r\n"
  "Connection: close\r\n"
  "Access-Control-Allow-Origin: *\r\n"
  "\r\n"
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
    "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
    "</specVersion>"
    "<URLBase>http://%u.%u.%u.%u:%u/</URLBase>" // WiFi.localIP(), _port
    "<device>"
      "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
      "<friendlyName>%s</friendlyName>"
      "<presentationURL>%s</presentationURL>"
      "<serialNumber>%s</serialNumber>"
      "<modelName>%s</modelName>"
      "<modelNumber>%s</modelNumber>"
      "<modelURL>%s</modelURL>"
      "<manufacturer>%s</manufacturer>"
      "<manufacturerURL>%s</manufacturerURL>"
      "<UDN>uuid:%s</UDN>"
    "</device>"
  "</root>\r\n"
  "\r\n"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353"
"balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353balls12345535353";

*/


