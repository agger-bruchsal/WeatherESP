#ifndef webserver_h
#define webserver_h

#include <Arduino.h>

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server ( 80 );

String getPage(){
  String page = "{";
  page += "\"weatherdata\": {";
  for (unsigned int i = 0; i < sizeof(sensorReading)/sizeof(sensorReading[0]); i++ ) {
    page += "  \""+sensorReading[i].sensor+"\": \""+sensorReading[i].value+"\",";
  }
  page += "}";
  page += "}";
  return page;
}

void handleRoot(){ 
  server.send ( 200, "text/html", getPage() );
}

#endif