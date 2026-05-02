#include "deviceManager.h"

DeviceManager::DeviceManager() {};

DeviceManager& DeviceManager::getInstance()
{
  static DeviceManager instance;
  return instance;
}

void DeviceManager::connect(std::string port)
{
  device = new SerialManager(port);
  device->start();

  device->setCallback(
      [this](std::string data)
      {
        int value = std::stoi(data);

        float volume = (value - 5.0f) / (1020.0f - 5.0f);
        volume = std::clamp(volume, 0.0f, 1.0f);
        std::cout << "value: " << value << ", volume: " << volume << std::endl;
        subscriber(0, volume);
      });
  device->send("1024");
}

void DeviceManager::send(std::string message) { device->send(message); }

void DeviceManager::onVolumeChange(std::function<void(int index, float volume)> callback) { subscriber = callback; }
