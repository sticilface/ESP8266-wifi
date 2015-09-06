//  This is a protype animator object class



#include <Arduino.h>
#include <NeoPixelBus.h>
#include <functional>


class NeoPixelBus;
class NeoPixelAnimator;

typedef std::function<void(void)> ObjectCallback;

class AnimatedObject {
public:
	    // Constructor: number of LEDs, pin number, LED type
    AnimatedObject(NeoPixelBus* bus, NeoPixelAnimator* animator, uint8_t n);
    ~AnimatedObject();
    void Begin(); 
    uint8_t Add(ObjectCallback Objectupdate); 
    void Stop(uint8_t n); 
    uint8_t Status() {
  		return (_ObjectCount);
    }; 

    void UpdateAll(); 




private:

NeoPixelBus* _bus; 
NeoPixelAnimator* _animator; 
uint8_t _ObjectCount; 
uint8_t _ActiveObjectsCount; 


    struct ObjectContext
    {
        ObjectContext() :
            x(0),
            y(0),
            isRunning(false),
            fnUpdate(NULL)
        {}

        uint16_t x;
        uint16_t y;
        bool isRunning; 
        ObjectCallback fnUpdate;

    };

    ObjectContext* _objects;

};

