#include "serialManager.h"

SerialManager::SerialManager(std::string port)
{
  SerialManager::port = port;
  running = false;

  if (serial.openDevice(this->port.c_str(), 115200) != 1)
  {
    std::cout << "Could not Connect to Device\n";
    return;
  }
}

void SerialManager::start()
{
  running = true;
  thread = std::thread(&SerialManager::run, this);
}

void SerialManager::stop()
{
  if (running)
  {
    running = false;

    if (thread.joinable())
    {
      thread.join();
    }
  }
}

void SerialManager::run()
{
  std::string incoming;

  while (running)
  {
    char byte;
    int bytesRead = serial.readChar(&byte, 10);

    if (bytesRead > 0)
    {
      incoming += byte;
      if (byte == '\n')
      {
        if (onValueReceived)
        {
          onValueReceived(incoming);
        }
        incoming = "";
      }
    }

    if (!queue.empty())
    {
      std::string msg = std::move(queue.front());
      queue.pop();
      serial.writeString(msg.c_str());
    }
  }
}

void SerialManager::send(std::string message)
{
  std::string msg = message + "\n";
  queue.push(msg);
}

void SerialManager::setCallback(std::function<void(std::string)> cb) { onValueReceived = cb; }

SerialManager::~SerialManager()
{
  stop();
  serial.closeDevice();
}