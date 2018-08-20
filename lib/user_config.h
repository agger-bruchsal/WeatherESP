#ifndef user_config_h
#define user_config_h

// Local Hostname (where you want to reach your controller at)
String host = "weatherstation";

struct sensorReadings {
  //uint32_t timeStamp;
  String sensor;
  double value;
};

sensorReadings sensorReading[] = {
	{"temperature", 0.0},
	{"pressure", 0.0},
	{"humidity", 0.0},
	{"gas", 0.0},
	{"luminosity", 0.0},
	{"uv_index", 0.0}
};


//Connect to the following MQTT Server
const char* mqtt_server = "10.10.10.10";

#define SEALEVELPRESSURE_HPA 1013.25 

uint16_t luminosity;
float uva_radiation;
float uvb_radiation;

// Update stuff every 5 seconds by default
unsigned long INTERVAL = 5000;   // time between reads
long lastReadSensors, lastSendMqtt;
#endif
