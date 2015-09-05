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
  uint16_t pixel; // has to carry negative value now. 
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

      for (uint16_t pixel = 0; pixel < pixelCount; pixel++) {
      RgbColor originalColor = strip->GetPixelColor(pixel); 
      AnimUpdateCallback animUpdate = [=](float progress)
            {
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, RgbColor(0,0,0), progress);
                strip->SetPixelColor(pixel, updatedColor);
            };

            animator->StartAnimation(pixel, 1000 , animUpdate); // might change this to be a random variant...

        }

       
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

          if (pixel > 0) { 
                if (animator->IsAnimating(pixel - 1)) { coordinates_OK = false ;  break; }; 
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

            if (pixel > 0) {
                pixel -= 1;   //*****       SUBTRACT ONE TO GET TRUE PIXEL       **** 

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
 static uint8_t static_colour; 
 //static uint32_t temp_timer; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

        Debugln("Random Colour - Pre effect"); 
        Pre_effect();
        Effect_Refresh = true; 

        break;
    
    case RUN_EFFECT:  
  
      if (  millis() - lasteffectupdate >  map ( WS2812_Settings.Timer , 1, 2000, 1000 , 600000 )  || Effect_Refresh)  {

        if ( !animator->IsAnimating() || Effect_Refresh ) {

              effectPosition++ ; 

               if (WS2812_Settings.Random == true ) {
                 if (effectPosition == 10 || Effect_Refresh ) { static_colour = random(255); effectPosition = 0 ; } ; 


                 colour = Return_Palette(Wheel(static_colour)) ;

               } else {

                colour = Return_Palette(WS2812_Settings.Color) ;

               }

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

static uint16_t x,y, pixel;  
static X_Y_Coordinates XY; 
const uint16_t Total_y = return_total_y ( WS2812_Settings.Total_X ) ; 
uint8_t* coordinates;        // Holds LED color values (3 bytes each)
RgbColor originalColor; 
AnimUpdateCallback animUpdate; 

  switch(Current_Effect_State) {

    case PRE_EFFECT:

    if (!Enable_Animations) { Current_Effect_State = POST_EFFECT ; HoldingOpState = OFF; break;  } //  DO NOT RUN IF ANIMATIONS DISABLED

      // generate pixel... 

      XY.X = random ( 0, WS2812_Settings.Total_X ); 
      XY.Y = random ( 0, Total_y ) ; 
      pixel = return_pixel(XY.X, XY.Y, WS2812_Settings.Total_X) - 1; 

      Debugf("START (%u,%u)-> %u \n", XY.X, XY.Y, pixel); 

    // coordinates = (uint8_t *)malloc(WS2812_Settings.Effect_Max_Size);

    // if (coordinates) 
    // {
    //     memset(coordinates, 0, WS2812_Settings.Effect_Max_Size * 3);   //  x, y, pixel..... 
    // }


      Pre_effect(); 
      Effect_Refresh = false; 
    break; 

    case RUN_EFFECT:

        if (Effect_Refresh) Current_Effect_State = PRE_EFFECT; 

        if (  millis() - lasteffectupdate >  WS2812_Settings.Timer   || Effect_Refresh)  {


          do {

            XY = return_adjacent(XY); 
            pixel = return_pixel(XY.X, XY.Y, WS2812_Settings.Total_X) - 1;   
          
          } while ( animator->IsAnimating(pixel)) ; 




        Debugf("(%u,%u)-> %u \n", XY.X, XY.Y, pixel); 

        //originalColor = strip->GetPixelColor(pixel);
        originalColor = RgbColor(0,0,0);


        animUpdate = [=](float progress)
        {
         //  RgbColor updatedColor = RgbColor::LinearBlend(original, dim(WS2812_Settings.Color) ,  progress) ;
         //   strip->SetPixelColor(pixel, updatedColor);

                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, dim(WS2812_Settings.Color), new_progress);
                if (progress == 1.0) updatedColor = originalColor; 

                strip->SetPixelColor(pixel, updatedColor);

        };

        animator->StartAnimation(pixel, WS2812_Settings.Timer-100 , animUpdate);
// 
// 
// 
// 
// 

        Effect_Refresh = false; 
        lasteffectupdate = millis(); 
        }

      break;
    case POST_EFFECT: 


        free(coordinates);




      Post_effect(); 
      break; 

      
} 
}



X_Y_Coordinates return_adjacent(X_Y_Coordinates Input) {
X_Y_Coordinates Output; 
bool OK;
              do {  
                uint16_t X = Input.X;
                uint16_t Y = Input.Y;
                uint8_t direction = random(8); 
                OK = false; 

                if (direction == 0 || direction == 3 || direction == 5 ) X--;
                if (direction == 0 || direction == 1 || direction == 2 ) Y++;
                if (direction == 2 || direction == 4 || direction == 7 ) X++; 
                if (direction == 5 || direction == 6 || direction == 7 ) Y--; 

                //  direction generated...  Now check if it is valid, assigns it to output.... 
                if (X < WS2812_Settings.Total_X && 
                    Y < return_total_y ( WS2812_Settings.Total_X ) && 
                    return_pixel(X, Y, WS2812_Settings.Total_X) != 0 )
                {
                  OK = true;
                  Output.X = X; 
                  Output.Y = Y;
                }
              
            } while ( !OK ) ; // 

return Output; 

}
