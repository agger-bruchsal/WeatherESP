#include <WiFiManager.h>
#include <ESP8266WebServer.h>

#include <Wire.h>

#include "lib/user_config.h"
#include "lib/sensors.h"
#include "lib/webserver.h"
#include "lib/mqtt_handler.h"

void setup() {
  Serial.begin ( 115200 );
  Wire.begin();                                 // I2C-Bus initialisieren GPIO2-Data GPIO0-Clock
  WiFi.hostname( host );
  WiFiManager wifiManager;

  wifiManager.setBreakAfterConfig(true);
  if (!wifiManager.autoConnect("ESP_WeatherStation")) {
    Serial.println("failed to connect, we will fire up config mode");
    delay(3000);
    wifiManager.startConfigPortal("ESP_WeatherStation");
    delay(5000);
  }
  /*return index page which is stored in serverIndex */
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
  setupMqtt();
  sensorSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  readSensors();
  handleMQTT();
}
