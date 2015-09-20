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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//                      SQUARES
//  ToDo  Add in three phase effect.... IN, HOLD, OUT... use switch statement inside...animation callback!
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void cache Squares2 (uint8_t mode) { // WORKING RANDOM SQUARE SIZES...

  uint16_t x,y, total_y;
  uint16_t total_x = WS2812_Settings.Total_X; 
  uint8_t  square_size =  WS2812_Settings.Effect_Max_Size;
  uint8_t  numberofpoints = WS2812_Settings.Effect_Count; // default 5, if it = 10, then its random......
  uint8_t  effect_option = WS2812_Settings.Effect_Option;
  uint8_t  Palette_Range = WS2812_Settings.Palette_Range; 
  uint8_t  Number_of_colours = WS2812_Settings.Palette_Number; 
  uint8_t  dimoffset ; 
  int16_t pixel; // has to carry negative value now. 
  uint32_t timeforsequence; 
  bool     coordinates_OK = false;
  uint16_t x_rand,y_rand;
  uint16_t lower_boundary_2000, upper_boundary_2000 ; 
  uint32_t lower_boundary, upper_boundary ; 

  static uint32_t counter, effect_timer; 
  //static uint16_t position = 0; 
   RgbColor color;
   static uint8_t static_colour; 
// these are defaults... 
  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 1;
  if (total_x == 0) total_x = 13; 
  if (Palette_Range == 0) Palette_Range = 10;
  if (Number_of_colours == 0 ) Number_of_colours = 10; // set the default numbers of colours in palette. 
  total_y = return_total_y(total_x); 
  
     //uint32_t  lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     //uint32_t  upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );

  switch(Current_Effect_State) {

  case PRE_EFFECT:

    effectPosition = 0;   
    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break ; } //  DO NOT RUN IF ANIMATIONS DISABLED
     //if (effect_option == 1) fade_to(RgbColor(0,0,0), RGB); 
    
    // if (effect_option > 0) {
     // animator->FadeTo(1000, RgbColor(0,0,0)); // a timer for this should not be necessary as the RUN effect waits for animations to stop running..
    // }

      animator->FadeTo(1000,RgbColor(0,0,0)); 

      // for (uint16_t pixel = 0; pixel < pixelCount; pixel++) {
      // RgbColor originalColor = strip->GetPixelColor(pixel); 
      // AnimUpdateCallback animUpdate = [=](float progress)
      //       {
      //           RgbColor updatedColor = RgbColor::LinearBlend(originalColor, RgbColor(0,0,0), progress);
      //           strip->SetPixelColor(pixel, updatedColor);
      //       };

      //       animator->StartAnimation(pixel, 1000 , animUpdate); // might change this to be a random variant...

      //   }

       
       lower_boundary_2000 = constrain (WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 3 ), 1, 2000);
       upper_boundary_2000 = constrain (WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 3 ), 1, 2000);

       lower_boundary = map ( lower_boundary_2000 , 1, 2000, 50 , 65000 );
       upper_boundary = map ( upper_boundary_2000 , 1, 2000, 50 , 65000 );

    Debugln("Squares 2 Running");
    Debugf("Min time = %u \n", lower_boundary);
    Debugf("Max time = %u \n", upper_boundary);
    Debugf("Total effects = %u \n", numberofpoints); 

    Pre_effect();  // PRE effect SETS LAST EFFECT UPDATE TO ZERO... ? is this requires?
    lasteffectupdate = 0; // millis();  // this has to go here otherwise the pre_effect routine restarts it...

    break;


  case RUN_EFFECT:  

      if ( Effect_Refresh == true ) { // This allows a refresh, or brightness change etc...  to re-set up the effect..
        Current_Effect_State = PRE_EFFECT;
        Effect_Refresh = false; 
        Debugln("Effect Refresh called");
        break; 
      }

      //if (Effect_Refresh == true)
     //Debugf("\n effectposit = %u, numberofpoints = %u, effectstate = %u : ", effectPosition,numberofpoints,(uint8_t)Current_Effect_State );
       //uint16_t effect_timeout = ;

     if  ( (millis() - lasteffectupdate > WS2812_Settings.Timer ) && effectPosition < numberofpoints ) {   //   This staggers the effects...
    ///  if  ( effectPosition < numberofpoints ) {
//      espcyclecount = ESP.getCycleCount(); 

       //if (effect_option == 1 ) {
    //Debug(".");
        //Serial.print("Colour chosen: ");
        //Serial.println(numbercalled++); 
      //if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(WS2812_Settings.Effect_Min_Size  ,  WS2812_Settings.Effect_Max_Size + 1 );
     // checks to see if it is a linear string or not... if less then it is equal to 0 ...
     
     if (total_x > square_size) { 
           x_rand = random(0, total_x - square_size + 1) ; 
     } else {
           x_rand = 0; 
     };

     if (total_y > square_size) {
           y_rand = random(0, total_y - square_size + 1) ;
       } else {
           y_rand = 0; 
     }

      coordinates_OK = false; // reset the coordinates OK...

      for (uint16_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++) {

          pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x ); 

          if (pixel > -1) { 
                if (animator->IsAnimating(pixel)) { coordinates_OK = false ;  break; }; 
                          }
            if (sq_pixel == (square_size * square_size) - 1 ) { coordinates_OK = true ; }; 
          }

    if (coordinates_OK) {
//Debug("\n1, "); 
      counter++; 
      counter = counter % numberofpoints; //  counter++;  // Is in use

      //Debugf("Counter = %u, effectposition = %u, (%u,%u) \n", counter, effectPosition, x_rand,y_rand);

   //   uint16_t lower_boundary_2000 = constrain (WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 3 ), 2, 2000);
   //   uint16_t upper_boundary_2000 = constrain (WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 3 ), 1, 2000);

   //    lower_boundary = map ( lower_boundary_2000 , 1, 2000, 50 , 65000 );
   //    upper_boundary = map ( upper_boundary_2000 , 1, 2000, 50 , 65000 );

       lower_boundary_2000 = constrain (WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 3 ), 1, 2000);
       upper_boundary_2000 = constrain (WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 3 ), 1, 2000);

       lower_boundary = map ( lower_boundary_2000 , 1, 2000, 50 , 65000 );
       upper_boundary = map ( upper_boundary_2000 , 1, 2000, 50 , 65000 );

       timeforsequence = random(lower_boundary, upper_boundary);

////////////////
//
//
//        Colour generation.  If you want random colour... pick a random colour when 10 times max animation time
//        None random = picking random colour from palette based on CHOSEN COLOUR...           
//
/////////////////          
//Debug("2, "); 
        //yield();
              if (WS2812_Settings.Random == true ) {
                if (millis() - effect_timer > upper_boundary * 10 ) { static_colour = random(255); effect_timer = millis() ; } ; 
                color = Return_Palette(Wheel(static_colour), counter) ;
              } else {
           //     Debug("3, "); 
                color = Return_Palette(WS2812_Settings.Color, counter) ;
             //   Debug("4, "); 
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
            //if (sq_pixel > 0) Debug(", ");
            //Debug(pixel);
            //Serial.print(",");

            if (sq_pixel == 0) effectPosition++; // adds to running count. 

            if (pixel > -1) {
                //pixel -= 1;   //*****       SUBTRACT ONE TO GET TRUE PIXEL       **** 

                RgbColor originalColor = strip->GetPixelColor( pixel  ); // subtract one... to get true pixel... 
          
            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                //if (sq_pixel == 0 && progress == 0.0) effectPosition++; // adds to running count. 
                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, color, new_progress);
                strip->SetPixelColor(pixel, updatedColor);
                if (sq_pixel == 0 && progress == 1.0 && effectPosition > 0) effectPosition--; 
            };

            animator->StartAnimation(pixel, timeforsequence , animUpdate); // might change this to be a random variant...
          } // end of if for is pixel valid... >=0
          //yield();
        }

     //    Debug("6");

        lasteffectupdate = millis(); // NOT really needed.....

          }
        };
    break;

    case POST_EFFECT:

    Post_effect(); 
    Debugln("Square2 Ended");
    break;
  }

} 
// end of Squares2


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        Effect_Top_Bottom
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void cache Effect_Top_Bottom(EffectSetting Setting, BlendMethod Method) { 
  uint16_t total_x; 
  static uint32_t random_time = 0 ; 

  RgbColor colour_top, colour_bottom; 

     uint32_t  lower_boundary; //  = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     uint32_t  upper_boundary; //  = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 1 , 65000 );
     
     uint16_t  animation_time; // = random_time / 10; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

    Pre_effect(); 
    Debugln("Top Bottom PRE...");
    lasteffectupdate = 0; 
    break;


    case RUN_EFFECT: // 
  

      if (  
              ( millis() - lasteffectupdate > random_time  && animator->IsAnimating() == false ) ||  Effect_Refresh 
         ) 

          { // only generate new effect if NOT blending..
       
            Effect_Refresh = false; // this stops the effect from looping constantly...

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
                colour_top  = Wheel(random(255)) ; //  RgbColor(255,0,0); 
              } else {
                colour_top  = WS2812_Settings.Color ; //  RgbColor(255,0,0); 
              }

///////// ----   Generate Bottom Colours

                colour_bottom = Return_Palette (colour_top, 1);

                colour_top =    dim(colour_top);
                colour_bottom = dim(colour_bottom); 

/////// --- Timing

                lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 10000 , 600000 );
                upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 10000 , 600000 );
                
                random_time = random( lower_boundary, upper_boundary ); // generate length of animation pause
    //Debugf("Time = %u \n", random_time);


                lower_boundary = map ( WS2812_Settings.Timer  - ( WS2812_Settings.Timer / 20 ), 1, 2000, 500 , 65000 );
                upper_boundary = map ( WS2812_Settings.Timer  + ( WS2812_Settings.Timer / 20 ), 1, 2000, 500 , 65000 );

                animation_time = (uint16_t) random( lower_boundary, upper_boundary ); // generate length of animation


                top_bottom_fade(colour_top, colour_bottom, WS2812_Settings.Total_X, animation_time, Method); 
      
                lasteffectupdate = millis(); 

    } // end of if that generates effect. 

    break;

    case POST_EFFECT:
    Post_effect(); 
        //Serial.println("Top Bottom POST...");
    break;
  }

} // end of actual function.....



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        STRIP OFF
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void cache StripOFF() {

  switch(Current_Effect_State) {

    case PRE_EFFECT:
    Pre_effect(); 

    if(Enable_Animations) {
        for (uint16_t n = 0; n < strip->PixelCount(); n++)
            {
              HslColor original = strip->GetPixelColor(n);

        AnimUpdateCallback animUpdate = [=](float progress)
        {
                  float new_lightness = original.L - (original.L  * progress) ;
                  strip->SetPixelColor(n, HslColor(original.H,original.S,new_lightness));
        };

        animator->StartAnimation(n, 2000 , animUpdate);
        } 
    } else {
      strip->ClearTo(0,0,0);
    }

    break;

    case RUN_EFFECT:

      if (millis() - lasteffectupdate > 30000 ){
         strip->ClearTo(0,0,0); 
         lasteffectupdate = millis()  ;
      }



    break;

    case POST_EFFECT:
    Post_effect(); 
    break;
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cache RGBcolour () {


    //long  now1 = millis(); 
    //long interval = now1 - lasteffectupdate; 
    
  switch(Current_Effect_State) {
    
    case PRE_EFFECT:

    Debugln("Effect set to Color"); 
    
    Pre_effect(); 

    if (Enable_Animations)  { animator->FadeTo(2000, dim(WS2812_Settings.Color)); }
      else {
        strip->ClearTo(dim(WS2812_Settings.Color)) ; 
      }

    break;

    case RUN_EFFECT:

    if (Effect_Refresh == true) { 
      Debugln("Effect Refresh Called"); 
      Current_Effect_State = PRE_EFFECT;
      Effect_Refresh = false;
    }

      if (millis() - lasteffectupdate > 30000 ){
         strip->ClearTo(dim(WS2812_Settings.Color)); 
         lasteffectupdate = millis()  ;
      }

    break;
    case POST_EFFECT:
        Debugln("Color END"); 
        Post_effect(); 
        break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
// 
//        RAINBOW
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 void cache rainbow() {

  //static int wsPoint =0;


  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

    effectPosition = 0 ;  

      for (uint16_t n = 0; n < strip->PixelCount(); n++)
            {
              RgbColor original = strip->GetPixelColor(n);
              RgbColor newcolor = dim(Wheel( n + effectPosition)); 

        AnimUpdateCallback animUpdate = [=](float progress)
        {
              RgbColor updatedColor = RgbColor::LinearBlend(original, newcolor, progress);
              strip->SetPixelColor(n, updatedColor);
        };
        animator->StartAnimation(n, 2000 , animUpdate);
    }
    
    effectPosition++ ; 

    Debug("Pre effect end") ; 
    Pre_effect(); 

    break;
    case RUN_EFFECT:  

      if (millis() - lasteffectupdate > WS2812_Settings.Timer && (!animator->IsAnimating()) ){

        for(int i=0; i < strip->PixelCount(); i++) {
          strip->SetPixelColor(i, dim(Wheel( i + effectPosition)));
         }
         effectPosition++;
         if (effectPosition == 256) effectPosition = 0; 
         lasteffectupdate = millis()  ;
      }
 
    break;
    case POST_EFFECT:
    Post_effect(); 
    Debug("Rainbow END"); 
    break;

    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void  cache Rainbowcycle() {

  switch(Current_Effect_State) {
    case PRE_EFFECT:

     //   if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

    effectPosition = 0; 
    Pre_effect(); 

    if (Enable_Animations) {
    
    for (uint16_t i = 0; i < pixelCount; i++)
    {
              RgbColor original = strip->GetPixelColor(i);
        
        AnimUpdateCallback animUpdate = [=](float progress)
        {
           RgbColor updatedColor = RgbColor::LinearBlend(original, dim(Wheel(i * 256 / pixelCount + effectPosition)) ,  progress) ;
            strip->SetPixelColor(i, updatedColor);
        };

        animator->StartAnimation(i, 2000, animUpdate);
        lasteffectupdate = millis();  
    effectPosition++; 
    }
   }

    break;
    case RUN_EFFECT:  
    
    if (Enable_Animations) {
      if (animator->IsAnimating()) { break; }  ; //  This line stops the effect from running if it is still in the warm up! 
    }
      if (millis() - lasteffectupdate > WS2812_Settings.Timer) {      

           for(uint16_t i=0; i< pixelCount; i++) {
                strip->SetPixelColor(i, dim(Wheel(i * 256 / pixelCount + effectPosition)));
            }
          if (effectPosition==256*5) effectPosition=0; 
          lasteffectupdate = millis(); 
          effectPosition++;
          }
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
} // END OF RAINBOW CYCLE


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        DMX.... 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void cache DMXfunc () {

int packetSize; 
static uint32_t timeout = 0; 
//TODO: Dynamically allocate seqTracker to support more than 4 universes w/ PIXELS_MAX change
static uint8_t         *seqTracker;    /* Current sequence numbers for each Universe */
static int32_t         lastPacket;     /* Packet timeout tracker */
static uint8_t         ppu, uniTotal, universe, channel_start, uniLast; 
static uint16_t        count, bounds ; 
static uint32_t        *seqError;      /* Sequence error tracking for each universe */
 static      uint32_t timeout_data = 0; 

ppu = 170; 
universe = WS2812_Settings.Effect_Option; 
channel_start = 1;

  switch(Current_Effect_State) {

    case PRE_EFFECT:

      timer_effect_tick_timeout = 0; // Enable RAPID flow.... 
      Debugln("DMX Effect Started");
      if(millis() > 30000) Adalight_Flash(); 



       count = strip->PixelCount() * 3;
       bounds = ppu * 3;
        if (count % bounds) 
            uniLast = universe + count / bounds;
        else 
            uniLast = universe + count / bounds - 1;

        uniTotal = (uniLast + 1) - universe;

        if (seqTracker) free(seqTracker);
        if ((seqTracker = (uint8_t *)malloc(uniTotal)))
        memset(seqTracker, 0x00, uniTotal);

        if (seqError) free(seqError);
        if ((seqError = (uint32_t *)malloc(uniTotal * 4)))
        memset(seqError, 0x00, uniTotal * 4);

      Debugf("Count = %u, bounds = %u, uniLast = %u, uniTotal = %u\n", count, bounds, uniLast, uniTotal);

//  strip = new NeoPixelBus(count, pin);
      //e131 = new 

      e131.begin( E131_MULTICAST , universe ) ; // E131_MULTICAST // universe is optional and only used for Multicast configuration.


  //    e131.begin( E131_MULTICAST , UNIVERSE) ; // E131_MULTICAST // universe is optional and only used for Multicast configuration.

      Pre_effect(); 

    break; 

    case RUN_EFFECT:

 // if(e131.parsePacket()) {
 //        if (e131.universe == WS2812_Settings.Effect_Option) {
 //            for (uint16_t i = 0; i < pixelCount; i++) {
 //                uint16_t j = i * 3 + (CHANNEL_START - 1);
 //                strip->SetPixelColor(i, e131.data[j], e131.data[j+1], e131.data[j+2]);
 //            }
 //            strip->Show();
 //            timeout = millis(); 
 //        }
 //    }


 if(e131.parsePacket()) {
        if ((e131.universe >= universe) && (universe <= uniLast)) {
            /* Universe offset and sequence tracking */
            uint8_t uniOffset = (e131.universe - universe);
            if (e131.packet->sequence_number != seqTracker[uniOffset]++) {
                seqError[uniOffset]++;
                seqTracker[uniOffset] = e131.packet->sequence_number + 1;
            }
            
            /* Find out starting pixel based off the Universe */
            uint16_t pixelStart = uniOffset * ppu;

            /* Calculate how many pixels we need from this buffer */
            uint16_t pixelStop = strip->PixelCount();
            if ((pixelStart + ppu) < pixelStop)
                //pixelStop = pixelStart + config.ppu;
                  pixelStop = pixelStart + ppu;

            /* Offset the channel if required for the first universe */
            uint16_t offset = 0;
            if (e131.universe == universe)
                offset = channel_start - 1;

            /* Set the pixel data */
            uint16_t buffloc = 0;
            for (uint16_t i = pixelStart; i < pixelStop; i++) {
                uint16_t j = buffloc++ * 3 + offset;
                //pixels.setPixelColor(i, e131.data[j], e131.data[j+1], e131.data[j+2]);
                strip->SetPixelColor(i, e131.data[j], e131.data[j+1], e131.data[j+2]);
            }

            /* Refresh when last universe shows up  or within 10ms if missed */
            if ((e131.universe == uniLast) || (millis() - lastPacket > 10)) {
            //if (e131.universe == uniLast) {
            //if (millis() - lastPacket > 25) {
                lastPacket = millis();
                strip->Show();
            }
        }
    }



//  Set to black if 30 seconds passed... 
    if (millis() - timeout > 30000)  { 
        strip->ClearTo(0,0,0); 
        timeout = millis();
      } 


//  Print out errors...
     if (millis() - timeout_data > 30000) {

    //   for (int i = 0; i < ((uniLast + 1) - universe); i++)
    //     seqErrors =+ seqError[i];

    uint32_t seqErrors = 0, packet_rate = 0;
    static uint32_t packets_last = 0; 
    for (int i = 0; i < ((uniLast + 1) - universe); i++)
        seqErrors =+ seqError[i];
    
    packet_rate = ( e131.stats.num_packets - packets_last ) / 30;
      Debugf("DMX: Total Packets = %u, Sequence errors = %u, Rate = %u /s \n", e131.stats.num_packets, seqErrors, packet_rate);  
      timeout_data = millis();
      packets_last = e131.stats.num_packets; 

     }



      
      break;

    case POST_EFFECT: 
      //packetno = 0; 

        // if (e131 != NULL)
        // {
        //     delete e131;

        // }


      Post_effect(); 
      timer_effect_tick_timeout = 100; // Restore limiter RAPID flow.... 
      break; 

      } 
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        Random Colour.... 
//    ToDo... add in palletee selection... Sort out refresh of clolour...
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







void cache Random_colour() {

 uint16_t  timer = map ( WS2812_Settings.Timer , 1, 2000, 1000 , 60000 );
 RgbColor colour; 
 //static uint8_t static_colour; 
 //static uint32_t temp_timer; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

        Debugln("Random Colour - Pre effect"); 
        Pre_effect();
        Effect_Refresh = true; 

        break;
    
    case RUN_EFFECT:  

    {

    const uint32_t effect_timer = map ( WS2812_Settings.Timer , 1, 2000, 1000 , 600000 ) ;

      if ( millis() - lasteffectupdate >  effect_timer || Effect_Refresh  )  {


      random_colour_timer(true);  //  test function to update colour if random is chosen... based on timer... 



     //   if (  || Effect_Refresh ) {

             // effectPosition++ ; 

             //  if (WS2812_Settings.Random == true ) {
             //    if (effectPosition == 10 || Effect_Refresh ) { static_colour = random(255); effectPosition = 0 ; } ; 


             //    colour = Return_Palette(Wheel(static_colour)) ;

              // } else {

                colour = Return_Palette(WS2812_Settings.Color) ;

              // }

                dim(colour);

                animator->FadeTo( map ( WS2812_Settings.Timer , 1, 2000, 500 , 5000 ), colour); 
                
                lasteffectupdate = millis(); 
                Effect_Refresh = false; 
         }
      }
        break;

    case POST_EFFECT:
        Post_effect(); 
        break;
  }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        UDP   function...  
//    
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void cache UDPfunc () {

int packetSize; 
//static int packetno = 0; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:
      timer_effect_tick_timeout = 0; 

      if(millis() > 60000) Adalight_Flash(); 
      Udp.beginMulticast(WiFi.localIP(), multicast_ip_addr, UDPlightPort); 
      Pre_effect(); 

    break; 

    case RUN_EFFECT:

      packetSize = Udp.parsePacket();

        if  (Udp.available())  {
             for (int i = 0; i < packetSize; i = i + 3) {
                if (i > pixelCount * 3) break;         // Stops reading if LED count is reached. 
                    pixelsPOINT[i + 1] = Udp.read();   // direct buffer is GRB, 
                    pixelsPOINT[i]     = Udp.read();
                    pixelsPOINT[i + 2] = Udp.read();
              }
              Udp.flush();
              strip->Dirty(); 
              strip->Show();  // takes 6ms with 200, take 12ms with 400 ----> so 100 takes 3ms. 

        }
      
      break;

    case POST_EFFECT: 
      //packetno = 0; 
      Udp.stop(); 
      Post_effect(); 
      timer_effect_tick_timeout = 100; 
      break; 

      
} 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 
//        Lava Lamp   function...  
//    
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








void cache LavaLamp () {

uint16_t x,y;
int16_t pixel;  
static XY coordinates; 
const uint16_t Total_y = return_total_y ( WS2812_Settings.Total_X ) ; 
//uint8_t* coordinates;        // Holds LED color values (3 bytes each)

  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

      // generate pixel... 

      coordinates.x = random ( 0, WS2812_Settings.Total_X ); 
      coordinates.y = random ( 0, Total_y ) ; 
      pixel = return_pixel(coordinates.x, coordinates.y, WS2812_Settings.Total_X); 

      Debugf("START (%u,%u)-> %u \n", coordinates.x, coordinates.y, pixel); 

    // coordinates = (uint8_t *)malloc(WS2812_Settings.Effect_Max_Size);

    // if (coordinates) 
    // {
    //     memset(coordinates, 0, WS2812_Settings.Effect_Max_Size * 3);   //  x, y, pixel..... 
    // }

      effectPosition = 0; 
      Pre_effect(); 
      Effect_Refresh = false; 
    break; 

    case RUN_EFFECT:
      {

        if (Effect_Refresh) Current_Effect_State = PRE_EFFECT; 

        if (  millis() - lasteffectupdate >  WS2812_Settings.Timer || Effect_Refresh)  {

          bool OK = false; 
          uint8_t counter = 0; 

          do {
            counter++;

            XY returned_XY = return_adjacent(coordinates); 
            
            pixel = return_pixel(returned_XY.x, returned_XY.y, WS2812_Settings.Total_X);   
            Debug("."); 
            if (pixel > -1 && !animator->IsAnimating(pixel)) {
              OK = true; 
              coordinates = returned_XY; 
              Debugln("");
            }
            if (counter == 5) break; 
          } while (!OK) ; 


        if (OK) {

        Debugf("(%u,%u)-> %u", coordinates.x, coordinates.y, pixel); 

        //originalColor = strip->GetPixelColor(pixel);
        RgbColor originalColor = RgbColor(0,0,0);
        RgbColor newcolor = Wheel( effectPosition++ % 255 );  //   WS2812_Settings.Color; 

        AnimUpdateCallback animUpdate = [=](float progress)
        {
         //  RgbColor updatedColor = RgbColor::LinearBlend(original, dim(WS2812_Settings.Color) ,  progress) ;
         //   strip->SetPixelColor(pixel, updatedColor);

                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, newcolor, new_progress);
                if (progress == 1.0) updatedColor = originalColor; 

                strip->SetPixelColor(pixel, updatedColor);

        };

        animator->StartAnimation(pixel, WS2812_Settings.Timer * WS2812_Settings.Effect_Option , animUpdate);
// 
// 
// 
// 
// 

            Effect_Refresh = false; 
            lasteffectupdate = millis(); 
          } // end of if OK
        } // end of timer if
      } // end of switch scope
      break;
    case POST_EFFECT: 






      Post_effect(); 
      break; 

      
} 
}



// void cache AnimatorClass1 () {

// //static uint32_t testimer; 

//   switch(Current_Effect_State) {

//     case PRE_EFFECT:
//     {

//         if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED
//         if (animatedobject != NULL) delete animatedobject; // 
        
//         const uint8_t numberofanimations = 5; 
//         animatedobject = new AnimatedObject( strip, animator, numberofanimations); //  ...  pointers to strip, animator number of objects... 

//         static uint8_t coordinate_array[numberofanimations * 2];  // create array to hold the x,y 


//     for (uint8_t i = 0; i < WS2812_Settings.Effect_Count; i++ ) {

//             RgbColor newcolor = RgbColor(0,0,0);

//             if (i == 0) newcolor = RgbColor(255,0,0);
//             if (i == 1) newcolor = RgbColor(0,255,0);
//             if (i == 2) newcolor = RgbColor (0,0,255);
//             if (i > 2 ) newcolor = Wheel(random(255));

//             //RgbColor newcolor = Wheel(random(255));


//             uint8_t x = random ( 0, WS2812_Settings.Total_X ); 
//             uint8_t y = random ( 0, return_total_y ( WS2812_Settings.Total_X ) ) ; 

//             coordinate_array[i * 2] = x; 
//             coordinate_array[(i * 2) + 1] = y; 

          
//             Debugf("Start Pixels %u = (%u,%u) \n", i, x,y );

//           ObjectCallback ObjectUpdate = [i, newcolor, &coordinate_array ](uint8_t &x_t, uint8_t &y_t)
//             {

//                   X_Y_Coordinates XY;       
//                   uint16_t pixel; 
//                   bool OK = false; 
//                   uint8_t counter = 0; 

//                   XY.X = coordinate_array[  i*2      ];
//                   XY.Y = coordinate_array[ (i*2) + 1 ];

//                   do {
//                       counter++;
//                       X_Y_Coordinates returned_XY = return_adjacent(XY); 
//                       pixel = return_pixel(returned_XY.X, returned_XY.Y, WS2812_Settings.Total_X) - 1;   

//                       if (!animator->IsAnimating(pixel)) {
//                           OK = true; 
//                           coordinate_array[  i*2      ]  = XY.X = returned_XY.X;
//                           coordinate_array[ (i*2) + 1 ]  = XY.Y = returned_XY.Y; 
//                           }

//                       if (counter == 5) break; // bail out if it has not found a suitable pixel!  
//                       } while (!OK) ; 


//                   if (OK) {
//          //Debugf("%u (%u,%u)-> %u | ", i, XY.X, XY.Y, pixel); 
//                   RgbColor originalColor = RgbColor(0,0,0);
//          //RgbColor newcolor = Wheel( effectPosition++ % 255 );  //   WS2812_Settings.Color; 

//                             AnimUpdateCallback animUpdate = [newcolor,originalColor,pixel](float progress)
//                               {
//                                RgbColor updatedColor; 
//                                if (progress < 0.5) updatedColor = RgbColor::LinearBlend(originalColor, newcolor, progress * 2.0f);
//                                if (progress > 0.5) updatedColor = RgbColor::LinearBlend(newcolor, originalColor, (progress * 2.0f) - 1.0f );
//                                strip->SetPixelColor(pixel, updatedColor);
//                               };

//                   animator->StartAnimation(pixel, WS2812_Settings.Timer * WS2812_Settings.Effect_Option , animUpdate);

//                   };

//              };

//             uint8_t slot = animatedobject->Add(ObjectUpdate, x, y);
//             Debugf("Started sequence %u \n", slot); 
//           }; // end of multiple effect count generations... 

//             Pre_effect(); 
//             lasteffectupdate = millis(); 
          
//     }

//     break; 

//     case RUN_EFFECT:
//       {
//         static bool triggered = false; 
     
//          if (  millis() - lasteffectupdate >  WS2812_Settings.Timer || Effect_Refresh)  {

//           animatedobject->UpdateAll();

//           //Debugln(); 
//           lasteffectupdate = millis(); 
//           Effect_Refresh = false; 
//         }

//       } // end of switch scope
//       break;
//     case POST_EFFECT: 

//       delete animatedobject; 
//       animatedobject = NULL; 
      
//       Post_effect(); 
//       break; 

      
// } 
// }

void cache Snakes (bool overlap) {
typedef std::function<void()> AniObjectCallback;

    struct AnimationVars
    {
        uint16_t position = 0; 
        RgbColor colour = RgbColor(0,0,0);
        XY coordinates = toXY(0,0); 
        AniObjectCallback ObjUpdate = NULL; 
        RgbColor oldcolour = RgbColor(0,0,0); 
        RgbColor newcolour = RgbColor(0,0,0); 

        bool effectchanged = false; 
    };

    static AnimationVars* _vars = NULL;
    static uint8_t animationCount; 
    static uint32_t counter, effect_timer; 
    static uint8_t static_colour; 
    static uint8_t old_R, old_G, old_B; 

    static bool triggered; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:
    {
        timer_effect_tick_timeout = 30;  // make the loop hit this faster to update colours..
        if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED
        animator->FadeTo(500, RgbColor(0,0,0)); // fade out current effect
        animationCount = WS2812_Settings.Effect_Count;  // assign this variable as requires re-initilisation of effect. 
   //     initialiseAnimationObject(animationCount);  // initialise animation object with correct number of animations. 
       
        if (_vars != NULL) delete[] _vars; 
        _vars = new AnimationVars[animationCount];  // memory for all the animated object properties... 

    for (uint8_t i = 0; i < animationCount; i++ ) {

            AnimationVars* pVars;
            pVars = &_vars[i]; 

            pVars->coordinates.x = random ( 0, WS2812_Settings.Total_X ); 
            pVars->coordinates.y = random ( 0, return_total_y ( WS2812_Settings.Total_X ) ) ; 

          if (WS2812_Settings.Palette_Choice == WHEEL) pVars->position = random(255); 


            AniObjectCallback ObjectUpdate = [pVars,overlap]()
  //        ObjectCallback ObjectUpdate = [pVars]() //  lamda func passes READ only pointer to the stuct containing the animation vars.. these can be written to in animation...
            {

                  int16_t pixel; 
                  bool OK = false; 
                  uint8_t counter = 0; 
                  do {
                      counter++;
                      XY returned_XY = return_adjacent(pVars->coordinates); 
                      pixel = return_pixel(returned_XY.x, returned_XY.y, WS2812_Settings.Total_X);   

                      // true checking 
                      if (pixel > -1 &&  !animator->IsAnimating(pixel) ) {
                          pVars->coordinates = returned_XY;
                          OK = true; 
                          }

                      // // skip animating effects. 
                      // if (pixel > -1 &&  !animator->IsAnimating(pixel) && WS2812_Settings.Effect_Option && counter > 2 ) {
                      //     pVars->coordinates = returned_XY;
                      //     //OK = true; 
                      //     counter = 0; 
                      //     }

                      // allows overlap bailout, but only after trying not to.   
                      if (pixel > -1 && counter > 9 && overlap) {
                          pVars->coordinates = returned_XY;
                          OK = true;                      
                      }

                      } while (!OK && counter < 10) ; 

                      RgbColor Fixed_Colour = pVars->colour; 


                  if (OK) {

                            RgbColor temptestOLD = strip->GetPixelColor(pixel); 
                            AnimUpdateCallback animUpdate = [pVars,pixel,temptestOLD, Fixed_Colour](float progress)
                              {
                                RgbColor updatedColor, NewColour;
                                (WS2812_Settings.Effect_Option == 0) ? NewColour = pVars->colour : NewColour = Fixed_Colour; 
                                if (progress < 0.5) updatedColor = RgbColor::LinearBlend(temptestOLD, NewColour,  progress * 2.0f);
                                if (progress > 0.5) updatedColor = RgbColor::LinearBlend(NewColour, RgbColor(0,0,0) , (progress * 2.0f) - 1.0f );
                                strip->SetPixelColor(pixel, updatedColor);
                              };

                            animator->StartAnimation(pixel, map( WS2812_Settings.Effect_Max_Size,0,255, WS2812_Settings.Timer * 2 , 20000 ) , animUpdate);

                  };

             };

            //animatedobject->Add(ObjectUpdate);
            pVars->ObjUpdate = ObjectUpdate; 
             
          }; // end of multiple effect count generations... 

            Pre_effect(); 
            lasteffectupdate = millis();
            effect_timer = 0;  
            triggered = false;
    }

    break; 

    case RUN_EFFECT:
      {
        AnimationVars* pVars;

        if (animationCount != WS2812_Settings.Effect_Count) Current_Effect_State = PRE_EFFECT; // reboot only if no animations changed
        
 // set new colours       

static bool Effect_Refresh_colour, Effect_Refresh_position; 

if (Effect_Refresh) { 
  Effect_Refresh_position = true;
  Effect_Refresh_colour = true; 
  Effect_Refresh = false; 
}


const uint32_t new_colour_time = map (WS2812_Settings.Timer, 0, 255, 20000, 300000) ; 



if (!triggered || Effect_Refresh_colour || random_colour_timer(new_colour_time)) {

        if (WS2812_Settings.Palette_Choice == WHEEL)  {

            for (uint8_t i = 0; i < animationCount; i++) {
              pVars = &_vars[i]; 
              pVars->colour = dim(Wheel(pVars->position++ % 255) );  
              pVars->effectchanged = false; 
            }

          }  else  { //if (WS2812_Settings.Effect_Option == 0) {
              
              
               
    //          if (WS2812_Settings.Random == true ) {

                      // if (millis() - effect_timer > new_colour_time  || Effect_Refresh_colour ) { 
                      //         static_colour = random(255); 
                      //         for (uint8_t i = 0; i < animationCount; i++) {
                      //                 pVars = &_vars[i]; 
                      //                 pVars->oldcolour = pVars->colour; 
                      //                 pVars->newcolour = dim(Return_Palette(Wheel(static_colour), i) ) ; 
                      //                 pVars->effectchanged = true; 
                      //         }
                      //  effect_timer = millis() ;
                      // }
    //          } else if (WS2812_Settings.Random == false ) {

                              for (uint8_t i = 0; i < animationCount; i++) {
                                      pVars = &_vars[i]; 
                                      pVars->oldcolour = pVars->colour; 
                                      pVars->newcolour = dim(Return_Palette(WS2812_Settings.Color, i) ); 
                                      pVars->effectchanged = true; 
                              }

              effect_timer = millis() ;            
              triggered = true; 
      //        }

          }
    Effect_Refresh_colour = false; 
}


//  Update the blending colours for each effect outside of the  other loops

        for (uint8_t i = 0; i < animationCount; i++) {

          pVars = &_vars[i]; 
          if (pVars->effectchanged == true) {

                const uint32_t transitiontime2 = 5000; // map (WS2812_Settings.Timer, 0, 255, 20000, 300000) ; 
                const uint32_t _time = (millis() - effect_timer); 
                float _delta = float (_time) /  float(transitiontime2)  ; // WS2812_Settings.Timer * 10 ) ; 
                
                if (_delta < 1.0) {
                  pVars->colour = HslColor::LinearBlend(  pVars->oldcolour , pVars->newcolour, _delta); 
                } else {
                  pVars->colour = pVars->newcolour;
                }

                if (_delta > 1) {  
                    pVars->effectchanged = false; 
                  }

            }      

        }     


    
// push effects out... 

    if (  millis() - lasteffectupdate >  WS2812_Settings.Timer || Effect_Refresh_position)  {
            // update POSITION...
            for (uint8_t i = 0; i < animationCount; i++) {
              pVars = &_vars[i]; 
              pVars->ObjUpdate();   //
            }

          lasteffectupdate = millis(); 
          Effect_Refresh_position = false; 
        }

            //}
      } // end of switch scope
      break;
    case POST_EFFECT: 

      timer_effect_tick_timeout = 100; 
   //   delete animatedobject; 
   //   animatedobject = NULL; 

        if (_vars)
    {
        delete[] _vars;
        _vars = NULL;
    }

      Debugln("End of Effect"); 
      Post_effect(); 
      break; 

      
} 
}



void cache Strobe () {
  
static uint16_t old_update_freq; 
static bool effect_state = false; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:
    {
           timer_effect_tick_timeout = 0; 
           old_update_freq = Pixel_Update_Freq; 
           Pixel_Update_Freq = 2 + ( pixelCount * 30 ) / 1000 ;   
           Pre_effect(); 
          lasteffectupdate = 0; 
    }

    break; 

    case RUN_EFFECT:
      {

        uint16_t freq = map (WS2812_Settings.Timer, 0, 255, 50, 100);

    if ( millis() - lasteffectupdate >  freq )  {
      effect_state = !effect_state; 
      if (effect_state) strip->ClearTo(WS2812_Settings.Color); else 
          strip->ClearTo(RgbColor(0,0,0));
          lasteffectupdate = millis(); 
          strip->Show(); 
      }



      } // end of switch scope
      break;
    case POST_EFFECT: 
           timer_effect_tick_timeout = 100; 
           Pixel_Update_Freq = old_update_freq; 
      Post_effect(); 
      break; 

      
} 
}



////////////////////////////////////

// void cache Empty () {


//   switch(Current_Effect_State) {

//     case PRE_EFFECT:
//     {

//             Pre_effect(); 
          

//     }

//     break; 

//     case RUN_EFFECT:
//       {


//       } // end of switch scope
//       break;
//     case POST_EFFECT: 

//       Post_effect(); 
//       break; 

      
// } 
// }



//////////////////////

// void cache AnimatorClass2 () {

// static uint32_t testimer; 

//   switch(Current_Effect_State) {

//     case PRE_EFFECT:
//     {

//         if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED
//         if (animatedobject != NULL) delete animatedobject; // ...
//         animatedobject = new AnimatedObject( strip, animator, 5); //  ...  pointers to strip, animator number of objects... 


//     for (uint8_t i = 0; i < WS2812_Settings.Effect_Count; i++ ) {

//             RgbColor newcolor = Wheel(random(255));

//             uint8_t x = random ( 0, WS2812_Settings.Total_X ); 
//             uint8_t y = random ( 0, return_total_y ( WS2812_Settings.Total_X ) ) ; 
          
//             Debugf("Start Pixels %u = (%u,%u) \n", i, x, y );

//              ObjectCallback ObjectUpdate = [x,y,i]()
//                 {
//                   static bool triggered = false; //  = x; 
//                   static uint16_t X = 0, Y = 0; 

//                   if (!triggered) { 
//                     X = x;
//                     Y = y; 
//                     triggered = true; 
//                   }
                  
//                   X++;
//                   Y++; 

//                   Debugf("%u Callback (%u,%u) \n", i, X, Y ); 
          

//                  };

//             uint8_t slot = animatedobject->Add(ObjectUpdate);

//             Debugf("Started sequence %u \n", slot); 

//           }; // end of multiple effect count generations... 


//             Pre_effect(); 
//             testimer = millis(); 
          

//     }

//     break; 

//     case RUN_EFFECT:
//       {
//         static bool triggered = false; 
     
//          if (  millis() - lasteffectupdate >  WS2812_Settings.Timer || Effect_Refresh)  {

//           animatedobject->UpdateAll();
//           lasteffectupdate = millis(); 
//           Effect_Refresh = false; 
//         }

//       } // end of switch scope
//       break;
//     case POST_EFFECT: 

//       delete animatedobject; 
//       animatedobject = NULL; 
      
//       Post_effect(); 
//       break; 

      
// } 
// }
