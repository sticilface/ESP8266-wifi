  const char htmlendstring[] PROGMEM = R"=====(
<p><a href='/'>Home</a>  <a href='/wifi'>WiFi</a>  <a href='/mqtt'>MQTT</a>  <a href='/misc'>MISC</a>  <br><a href='/io'>Input/Output</a> <a href='/test'>Test</a> <a href='/ws2812'>WS2812</a>
  )=====";  


const char webpage_misc_1[] PROGMEM = R"=====(
  <!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta http-equiv='refresh' content='30'> <meta name='viewport' content='initial-scale=1'><title> % ESP Melvide</title></head><body><h1> Misc Functions</h1>
  <p> Version = % 
  <br> Compile Time = % 
  <br> SDK Version = % 
  <br> Heap Size = % 
  <br> Flash Size = % 
  <br> Flash Size by ID = % 
  <br> Flash ID = % 
  <br> Chip ID = % 
  <br> Sketch Size = % 
  <br> Free Space = % 
  <br> Millis = % 
  <br> Up Time = % 
  <br> VCC = % 
  <br> RSSI = % 
  <br> CPU freq = % 
  <p><form action='/misc' method='POST'>
  <p> Select Speed <select name='serial' onchange='this.form.submit();'>
  )=====";

  // Use response1 += FPSTR(HTTP);  FPSTR(webpage_misc_1)



const char webpage_misc_2[] PROGMEM = R"=====(
  </select>
  </form></p>
  <p><a href='/bytedump'> EEPROM DUMP </a>
  <br><a href='/misc?eeprom=bytedump'> EEPROM DUMP BYTES </a>
  <br><a href='/misc?eeprom=wipe'> EEPROM FORMAT </a>
)=====";


const char webpage_ws2812_config_1[] PROGMEM = R"=====(
  <!DOCTYPE HTML>
  <head>
    <title>%</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'/>
    <meta http-equiv='Pragma' content='no-cache'>
    <link rel='shortcut icon' href='http://espressif.com/favicon.ico'>
    <style>
       body {background-color: #E6E6FA;}
    </style> 
  </head>
    <body><h1> % </h1>
    <br> <a href='/lightsconfig?reset=true'>RESET</a> | <a href='/lightsconfig?resetall=true'>RESET ALL</a> 
    <form name=form action='/lightsconfig' method='POST'>
)=====";



const char webpage_ws2812_config_2[] PROGMEM = R"=====(
  <input type='submit' value='Submit'>
          </form></p>
          <form action='/lightsconfig' method='POST'>
          <p>Save Preset: <input type='text' id='leds' name='preset' value='%' >
          <br><input type='submit'  value='Save'/>
          </form>
          <form action='/lightsconfig' method='POST'>
          <p>LEDs: <input type='text' id='leds' name='leds' value='%' >
          <br>  <input type='submit' value='Submit'/>
          </form>
)=====";


const char webpage_ws2812_main_1[] PROGMEM = R"=====(
<!DOCTYPE HTML>
  <head>
    <title>%</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'/>
    <meta http-equiv='Pragma' content='no-cache'>
    <link rel='shortcut icon' href='http://espressif.com/favicon.ico'>
    <style>
       body {background-color: #E6E6FA;}
    </style> 
  </head>
<body><h1> % </h1>
%
<br> <a href='/ws2812?mode=off'>OFF</a> | <a href='/ws2812?mode=on'>ON</a>  | % | <a href='/ws2812?mode=refresh'>REFRESH</a> | <a href='/lightsconfig'>CONFIG</a>
<br> PRESET: <a href='/ws2812?preset=1'>1</a> | <a href='/ws2812?preset=2'>2</a> | <a href='/ws2812?preset=3'>3</a> | <a href='/ws2812?preset=4'>4</a> | <a href='/ws2812?preset=5'>5</a> | <a href='/ws2812?preset=6'>6</a> | <a href='/ws2812?preset=7'>7</a> | <a href='/ws2812?preset=8'>8</a> | <a href='/ws2812?preset=9'>9</a> | <a href='/ws2812?preset=10'>10</a>
<form name=frmTest action='/ws2812' method='POST'>
<br>Select Mode <select name='modedrop' onchange='this.form.submit();'>
)=====";


//  FPSTR(webpage_ws2812_main_1)

const char webpage_ws2812_main_2[] PROGMEM = R"=====(
<form name=frm action='/ws2812' method='POST' style ='display:inline;'>
Select Palette <select name='palettedrop' onchange='this.form.submit();'>
)=====";


const char webpage_ws2812_main_3[] PROGMEM = R"=====(
<form action='/ws2812' method = 'POST' style ='display:inline;'>
  <input type='checkbox' name='random' value='1' % onchange='this.form.submit();'>
</form>
)=====";

const char webpage_ws2812_main_4[] PROGMEM = R"=====(
<form action='/ws2812' method='POST'
  <p>Color: <input class='color' name='rgbpicker' value = '%' >
  <br>  <input type='submit' value='Submit'/>
  </form>
)=====";


const char webpage_ws2812_main_5[] PROGMEM = R"=====(
  <form name=sliders action='/ws2812' method='POST'>
  <br>Brightness: <input type='range' name='dim'min='0' max='255' value='%' onchange='this.form.submit();' >
  <br>Timer: <input type='range' name='timer'min='1' max='2000' value='%' onchange='this.form.submit();'>
  </form><br>
    <form action='/ws2812' method='POST' style='display:inline;'>
    Current Preset: % 
    <br>Save Preset: <input type='text' id='presetsave' name='presetsave' value='%' >
    <input type='submit'  value='Save'/>
    </form>
    <br>Power = %mA
)=====";


 // //  FPSTR(webpage_mqtt_1)

const char webpage_mqtt_1[] PROGMEM = R"=====(
<!DOCTYPE HTML>
  <head>
    <title> % </title>
    <meta name='viewport' content='width=device-width, initial-scale=1'/>
    <meta http-equiv='Pragma' content='no-cache'>
    <link rel='shortcut icon' href='http://espressif.com/favicon.ico'>
    <style>
       body {background-color: #E6E6FA;}
    </style> 
  </head>
      <body><h1> % </h1>
      <title>MQTT Configuration</title></head><body><h1>MQTT Config</h1>
      <form action='/mqtt' method='POST'> ENABLED:
      <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='NO' % > NO 
      <input type='radio' onChange='this.form.submit();' name='form_MQTT_enabled' value='YES' %> YES 
      </form>
  )=====";

const char webpage_mqtt_2[] PROGMEM = R"=====(
  <br>Current device name is: <a href='http://%.local'>%</a>
  <br><form action='/mqtt' method='POST'>
  New Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>
  MQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>
  Enable Uptime <input type='radio' name='form_Uptime_enabled' value='NO' %> NO <input type='radio' name='form_Uptime_enabled' value='YES' %> YES
  <p><input type='submit' name='reboot' value='Reboot!'/>
  <input type='submit' value='Submit'/>
  </form></p>
)=====";


const char webpage_handlewifi_1[] PROGMEM = R"=====(
<!DOCTYPE HTML>
  <head>
    <title>Wifi Configuration</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'/>
    <meta http-equiv='Pragma' content='no-cache'>
    <link rel='shortcut icon' href='http://espressif.com/favicon.ico'>
    <style>
       body {background-color: #E6E6FA;}
    </style> 
</head>
    <body><h1>Wifi Config</h1>
    <p>Current IP address is: <a href='http://%'> % </a>
    <br>Current SSID is: %
    <br>Current MQTT Server is: % ... % 
    <br>Current device name is: <a href='http://%.local'>%.local</a>
    <br><form action='/wifi' method='POST'>
    New Device Name: <input type='text' id='deviceid' name='deviceid' value=''> (Restart Required)<br>
    MQTT Server IP: <input type='text' id='mqttserver' name='mqttserver' value=''><br>
     <p>Please Select Wifi Network...</p>
)=====";


  const char webpage_handlewifi_2[] PROGMEM = R"=====(
  Password: <input type='text' name='password' value=''><br/></p>
  <input type='submit' name='reboot' value='Reboot!'/>
  <input type='submit' name ='scan' value='Scan'/>
  <input type='button' onClick='window.location.replace(location.pathname)' value='Refresh'/>
  <input type='submit' value='Submit'/>
  </form></p>
  )=====";





