#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define bleServerName "Hello_BLE"
unsigned long lastTime = 0;
unsigned long timerDelay = 3000; 
bool deviceConnected = false;

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
#define STRING_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic stringCharacteristic(STRING_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor stringDescriptor(BLEUUID((uint16_t)0x2901)); 

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) { deviceConnected = true; }
    void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

void setup() {
    pinMode(25, OUTPUT);
    Serial.begin(115200);
    BLEDevice::init(bleServerName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *stringService = pServer->createService(SERVICE_UUID);
    
    stringService->addCharacteristic(&stringCharacteristic);
    stringDescriptor.setValue("Hello Message");
    stringCharacteristic.addDescriptor(&stringDescriptor);
    stringCharacteristic.addDescriptor(new BLE2902()); 
    
    stringService->start();
    pServer->getAdvertising()->start();
    Serial.println("Waiting for a client...");
}

void loop() {
    digitalWrite(25, HIGH);
    
          if (deviceConnected && (millis() - lastTime) > timerDelay) {
          unsigned long sendTime = micros();
          String message = "TS:" + String(sendTime); // Include timestamp
          stringCharacteristic.setValue(message.c_str());
          stringCharacteristic.notify();
          Serial.println("Sent timestamped message");
          lastTime = millis();
      }
      
    if (deviceConnected && (millis() - lastTime) > timerDelay) {
        //stringCharacteristic.setValue("Example Sensor Value");
        stringCharacteristic.notify();
        //Serial.println("Sent Message");
        lastTime = millis();
    }
}
