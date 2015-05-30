void loop() {
  // put your main code here, to run repeatedly:


  //mdns.update();  
  mqttclient.loop();
  server.handleClient();
    
 

 ////////////// -------- PLUGIN CONTROLs -----------------------
  

   timer.run();

 
loop_Plugin();
 
    //CPU_clock_count();

 //strip.UpdateAnimations();

 

}

