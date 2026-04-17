#pragma once
#include <napi.h>
#include <iostream>
#include <thread>
#include "audioMixer.h"

class Worker
{
private:
  std::thread workerThread;

  Napi::ThreadSafeFunction callbackFunction;

  bool running;

  Worker();
  ~Worker();

  void run();

public:
  static Worker &getInstance();

  Worker(const Worker &) = delete;
  Worker &operator=(const Worker &) = delete;

  void start(Napi::Env env, Napi::Function callback);
  void stop();
};