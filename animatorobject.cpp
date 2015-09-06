//  







#include "animatorobject.h"


AnimatedObject::AnimatedObject(NeoPixelBus* bus, NeoPixelAnimator* animator, uint8_t n):  
	_bus(bus), 
	_animator(animator),
	_ObjectCount(n)
{

    _objects= new ObjectContext[n];

}

AnimatedObject::~AnimatedObject()
{
    _bus = NULL;
    _animator = NULL; 

    if (_objects)
    {
        delete[] _objects;
        _objects = NULL;
    }
}




bool AnimatedObject::Add(ObjectCallback Objectupdate, uint8_t x, uint8_t y, uint32_t effectvar) {

  uint8_t n; //  = 0; 

  if (_ActiveObjectsCount == _ObjectCount) return false; 

  for (n = 0; n < _ObjectCount; n++ ) {
  	if ( ! _objects[n].isRunning ) break; 
  }

//   do { n++ ;}  while (_objects[n].isRunning); 

    _objects[n].fnUpdate = Objectupdate;
    _objects[n].isRunning = true; 
    _objects[n].x = x;
    _objects[n].y = y; 
    _objects[n].effectvar = effectvar; 
    //Serial.printf("is Running = %u \n", _objects[n].isRunning ) ;
    _ActiveObjectsCount++;
    return true; // returns 0 if failed, 1 = slot 1, 2 = slot 2 etc.....  

}


void AnimatedObject::Stop(uint8_t n) {
  _ActiveObjectsCount--;
  _objects[n].isRunning = false; 

} 

void AnimatedObject::UpdateAll() {
			//Serial.print("Update Called: ");
            ObjectContext* pObject;
			if (!_ActiveObjectsCount) return; 

			for (uint8_t n = 0; n < _ObjectCount; n++ )  {					
				    pObject = &_objects[n]; 
					if (pObject->isRunning) pObject->fnUpdate(pObject->x, pObject->y, pObject->effectvar);
			}

			
			//Serial.println(); 

}

// void AnimatedObject::UpdateOne(uint8_t n) {
// 		//static uint8_t n = 0;
// //		if (!_ActiveObjectsCount) return; 
// 		// uint8_t test = 0; 
// 		// ObjectContext* pObject;

// 		// do { 
// 		// 	test = n; 
// 		// 	pObject = &_objects[n++]; 
// 		// } while (!pObject->isRunning && n < _ObjectCount) ; 

// 		// if (n == _ObjectCount) n = 0; 

// 		ObjectContext* pObject;

// 		pObject = &_objects[n];

// 		if (pObject->isRunning) { 
// 			pObject->fnUpdate(pObject->x, pObject->y, pObject->effectvar);
// 			//Serial.printf("%u,",n);
// 		}


// 	//	if (n++ == _ObjectCount) n = 0; 

// 	//	Serial.print("Updated: ");
// 	//	Serial.print(test);

// }

void AnimatedObject::UpdateAsync() {
		static uint8_t n = 0; 
		ObjectContext* pObject;

		if (!_ActiveObjectsCount) return; 

		 do { 
		 	pObject = &_objects[n++]; 
		 } while (!pObject->isRunning && n < _ObjectCount) ; 

		 if (n == _ObjectCount) n = 0; 

		if (pObject->isRunning) pObject->fnUpdate(pObject->x, pObject->y, pObject->effectvar);

}


void AnimatedObject::Begin()
 
 {


}