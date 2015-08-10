
#define DEBUG_YES


//////////////   NOT IN USE.....
/* 
    // UDP specific
	#define ART_NET_PORT 6454
	// Opcodes
	#define ART_POLL 0x2000
	#define ART_DMX 0x5000
	// Buffers
	#define MAX_BUFFER_ARTNET 530
	// Packet
	#define ART_NET_ID "Art-Net\0"
	#define ART_DMX_START 18
 */



#ifdef DEBUG_YES
#define Debug(x)    Serial.print(x)
#define Debugln(x)  Serial.println(x)
#define Debugf(...) Serial.printf(__VA_ARGS__)
#define Debugflush  Serial.flush
#else
#define Debug(x)    {}
#define Debugln(x)  {}
#define Debugf(...) {}
#define Debugflush  {}
#endif

#define cache  ICACHE_FLASH_ATTR
//#define WIFIOVERRIDE

//#define SQUAREDEBUG
//#define LOOPDEBUG