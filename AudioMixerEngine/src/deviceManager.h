#pragma once
#include <iostream>

#include "serialManager.h"

class DeviceManager
{
 private:
  SerialManager* device;

  DeviceManager();

  std::function<void(int index, float volume)> subscriber;

 public:
  static DeviceManager& getInstance();

  DeviceManager(const DeviceManager&) = delete;
  DeviceManager& operator=(const DeviceManager&) = delete;

  void connect(std::string port);

  void send(std::string message);

  void onVolumeChange(std::function<void(int index, float volume)> callback);
};