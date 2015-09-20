
///////////////////////////////////////////////////////////////////////////////////////////
//
//                                           Globals  
//
//////////////////////////////////////////////////////////////////////////////////////////



// Wifi parameters
const int BUFSIZE = 32; 
const String version =  "WS2812-OTA";
const char compile_date[] = __DATE__ " " __TIME__;


String buf; // use for HTTP 

char ssid[BUFSIZE] ;
char password[BUFSIZE] ;
char deviceid[BUFSIZE];  

const uint16_t aport = 8266; 
bool isOTAupdate = false; 

const uint16_t UDPlightPort = 8888; 
const IPAddress multicast_ip_addr(224,0,0,0); // Multicast broadcast address


uint8_t wifimode = 0;
const int APtimeout = 300000;
bool APtimeout_done = false;
bool networkrestart = false;
long AP_STA_timer = 0;




///////////////////////////////////////////////////////////////////////////////////////////
//
//                                            MQTT  
//
//////////////////////////////////////////////////////////////////////////////////////////

char mqttesptopic[] = "esp";  //Required GLOBAL ESP subscription !
const int MQTTtimeout = 600000;
bool mqttreload = false;
IPAddress MQTTserver(0,0,0,0);
bool MQTT_enabled = false;



///////////////////////////////////////////////////////////////////////////////////////////
//
//                                            EEPROM  
//
//////////////////////////////////////////////////////////////////////////////////////////

bool EEPROM_commit_var = false; 


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

const uint16_t START_address_settings = 160; // Start EEPROM address for Plugin



bool DEBUG = false;



const uint8_t numberofbaudrates(6); 
const long baudrates[numberofbaudrates] = {9600,115200,256000,460800,921600,2000000};
uint8_t currentspeed = 2; 
long serialspeed = 115200; 



///////////////////////////////////////////////////////////////////////////////////////////
//
//                                            Plugin Variables  
//
//////////////////////////////////////////////////////////////////////////////////////////



enum operatingState { 
OFF = 0,                     // 0
COLOR,                       // 1
RANDOM,                      // 2
RAINBOW,                     // 3
RAINBOWCYCLE,                // 4
RANDOM_TOP_BOTTOM,           // 5
RANDOM_TOP_BOTTOM_LINEAR,    // 6
SQUARES2,                    // 7
SQUARESRANDOM,               // 8
ADALIGHT,                    // 9
UDP,                         // 10
DMX,                         // 11
SNAKES,                      // 12
SNAKES_OVERLAP,               // 13 
STROBE, 
ChaseRainbow, 
FADE, 
LOOPAROUND,
PICKRANDOM,
FADEINFADEOUT,
COOLBLOBS,
SPIRAL,
TEST4,
SQUARES,
EQ1,
RANDOMFUNC, 
ARTNET,  
SINGLE_COLOUR_FADE,
RANDOM_COLOUR_FADE, 
HSICYCLE, 
NEWANIMATIONS, 
};


operatingState opState = OFF;
operatingState LastOpState = OFF;
operatingState HoldingOpState = OFF; 

static const char *MODE_STRING[] = {
"Off", 
"Color", 
"Random-Colour", 
"Rainbow", 
"Rainbow-Cycle",
"HSV-Top-Bottom", 
"Linear-Top-Bottom", 
"Squares2",
"Squares-Random",
"AdaLight", 
"UDP", 
"DMX",
"Snakes", 
"Snakes-overlap",
"Strobe", 
"Fade", 
"Looparound", 
"Pick-Random", 
"Fadein-FadeOut", 
"Cool-Blobs", 
"Spiral",
"Test4",
"Squares",
"EQ1",
"Random-Function", 
"Art-Net", 
"Single-Colour-Fade", 
"Random-Colour-Fade", 
"HSIcycle", 
"New-Animations", 
};
const uint8_t numberofmodes(15); /////// DONT FORGET THIS....


static const char *PALETTE_STRING[] = {
"ALL", "Complementary", "Monochromatic", "Analogous", "Split-Complements", 
"Triadic", "Tetradic", "Even-Spread", "Wheel"
};
const uint8_t numberofpalettes(9); /////// DONT FORGET THIS....




uint16_t Pixel_Update_Freq = 0; 
uint32_t lasteffectupdate = 0; 
uint32_t power = 0; 

uint8_t current_loaded_preset = 0; 
bool current_loaded_preset_changed = false; 


uint16_t pixelCount = 40;
uint8_t pixelPIN = 2;

bool paused = false; 
bool LED_Settings_Changed = false; 
bool Effect_Refresh = false; 
uint8_t timer_effect_tick_timeout = 10; // controls the flow of effect generation... put to 0 by UDP and ADAlight...
bool Enable_Animations = false; 


String WebRGBcolour = "000000"; // This is for the WEBPAGE... takes the value when colour is changed...
Palette Palette_Choice;  //   1
effectState Current_Effect_State = PRE_EFFECT; 
uint16_t effectPosition = 0;

struct WS2812Settings_t {
   uint8_t  SavedOpState;
   uint16_t Timer;
   uint16_t Animationspeed; 
   uint8_t  Brightness;
   RgbColor Color;
   uint8_t  Palette_Choice;  //   1
   uint8_t  Palette_Range;   //   2
   uint8_t  Palette_Number;  //   3
   bool     Random;          //   4
   uint8_t  Time_Stretch;    //   5 
   uint16_t Total_X;         //   6
   uint8_t  Effect_Count;    //   7
   uint8_t  Effect_Min_Size; //   8
   uint8_t  Effect_Max_Size; //   9
   uint8_t  Effect_Option;   //   10
   uint8_t  CheckSum;        //
} WS2812_Settings;

static const char *VAR_STRING[] = {
"Palette             ", // var 1
"Palette Range       ", // var 2
"Number Of Colours   ", // var 3
"Random Colour       ", // var 4
"Effect Time Stretch ", // var 5
"Total X             ", // var 6
"Active Effects      ", // var 7
"Min Size Of Effect  ", // var 8
"Max Size Of Effect  ", // var 9
"Effect Option       "};// var 10


#ifdef GAMMA_CORRECTION
const uint8_t GAMMA_2811[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
    6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
    11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
    19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
    29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
    40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
    90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
    111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
    135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
    162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
    191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
    222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255
};
#endif






