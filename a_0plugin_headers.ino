#define WS2812_DEVICE


#include <PID_v1.h>

#include <DHT.h>

#ifdef WS2812_DEVICE
	#include <NeoPixelBus.h>
	#include <RgbColor.h>
#endif
