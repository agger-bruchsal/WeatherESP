#ifndef user_config_h
#define user_config_h

// Local Hostname (where you want to reach your controller at)
String host = "weatherstation";

//Connect to the following MQTT Server
const char* mqtt_server = "10.10.10.10";

#define SEALEVELPRESSURE_HPA 1013.25 

struct sensorReadings {
  //uint32_t timeStamp;
  String sensor;
  double value;
};

struct sensorState {
  //uint32_t timeStamp;
  String sensor;
  String value;
};

float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

sensorReadings sensorReading[] = {
	{"temperature", 0.0},
	{"pressure", 0.0},
	{"humidity", 0.0},
	{"luminosity", 0.0},
	{"uv_index", 0.0},
	{"air_quality_index", 0.0}
};

sensorState sensorState[] = {
		{"air_quality", "unknown"}
};

float hum_score, gas_score;
float gas_reference = 250000;
float hum_reference = 40;
int getgasreference_count = 0;

// Update stuff every 5 seconds by default
unsigned long INTERVAL = 5000;   // time between reads
long lastReadSensors, lastSendMqtt;
#endif
