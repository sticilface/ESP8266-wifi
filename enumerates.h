enum BlendMethod { HSL = 0, RGB };

enum EffectSetting { NORMAL = 0, SINGLECOLOUR, TWOCOLOUR , RANDOMSINGLECOLOUR};

enum Palette { ALL = 0, COMPLEMENTARY, MONOCHROMATIC, ANALOGOUS, SPLITCOMPLEMENTS, TRIADIC, TETRADIC, MULTI}; 

enum Colour { RANDOM_COLOUR = 0, SPECIFIC_COLOUR }; 

enum effectState { PRE_EFFECT = 0, RUN_EFFECT, POST_EFFECT, EFFECT_PAUSED};

struct X_Y_Coordinates {
  uint16_t X;
  uint16_t Y;
};
