#include <Wire.h>
#include <string>
#define SLAVE_ADDRESS 1

int sensorPin = A1; 
int wantRequest = 0;

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

const static uint8_t g_authDescriptor[] = {
  0x42, 0x79, 0x74, 0x65, 0x4d, 0x65, 0xff, 0x41, 0x75, 0x64, 0x69, 0x00,
  0xea, 0x03, 0x00, 0x00, 0x96, 0x02, 0x00, 0x00, 0x03, 0x0c, 0x30, 0x02,
  0x00, 0xf1, 0x2f, 0xf7, 0x04, 0x04, 0x66, 0x4e, 0x97, 0x8f, 0x2c, 0x1b,
  0x1e, 0x9d, 0x44, 0x4d, 0x04, 0x49, 0x4d, 0x55, 0x53, 0x4f, 0x4d, 0x45,
  0x2d, 0x50, 0x41, 0x52, 0x54, 0x2d, 0x4e, 0x4f, 0x54, 0x68, 0x65, 0x20,
  0x42, 0x4d, 0x45, 0x36, 0x38, 0x30, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68,
  0x65, 0x20, 0x66, 0x69, 0x72, 0x73, 0x74, 0x20, 0x67, 0x61, 0x73, 0x20,
  0x73, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20,
  0x69, 0x6e, 0x74, 0x65, 0x67, 0x72, 0x61, 0x74, 0x65, 0x73, 0x20, 0x68,
  0x69, 0x67, 0x68, 0x2d, 0x6c, 0x69, 0x6e, 0x65, 0x61, 0x72, 0x69, 0x74,
  0x79, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x68, 0x69, 0x67, 0x68, 0x2d, 0x61,
  0x63, 0x63, 0x75, 0x72, 0x61, 0x63, 0x79, 0x20, 0x67, 0x61, 0x73, 0x2c,
  0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x75, 0x72, 0x65, 0x2c, 0x20, 0x68,
  0x75, 0x6d, 0x69, 0x64, 0x69, 0x74, 0x79, 0x20, 0x61, 0x6e, 0x64, 0x20,
  0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20,
  0x73, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x73, 0x2e, 0x20, 0x49, 0x74, 0x20,
  0x69, 0x73, 0x20, 0x65, 0x73, 0x70, 0x65, 0x63, 0x69, 0x61, 0x6c, 0x6c,
  0x79, 0x20, 0x64, 0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65, 0x64, 0x20,
  0x66, 0x6f, 0x72, 0x20, 0x6d, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x20, 0x61,
  0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20,
  0x61, 0x6e, 0x64, 0x20, 0x77, 0x65, 0x61, 0x72, 0x61, 0x62, 0x6c, 0x65,
  0x73, 0x20, 0x77, 0x68, 0x65, 0x72, 0x65, 0x20, 0x73, 0x69, 0x7a, 0x65,
  0x20, 0x61, 0x6e, 0x64, 0x20, 0x6c, 0x6f, 0x77, 0x20, 0x70, 0x6f, 0x77,
  0x65, 0x72, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x75, 0x6d, 0x70, 0x74, 0x69,
  0x6f, 0x6e, 0x20, 0x61, 0x72, 0x65, 0x20, 0x63, 0x72, 0x69, 0x74, 0x69,
  0x63, 0x61, 0x6c, 0x20, 0x72, 0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x6d,
  0x65, 0x6e, 0x74, 0x73, 0x2e, 0x20, 0x54, 0x68, 0x65, 0x20, 0x42, 0x4d,
  0x45, 0x36, 0x38, 0x30, 0x20, 0x67, 0x75, 0x61, 0x72, 0x61, 0x6e, 0x74,
  0x65, 0x65, 0x73, 0x20, 0x2d, 0x20, 0x64, 0x65, 0x70, 0x65, 0x6e, 0x64,
  0x69, 0x6e, 0x67, 0x20, 0x6f, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73,
  0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x63, 0x20, 0x6f, 0x70, 0x65, 0x72,
  0x61, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x6f, 0x64, 0x65, 0x20, 0x2d,
  0x20, 0x6f, 0x70, 0x74, 0x69, 0x6d, 0x69, 0x7a, 0x65, 0x64, 0x20, 0x63,
  0x6f, 0x6e, 0x73, 0x75, 0x6d, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x2c, 0x20,
  0x6c, 0x6f, 0x6e, 0x67, 0x2d, 0x74, 0x65, 0x72, 0x6d, 0x20, 0x73, 0x74,
  0x61, 0x62, 0x69, 0x6c, 0x69, 0x74, 0x79, 0x20, 0x61, 0x6e, 0x64, 0x20,
  0x68, 0x69, 0x67, 0x68, 0x20, 0x45, 0x4d, 0x43, 0x20, 0x72, 0x6f, 0x62,
  0x75, 0x73, 0x74, 0x6e, 0x65, 0x73, 0x73, 0x2e, 0x20, 0x49, 0x6e, 0x20,
  0x6f, 0x72, 0x64, 0x65, 0x72, 0x20, 0x74, 0x6f, 0x20, 0x6d, 0x65, 0x61,
  0x73, 0x75, 0x72, 0x65, 0x20, 0x61, 0x69, 0x72, 0x20, 0x71, 0x75, 0x61,
  0x6c, 0x69, 0x74, 0x79, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x70, 0x65, 0x72,
  0x73, 0x6f, 0x6e, 0x61, 0x6c, 0x20, 0x77, 0x65, 0x6c, 0x6c, 0x62, 0x65,
  0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x65, 0x20, 0x67, 0x61, 0x73, 0x20,
  0x73, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x20, 0x77, 0x69, 0x74, 0x68, 0x69,
  0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x42, 0x4d, 0x45, 0x36, 0x38, 0x30,
  0x20, 0x63, 0x61, 0x6e, 0x20, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x20,
  0x61, 0x20, 0x62, 0x72, 0x6f, 0x61, 0x64, 0x20, 0x72, 0x61, 0x6e, 0x67,
  0x65, 0x20, 0x6f, 0x66, 0x20, 0x67, 0x61, 0x73, 0x65, 0x73, 0x20, 0x73,
  0x75, 0x63, 0x68, 0x20, 0x61, 0x73, 0x20, 0x76, 0x6f, 0x6c, 0x61, 0x74,
  0x69, 0x6c, 0x65, 0x20, 0x6f, 0x72, 0x67, 0x61, 0x6e, 0x69, 0x63, 0x20,
  0x63, 0x6f, 0x6d, 0x70, 0x6f, 0x75, 0x6e, 0x64, 0x73, 0x20, 0x28, 0x56,
  0x4f, 0x43, 0x29, 0x2e, 0x0b, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61,
  0x74, 0x75, 0x72, 0x65, 0x08, 0x68, 0x75, 0x6d, 0x69, 0x64, 0x69, 0x74,
  0x79, 0x0c, 0x61, 0x69, 0x72, 0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x75,
  0x72, 0x65, 0x0b, 0x61, 0x69, 0x72, 0x20, 0x71, 0x75, 0x61, 0x6c, 0x69,
  0x74, 0x79
  // TODO: Populate using the script
};

static ModuleState g_state = ModuleState::NotConnected;
static uint32_t g_authChunkOffset;

static void requestHandler() {
  if (g_state != ModuleState::Connected)
  {
  switch (g_state) {
    case ModuleState::NotConnected:
      Wire.write(&g_authDescriptor[0], 20);
      g_state = ModuleState::SendAuthChunks;
      g_authChunkOffset = 20;
      break;
    case ModuleState::SendAuthChunks:
      if (sizeof(g_authDescriptor) - g_authChunkOffset <= 32) {
        Wire.write(&g_authDescriptor[g_authChunkOffset],
          (size_t)(sizeof(g_authDescriptor) - g_authChunkOffset));
        g_state = ModuleState::Connected;
        g_authChunkOffset = sizeof(g_authDescriptor);
      } else {
        Wire.write(&g_authDescriptor[g_authChunkOffset], 32);
        g_authChunkOffset += 32;
      }
      break;
    case ModuleState::Connected:
      break;
  }
  else
      int wantRequest = 1;

}


 void receiveEvent(int data) {
  while (Wire.available()) { 
    char c = Wire.read(); 
    Serial.print(c);   
    }        // print the character
  }

// void requestEvent(){

//   // Wire.write("test1\0", 6);

//       // Aktueller Spannungswert wird gemessen...
//     int rawValue = analogRead(sensorPin);
//     float voltage = rawValue * (5.0/1023) * 1000;
         
//     float resitance = 10000 * ( voltage / ( 5000.0 - voltage) );
         
//     // ... und hier auf die serielle Schnittstelle ausgegeben
//     Serial.print("Spannungswert:"); Serial.print(voltage); Serial.print("mV");
//     Serial.print(", Widerstandswert:"); Serial.print(resitance); Serial.println("Ohm");
//     Serial.println("---------------------------------------");
//     Wire.write((uint8_t *)&voltage, sizeof(float));

// }

class Value
{
  public:
        float a;
        float b;
        float c;
        float d;
};

void setup()
{
  Wire.begin(1);               
  Wire.onReceive(receiveEvent);             
  Wire.onRequest(requestHandler);   
}

void sendSensorData(){

}
void loop()
{  
   while(wantRequest == 1)
  {
    int rawValue = analogRead(sensorPin);
    float voltage = rawValue * (5.0/1023) * 1000;
         
    float resitance = 10000 * ( voltage / ( 5000.0 - voltage) );
         
    // ... und hier auf die serielle Schnittstelle ausgegeben
    Serial.print("Spannungswert:"); Serial.print(voltage); Serial.print("mV");
    Serial.print(", Widerstandswert:"); Serial.print(resitance); Serial.println("Ohm");
    Serial.println("---------------------------------------");
    Value value;
    value.a = rawValue * (5.0/1023) * 1000;
    value.b = rawValue * (5.0/1023) * 1000;
    value.c = rawValue * (5.0/1023) * 1000;
    value.d = rawValue * (5.0/1023) * 1000;

    Wire.write((uint8_t *)&value, sizeof(Value));

    wantRequest = 0;
  }
  delay(1000);
      
}
 
