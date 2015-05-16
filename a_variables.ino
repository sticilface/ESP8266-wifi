// Wifi parameters
const int BUFSIZE = 32; //was 16 

char ssid[BUFSIZE] ; //= "fyffest";
char password[BUFSIZE];// = "wellcometrust";
char deviceid[BUFSIZE];// = "";
//String deviceid;

String clientName;
String LocalIP;

char bufchar[BUFSIZE];
String buf;
String httpbuf;
String bytedump;

char mqttcharbuf[200];
String mqttbuf;
char mqttesptopic[BUFSIZE] = "esp";  //Required to define the GLOBAL MQTT RESPONSE!

int wifinetworksfound;
int wifimode;

const int APtimeout = 300000;
boolean APtimeout_done = false;
const int MQTTtimeout = 600000;
const int MQTTtemptimeout = 60000;
const int Uptimer_timeout = 60000;

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

const int deviceidAddressbyte = 1;
const int ssidAddressbyte = 2;
const int passwordAddressbyte = 3;
const int mqttAddressbyte = 4;
const int APbyte = 5; // Used to set emergency access mode...
const int MQTTenabledbyte = 6;
const int DEBUGenabledbyte = 7;
const int Temperature_enablebyte = 8; 
const int Humidity_enablebyte = 9;

//boolean MQTTenabled;

long AP_STA_timer;
long MQTT_connect_timer;
long request_timeout; 
long Uptime_timer;

// multicast DNS responder
MDNSResponder mdns;


// Features Enables

boolean MQTT_enabled = false;
boolean ThingSpeak_enable;
boolean Prowl_Enable;
int DEBUG;
boolean Plugin_enable = false;


////////////////////-----------MQTT---------------///////////////////

char mqtttopic[BUFSIZE];// = "esp";
char mqttserver[BUFSIZE];// = "192.168.1.24";


/////// ------- COMMAND ARGS -------
//const int numberofcommands = 13;
//const char* commands[numberofcommands] = {"setpoint","kd","ki","kp","state","ssid","password","eeprom","deviceid","mqttserver","restart", "send","test"};


String htmlendstring = "<p><a href='/'>Home</a>  <a href='/wifi'>WiFi</a>  <a href='/mqtt'>MQTT</a>  <a href='/misc'>MISC</a>  <br><a href='/io'>Input/Output</a> <a href='/test'>Test</a> <a href='/ws2812'>WS2812</a>";



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

enum operatingState { OFF = 0, RAINBOW, COLOR, ChaseRainbow, FADE, ADALIGHT, TEST, LOOPAROUND,PICKRANDOM,FADEINFADEOUT,COOLBLOBS};
operatingState opState = OFF;
operatingState LastOpState = LOOPAROUND;

