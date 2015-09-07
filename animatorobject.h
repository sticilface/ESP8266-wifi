//  This is a protype animator object class



#include <Arduino.h>
#include <NeoPixelBus.h>
#include <functional>


class NeoPixelBus;
class NeoPixelAnimator;

//typedef std::function<void(uint8_t &x, uint8_t &y, uint32_t &effectvar)> ObjectCallback;
typedef std::function<void()> ObjectCallback;


class AnimatedObject {
public:
	    // Constructor: number of LEDs, pin number, LED type
    AnimatedObject(NeoPixelBus* bus, NeoPixelAnimator* animator, uint8_t n);
    ~AnimatedObject();
    void Begin(); 
    //bool Add(ObjectCallback Objectupdate, uint8_t x, uint8_t y, uint32_t effectvar); 
    bool Add(ObjectCallback Objectupdate); 

    void Stop(uint8_t n); 
    uint8_t Status() {
  		return (_ObjectCount);
    }; 

    void UpdateAll();  // updates all the effects.. 
    void UpdateAsync();  // updates one at a time, works better when called frequenctly 





private:

NeoPixelBus* _bus; 
NeoPixelAnimator* _animator; 
uint8_t _ObjectCount; 
uint8_t _ActiveObjectsCount; 


    struct ObjectContext
    {
        ObjectContext() :
   //         x(0),
   //         y(0),
            isRunning(false),
   //         effectvar(0), 
            fnUpdate(NULL)
        {}

 //       uint8_t x;
 //       uint8_t y;
        bool isRunning; 
 //       uint32_t effectvar; 
        ObjectCallback fnUpdate;

    };

    ObjectContext* _objects;

};

