#ifndef webserver_h
#define webserver_h

#include <Arduino.h>

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server ( 80 );

String getPage(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  JsonObject& weatherdata = json.createNestedObject("weatherdata");
    for (unsigned int i = 0; i < sizeof(sensorReading)/sizeof(sensorReading[0]); i++ ) {
    	weatherdata[sensorReading[i].sensor] = sensorReading[i].value;
    }
  JsonObject& readable_states = json.createNestedObject("readable_states");
    for (unsigned int i = 0; i < sizeof(sensorState)/sizeof(sensorState[0]); i++ ) {
      readable_states[sensorState[i].sensor] = sensorState[i].value;
    }

  String output;
  json.printTo(output);
  return output;

}

void handleRoot(){ 
  server.send ( 200, "text/html", getPage() );
}

#endif