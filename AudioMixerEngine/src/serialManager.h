#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <thread>

#include "serialib.h"

class SerialManager
{
 private:
  serialib serial;
  std::string port;

  char buffer[256];

  std::queue<std::string> queue;

  std::thread thread;
  bool running;

  std::function<void(char[256])> onValueReceived;

  void run();

  ~SerialManager();

 public:
  SerialManager(std::string port);

  void setCallback(std::function<void(char[256])> cb);

  void start();
  void stop();

  void send(std::string message);
};