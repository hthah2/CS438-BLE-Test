#include "BLEDevice.h"

#define bleServerName "Hello_BLE"

static BLEUUID serviceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");
static BLEUUID stringCharacteristicUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static boolean doConnect = false;
static boolean connected = false;

// BLE Address and client pointers
static BLEAddress *pServerAddress;
static BLERemoteCharacteristic* stringCharacteristic;

const uint8_t notificationOn[] = {0x1, 0x0};

String receivedMessage = "";

static void stringNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                uint8_t* pData, size_t length, bool isNotify) {
    String receivedMessage = String((char*)pData).substring(0, length);
    
    if (receivedMessage.startsWith("TS:")) {
        unsigned long sendTime = receivedMessage.substring(3).toInt();
        unsigned long recvTime = micros();
        unsigned long latency = recvTime - sendTime;
        
        Serial.print("Latency: ");
        Serial.print(latency);
        Serial.println(" µs");
    }
  
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      Serial.println("Server found. Connecting!");
    }
  }
};

bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient();
  
  if (!pClient->connect(pAddress)) {
    Serial.println("Connection failed");
    return false;
  }
  Serial.println("Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.println("Service not found");
    return false;
  }

  stringCharacteristic = pRemoteService->getCharacteristic(stringCharacteristicUUID);
  if (stringCharacteristic == nullptr) {
    Serial.println("Characteristic not found");
    return false;
  }

  if(stringCharacteristic->canNotify()) {
    stringCharacteristic->registerForNotify(stringNotifyCallback);
  }
  
  return true;
}

void setup() {
  pinMode(25, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  digitalWrite(25, HIGH);
  if (doConnect) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("Connected!");
      stringCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("Connection failed");
    }
    doConnect = false;
  }
  
  delay(1000);
}
