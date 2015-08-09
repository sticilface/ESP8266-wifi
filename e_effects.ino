// static const char *VAR_STRING[] = {
// "Palette           ", // var 1
// "Palette Range     ", // var 2
// "Number Of Colours ", // var 3
// "Var4              ", // var 4
// "IntervalMultiplier", // var 5
// "Effect Option     ", // var 6
// "Total_X           ", // var 7
// "Number of effects ", // var 8
// "Var9              ", // var 9
// "Size of effect    "};// var 10


void cache Squares2 (uint8_t mode) { // WORKING RANDOM SQUARE SIZES...

  uint16_t x,y, total_y;
  uint16_t total_x = WS2812_Settings.Total_X; 
  uint8_t  square_size =  WS2812_Settings.Effect_Max_Size;
  uint8_t  numberofpoints = WS2812_Settings.Effect_Count; // default 5, if it = 10, then its random......
  uint8_t  effect_option = WS2812_Settings.Effect_Option;
  uint8_t  Palette_Range = WS2812_Settings.Palette_Range; 
  uint8_t  Number_of_colours = WS2812_Settings.Palette_Number; 
  uint8_t  dimoffset ; 
  int      pixel; // has to carry negative value now. 
  uint32_t timeforsequence; 
  bool     coordinates_OK = false;
  uint16_t x_rand,y_rand;
  static uint32_t counter, effect_timer; 
  static uint16_t position = 0; 
   RgbColor color;
   static uint8_t static_colour; 
// these are defaults... 
  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 1;
  if (total_x == 0) total_x = 13; 
  if (Palette_Range == 0) Palette_Range = 10;
  if (Number_of_colours == 0 ) Number_of_colours = 10; // set the default numbers of colours in palette. 
  total_y = return_total_y(total_x); 

      //Serial.println("Generating coordinates.");
   //          uint8_t count = 0; 
   //   static uint32_t runningcount = 0; 
   //   static int numbercalled = 0;
   //   static bool temp_bug_track2 = false; 
   //   static uint32_t espcyclecount = 0 ; 
   //   static uint32_t temp_timer = 0; 
   //   static bool pause_and_reboot_effect = false; 
   //          uint32_t now = millis(); 

     uint32_t  lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     uint32_t  upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );


  switch(Current_Effect_State) {

  case PRE_EFFECT:

    effectPosition = 0;   

     //if (effect_option == 1) fade_to(RgbColor(0,0,0), RGB); 
     if (effect_option > 0) {
      animator->FadeTo(1000, RgbColor(0,0,0)); // a timer for this should not be necessary as the RUN effect waits for animations to stop running..
     }

     
    //  if (effect_option == 0) { 
    //       dimoffset = WS2812_Settings.Brightness / 5;
    //       RgbColor Top =     dim ( Wheel (  random(255)  ), (uint8_t)dimoffset );
    //       RgbColor Bottom =  dim ( Wheel (  random(255)  ), (uint8_t)dimoffset ) ;        
    //       top_bottom_fade(  Top , Bottom , total_x, WS2812_Settings.Timer * 32); 
    // }

    Debugln("Squares 2 Running");
    
     //  lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     //  upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
    
    //if (temp_upper_boundary > 65000) temp_upper_boundary = 65000; 

    Debugf("Min time = %u \n", lower_boundary);
    Debugf("Max time = %u \n", upper_boundary);

    Pre_effect();  // PRE effect SETS LAST EFFECT UPDATE TO ZERO... ? is this requires?
    lasteffectupdate = millis();  // this has to go here otherwise the pre_effect routine restarts it...

    break;


  case RUN_EFFECT:  

    //  if ( Effect_Refresh == true ) { // This allows a refresh, or brightness change etc...  to re-set up the effect..
    //    Current_Effect_State = PRE_EFFECT;
    //    Effect_Refresh == false; 
    //    break; 
    //  }

      //if (Effect_Refresh == true)

     // if  ( (millis() - lasteffectupdate > ( WS2812interval * IntervalMultiplier ) ) && effectPosition < numberofpoints ) {      
      if  ( effectPosition < numberofpoints ) {
//      espcyclecount = ESP.getCycleCount(); 

       //if (effect_option == 1 ) {

        //Serial.print("Colour chosen: ");
        //Serial.println(numbercalled++); 
      //if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(WS2812_Settings.Effect_Min_Size  ,  WS2812_Settings.Effect_Max_Size);
     // checks to see if it is a linear string or not... if less then it is equal to 0 ...
     if (total_x > square_size) { 
           x_rand = random(0, total_x - square_size) ; 
     } else {
           x_rand = 0; 
     };

     if (total_y > square_size) {
           y_rand = random(0, total_y - square_size) ;
       } else {
           y_rand = 0; 
     }

      coordinates_OK = false; // reset the coordinates OK...

      for (uint16_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++) {

          pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x ); 

          if (pixel >= 0) { 
                if (animator->IsAnimating(pixel)) { coordinates_OK = false ;  break; }; 
                          }
            if (sq_pixel == (square_size * square_size)-1) { coordinates_OK = true ; }; 
          }

    if (coordinates_OK) {
//Debug("\n1, "); 

      counter++;  // Is in use yet.... 
      uint16_t lower_boundary_2000 = constrain (WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 5 ), 2, 2000);
      uint16_t upper_boundary_2000 = constrain (WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 5 ), 1, 2000);

       lower_boundary = map ( lower_boundary_2000 , 1, 2000, 500 , 65000 );
       upper_boundary = map ( upper_boundary_2000 , 1, 2000, 500 , 65000 );
       timeforsequence = random(lower_boundary, upper_boundary);

////////////////
//
//
//        Colour generation.  If you want random colour... pick a random colour when 10 times max animation time
//        None random = picking random colour from palette based on CHOSEN COLOUR...           
//
/////////////////          
//Debug("2, "); 
        yield();
              if (WS2812_Settings.Random == true ) {
                if (millis() - effect_timer > upper_boundary * 10 ) { static_colour = random(255); effect_timer = millis() ; } ; 
                color = Return_Palette(Wheel(static_colour), counter) ;
              } else {
                //Debug("3, "); 
                color = Return_Palette(WS2812_Settings.Color, counter) ;
                //Debug("4, "); 
              }

          


      color = dim(color);

          // if (effect_option == 3 ) {
          //   if (counter % numberofpoints == 0) { static_colour = random(255); position = 0 ;} ; 
          //   //if (position > numberofpoints) position = 0; 
          //   color = Return_Analogous(Wheel(static_colour), position++, numberofpoints , range) ;
          //   color = dim(color);
          //   } else if (effect_option == 4) { 
          //   color = dim(color);               
          //   } else { 
          //   color = dim(Wheel(random(255))); // RgbColor(random(255),random(255),random(255));
          //   }
       

       //lower_boundary = WS2812_Settings.Timer * IntervalMultiplier * 1 ;
       //upper_boundary = WS2812_Settings.Timer * IntervalMultiplier * 100 ;
       




      //upper_boundary = constrain(upper_boundary, lower_boundary ,65000); // can get rid of this when animation scaling 
      
      //Debugf("Time = %u \n", timeforsequence);
      //timeforsequence = random(( CurrentAnimationSpeed * IntervalMultiplier * 10), (CurrentAnimationSpeed * 1000 * IntervalMultiplier)); //generate same time for each object
//Debug("5, "); 

    for (uint16_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++)
        {
            pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x );    

            if (sq_pixel == 0) effectPosition++; // adds to running count. 

            if (pixel >= 0) {

            RgbColor originalColor = strip->GetPixelColor(pixel);
          
            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                //if (sq_pixel == 0 && progress == 0.0) effectPosition++; // adds to running count. 
                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, color, new_progress);
                strip->SetPixelColor(pixel, updatedColor);
                if (sq_pixel == 0 && progress == 1.0) effectPosition--; 
            };

            animator->StartAnimation(pixel, timeforsequence , animUpdate); // might change this to be a random variant...
          } // end of if for is pixel valid... >=0
          yield();
        }

//Debug("6"); 

        lasteffectupdate = millis(); // NOT really needed.....

          }
        } else {break; };
    break;

    case POST_EFFECT:

    Post_effect(); 
    Debugln("Square2 Ended");
    break;
  }

} 
// end of Squares2


void cache Effect_Top_Bottom(EffectSetting Setting, BlendMethod Method) { 
  uint16_t total_x; 
  static uint32_t random_time = 0 ; 
  //float Ceiling, Floor; 
  RgbColor colour_top, colour_bottom; 

     uint32_t  lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     uint32_t  upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     
     uint16_t  animation_time ; // = random_time / 10; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:
    Pre_effect(); 
    Debugln("Top Bottom PRE...");
    lasteffectupdate = 0; 
    break;


    case RUN_EFFECT: // 
  

      if (  
            ( 
              ( millis() - lasteffectupdate > random_time ) ||  ( Effect_Refresh == true )
            ) 
                &&  (animator->IsAnimating() == false ) 
          ) 

          { // only generate new effect if NOT blending..
       


   //       if (WS2812_Settings.var1 == 0) { Ceiling = 0.2f; } else { Ceiling = (float)WS2812_Settings.var1 / 255.0f ; }; 
   //       if (WS2812_Settings.var2 == 0) { Floor = -0.2f; } else { Floor = ((float)WS2812_Settings.var2 / 255.0f) * -1 ; } ;
          if (WS2812_Settings.Total_X == 0 && pixelCount > 10) { 
            total_x = 13; 
          } else if (WS2812_Settings.Total_X == 0 && pixelCount < 10) {
            total_x = 1; 
          } else { 
            total_x = WS2812_Settings.Total_X; 
          }; 


///////// ----   Generate Top Colours

        if (WS2812_Settings.Random == true)  {
                colour_top  = Wheel(random(0,255)) ; //  RgbColor(255,0,0); 
              } else {
                colour_top  = WS2812_Settings.Color ; //  RgbColor(255,0,0); 
              }

///////// ----   Generate Bottom Colours

                colour_bottom = Return_Palette (colour_top);
                colour_top = dim(colour_top);
                colour_bottom = dim(colour_bottom); 

/////// --- Timing

                lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
                upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
                random_time = random( lower_boundary, upper_boundary ); // generate length of animation
    //Debugf("Time = %u \n", random_time);

                animation_time = random_time / 10; 

      top_bottom_fade(colour_top,colour_bottom, WS2812_Settings.Total_X, animation_time, Method); 
      
      lasteffectupdate = millis(); 

    } // end of if that generates effect. 

///////////////////////////////////////////////////////
    break;

    case POST_EFFECT:
    Post_effect(); 
        //Serial.println("Top Bottom POST...");
    break;
  }

} // end of actual function.....



void cache StripOFF() {

  switch(Current_Effect_State) {

    case PRE_EFFECT:
    Pre_effect(); 

        for (uint16_t n = 0; n < strip->PixelCount(); n++)
            {
              HslColor original = strip->GetPixelColor(n);

        AnimUpdateCallback animUpdate = [=](float progress)
        {
                  float new_lightness = original.L - (original.L  * progress) ;
                  strip->SetPixelColor(n, HslColor(original.H,original.S,new_lightness));
        };

        animator->StartAnimation(n, WS2812_Settings.Timer, animUpdate);
    }

    break;

    case RUN_EFFECT:

    break;

    case POST_EFFECT:
    Post_effect(); 
    break;
  }

}

//void cache SetRGBcolour (RgbColor value) {

//SetRGBcolour(value, WS2812_Settings.Timer); 

//}

void cache RGBcolour () {


    //long  now1 = millis(); 
    //long interval = now1 - lasteffectupdate; 
    
  switch(Current_Effect_State) {
    
    case PRE_EFFECT:
    Debugln("Effect set to Color"); 
    Pre_effect(); 
    animator->FadeTo(WS2812_Settings.Timer, dim(WS2812_Settings.Color)); 
    break;

    case RUN_EFFECT:

    if (Effect_Refresh == true) { 
      Debugln("Effect Refresh Called"); 
      Current_Effect_State = PRE_EFFECT;
      Effect_Refresh = false;
    }


    break;
    case POST_EFFECT:
        Debugln("Color END"); 
        Post_effect(); 
        break;
  }
}




