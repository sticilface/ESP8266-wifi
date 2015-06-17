//#define cache ICACHE_FLASH_ATTR
#define cache  
// Wifi parameters
const int BUFSIZE = 32; //was 16 

char ssid[BUFSIZE]; // = "fyffest";
char password[BUFSIZE]; //  = "wellcometrust";

//const char *ssidb = "fyffest";
//const char *passwordb = "wellcometrust";

char deviceid[BUFSIZE];// = "";
//String deviceid;

String clientName = " ";
String LocalIP = " ";

char bufchar[BUFSIZE];
//String buf = " " ;
//String httpbuf = " ";
//String bytedump = " ";

char mqttcharbuf[200];

String mqttbuf = " ";

char mqttesptopic[BUFSIZE] = "esp";  //Required to define the GLOBAL MQTT RESPONSE!

uint8_t wifinetworksfound = 0;
uint8_t wifimode = 0;

const int APtimeout = 300000;
boolean APtimeout_done = false;
const int MQTTtimeout = 600000;
const int MQTTtemptimeout = 60000;
const int Uptimer_timeout = 5000;

// Action boolean perams .... memory functions
boolean networkrestart = false;
boolean paramsreload = false;
boolean mqttreloadvar2 = false;
boolean mqttreload = false;


const byte flagvalue = 199;  // Used to identify which settings are saved. 

const int deviceidAddress = 32;
const int ssidAddress = 64;
const int passwordAddress = 96;
const int mqttAddress = 128;  // ends at 144 so begin new data at 208?  gives space for another 2 variables 
const int serialspeed_address = 208;  // next slot 240

const int deviceidAddressbyte = 1;
const int ssidAddressbyte = 2;
const int passwordAddressbyte = 3;
const int mqttAddressbyte = 4;
const int APbyte = 5; // Used to set emergency access mode...
const int MQTTenabledbyte = 6;
const int DEBUGenabledbyte = 7;
const int SERIALspeedbyte = 8;



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
IPAddress multicast_ip_addr(224,0,0,0);
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

enum operatingState { 
OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, 
ADALIGHT, TEST, LOOPAROUND,PICKRANDOM,FADEINFADEOUT,
COOLBLOBS,UDP,RAINBOWCYCLE,SPIRAL,SQUARES2,
SQUARESRANDOM,TEST4,SQUARES,EQ1,RANDOM,
RANDOMFUNC, ARTNET, RANDOM_TOP_BOTTOM, RANDOM_TOP_BOTTOM_LINEAR, SINGLE_COLOUR_FADE,
RANDOM_COLOUR_FADE, HSICYCLE
};
operatingState opState = OFF;
operatingState LastOpState = OFF;
operatingState HoldingOpState = OFF; 
static const char *MODE_STRING[] = {
"Off", "Rainbow", "Color", "Chase Rainbow", "Fade", 
"AdaLight", "test", "Looparound", "Pick Random", "Fade in Fade Out", 
"Cool Blobs", "UDP", "Rainbow Cycle","Spiral","Squares2",
"Squares Random","Test4","Squares","EQ1","Random Colour", 
"Random Function", "Art-Net", "HSV Top Bottom", "Linear Top Bottom", "Single Colour Fade", 
"Random Colour Fade", "HSIcycle"
};
#define numberofmodes 27 /////// DONT FORGET THIS....

uint16_t pixelCount = 40;
uint8_t pixelPIN = 2;
uint8_t CurrentBrightness = 255; 
bool paused = false; 

uint16_t CurrentAnimationSpeed = 2000; 

RgbColor NewColour = RgbColor(0,0,0);

unsigned int localPort = 8888; // 6454; //8888;      // local port to listen on

enum effectState { PRE_EFFECT = 0, RUN_EFFECT, POST_EFFECT};
effectState Current_Effect_State = PRE_EFFECT; 

