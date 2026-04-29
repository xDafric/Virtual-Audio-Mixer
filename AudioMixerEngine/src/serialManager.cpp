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
  while (running)
  {
    int bytesRead = serial.readString(buffer, '\n', 256, 10);

    if (bytesRead > 0)
    {
      try
      {
        buffer[bytesRead] = '\0';

        if (onValueReceived)
        {
          onValueReceived(buffer);
        }
      }
      catch (...)
      {
        std::cout << "invalid input" << std::endl;
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

void SerialManager::setCallback(std::function<void(char[256])> cb) { onValueReceived = cb; }

SerialManager::~SerialManager()
{
  stop();
  serial.closeDevice();
}