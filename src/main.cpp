#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// Identify the service as a health thermometer.
#define SERVICE_UUID BLEUUID((uint16_t) 0x1809)
// Identify the characteristic as a temperature measurement.
#define CHARACTERISTIC_UUID BLEUUID((uint16_t) 0x2A1C)

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
BLEServer *server;
BLEService *tempService;
BLECharacteristic *objTempCelChar;

void setup() {
  // Initiate serial monitor.
  Serial.begin(9600);
  // Initiate contactless IR thermometer.
  mlx.begin();
  // Initiate Bluetooth low energy.
  server = BLEDevice::createServer();
  tempService = server->createService(SERVICE_UUID);
  tempService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_INDICATE);
  tempService->start();
  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  // advertising->setMinPreferred(0x06); // iOS
  advertising->setMinPreferred(0x12); // Android
  BLEDevice::startAdvertising();
}

void loop() {

}