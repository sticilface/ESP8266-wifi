void handle_root () {

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + version + " ESP Melvide</title></head>\n<body><h1> Misc Functions</h1>\n";

  httpbuf += "<p> Heap Size = " + String(system_get_free_heap_size()) + "</p>";
  httpbuf += "<p><a href='/bytedump'> EEPROM DUMP </a>";
  httpbuf += "<br><a href='/wifi?eeprom=bytedump'> EEPROM DUMP BYTES </a>";
  httpbuf += "<br><a href='/wifi?eeprom=wipe'> EEPROM FORMAT </a>";
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);
  
}

void setup_Plugin () {



}


void loop_Plugin () {



}