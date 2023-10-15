#include <Wire.h>

enum class ModuleState {
  // Startup state; the module is expected to send the initial
  // 20 bytes of the sensor auth descriptor
  NotConnected,

  // The module has sent the first 20 bytes of its auth
  // descriptor; any request in this state is expected to
  // stream the remaining descriptor bytes to the mainboard
  SendAuthChunks,

  // All bytes of the descriptor have been sent; in this state
  // the module is expected to send its current measurement
  // values in 32-byte chunks
  Connected
};

//BME680
typedef struct s_BME680Values {
  float humidity;
  float temperature;
  float air_pressure;
  float air_quality;
}   t_BME680Values;

//Sensor authentification
const static uint8_t g_authDescriptor[] = {
  // TODO: Populate using the script
};
static ModuleState g_state = ModuleState::NotConnected;
static uint32_t g_authChunkOffset;


void start_connection()
{
  Wire.requestFrom(1, sizeof(float));
}

void getBME_values(void)
{
  uint8_t buffer[sizeof(t_BME680Values)];
  //t_BME680Values value;
  t_BME680Values bmeValues;

  Wire.requestFrom(1, sizeof(t_BME680Values));
  while (Wire.available() < sizeof(t_BME680Values));
  for (int index = 0; index < sizeof(t_BME680Values); index++) {
    buffer[index] = Wire.read();
  }
  //value = *(t_BME680Values *)buffer;
  bmeValues = *(t_BME680Values *)buffer;
  //memcpy(&bmeValues, buffer, sizeof(t_BME680Values));
  Serial.println(bmeValues.temperature);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Wire.begin();
  getBME_values();
  //Wire.requestFrom(1, sizeof(float));
}

void loop() {
  
  
  delay(2000);
}
