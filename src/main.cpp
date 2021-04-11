#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// Identify the service as a health thermometer.
#define HEALTH_THERMOMETER_SERVICE_UUID BLEUUID((uint16_t) 0x1809)
// Identify the characteristic as a temperature measurement.
#define TEMPERATURE_MEASUREMENT_CHARACTERISTIC_UUID BLEUUID((uint16_t) 0x2A1C)
// Value calibration for the contactless IR sensor.
#define CALIBRATION_VALUE 0 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
BLEServer *server;
BLEService *tempService;
BLECharacteristic *objTempCelChar;

int val = 0;
void setup() {
  // Initiate serial monitor.
  Serial.begin(9600);
  // Initiate contactless IR thermometer.
  mlx.begin();
  // Initiate Bluetooth low energy.
  BLEDevice::init("TracingCzar-Proto");;
  server = BLEDevice::createServer();
  tempService = server->createService(HEALTH_THERMOMETER_SERVICE_UUID);
  objTempCelChar = tempService->createCharacteristic(TEMPERATURE_MEASUREMENT_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_INDICATE);
  tempService->start();
  
  // Set Bluetooth advertising settings.
  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(HEALTH_THERMOMETER_SERVICE_UUID);
  advertising->setScanResponse(true);
  // advertising->setMinPreferred(0x06); // iOS
  advertising->setMinPreferred(0x12); // Android
  BLEDevice::startAdvertising();
}

void loop() {
  double temp = mlx.readObjectTempC();
  objTempCelChar->setValue(temp);
  objTempCelChar->indicate();
  Serial.println(temp);
  delay(5000);
}