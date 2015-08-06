
void cache Squares2 (uint8_t mode) { // WORKING RANDOM SQUARE SIZES...

  uint16_t x,y, total_y;
  uint16_t total_x = var7; 
  uint8_t  square_size =  var10;
  uint8_t  numberofpoints = var8; // default 5, if it = 10, then its random......
  uint8_t  effect_option = var6;
  uint8_t  dimoffset ; 
  int      pixel; // has to carry negative value now. 
  uint32_t timeforsequence; 
  bool     coordinates_OK = false;
  uint16_t x_rand,y_rand;
  RgbColor color;
// these are defaults... 
  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 1;
  if (total_x == 0) total_x = 13; 

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

    uint32_t temp_lower_boundary  ;
    uint32_t temp_upper_boundary  ;

  switch(Current_Effect_State) {

 

  case PRE_EFFECT:

    effectPosition = 0;   

     //if (effect_option == 1) fade_to(RgbColor(0,0,0), RGB); 
     if (effect_option == 1 ) {
      animator->FadeTo(1000, RgbColor(0,0,0)); // a timer for this should not be necessary as the RUN effect waits for animations to stop running..
     }

     
     if (effect_option == 0) { 
          dimoffset = CurrentBrightness / 5;
          RgbColor Top =     dim ( Wheel (  random(0,255)  ), (uint8_t)dimoffset );
          RgbColor Bottom =  dim ( Wheel (  random(0,255)  ), (uint8_t)dimoffset ) ;        
          top_bottom_fade(  Top , Bottom , total_x, CurrentAnimationSpeed); 
    }

    lasteffectupdate = millis(); 
    Debugln("Squares 2 Running");
    
    temp_lower_boundary = CurrentAnimationSpeed * IntervalMultiplier * 1 ;
    temp_upper_boundary = CurrentAnimationSpeed * IntervalMultiplier * 10 ;
    
    if (temp_upper_boundary > 65000) temp_upper_boundary = 65000; 

    Debugf("Min time = %u \n", temp_lower_boundary);
    Debugf("Max time = %u \n", temp_upper_boundary);

    Pre_effect();  // PRE effect SETS LAST EFFECT UPDATE TO ZERO... ? is this requires?

    break;
  case RUN_EFFECT:  

      if ( lasteffectupdate == 0 ) { // This allows a refresh, or brightness change etc...  to re-set up the effect..
        Current_Effect_State = PRE_EFFECT;
        break; 
      }


     // if  ( (millis() - lasteffectupdate > ( WS2812interval * IntervalMultiplier ) ) && effectPosition < numberofpoints ) {      
      if  ( effectPosition < numberofpoints ) {
//      espcyclecount = ESP.getCycleCount(); 
       color = dim(Wheel(random(255))); // RgbColor(random(255),random(255),random(255));
       
        //Serial.print("Colour chosen: ");
        //Serial.println(numbercalled++); 
      //if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(2,7);
      
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

      uint32_t lower_boundary = CurrentAnimationSpeed * IntervalMultiplier * 1 ;
      uint32_t upper_boundary = CurrentAnimationSpeed * IntervalMultiplier * 100 ;

      upper_boundary = constrain(upper_boundary, lower_boundary ,65000); // can get rid of this when animation scaling 
      timeforsequence = random(lower_boundary,upper_boundary);
      Debugf("Time = %u \n",timeforsequence);
      //timeforsequence = random(( CurrentAnimationSpeed * IntervalMultiplier * 10), (CurrentAnimationSpeed * 1000 * IntervalMultiplier)); //generate same time for each object

    for (uint16_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++)
        {
            pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x );    
            if (sq_pixel == 0) effectPosition++; // adds to running count. 

            if (pixel >= 0) {
     
//            if (sq_pixel > 0) Debug(",");
//            Debugf("%3u", pixel); 

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