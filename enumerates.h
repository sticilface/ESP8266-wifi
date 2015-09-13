#ifndef ENUMERATES_H
#define ENUMERATES_H

enum BlendMethod { HSL = 0, RGB };

enum EffectSetting { NORMAL = 0, SINGLECOLOUR, TWOCOLOUR , RANDOMSINGLECOLOUR};

enum Palette { ALL = 0, COMPLEMENTARY, MONOCHROMATIC, ANALOGOUS, SPLITCOMPLEMENTS, TRIADIC, TETRADIC, MULTI, WHEEL}; 

enum Colour { RANDOM_COLOUR = 0, SPECIFIC_COLOUR }; 

enum effectState { PRE_EFFECT = 0, RUN_EFFECT, POST_EFFECT, EFFECT_PAUSED};

struct XY {

  //XY(uint8_t x, uint8_t y) : x(x), y(y) {};
  //XY(){};
  uint8_t x;
  uint8_t y;

};

struct XY_t {
  uint8_t x;
  uint8_t y;

  XY_t(uint8_t x, uint8_t y) : x(x), y(y) {}
  XY_t(){} 
};


//typePerson(string firstName,string lastName) : firstName(firstName), lastName(lastName) {}




#endif

