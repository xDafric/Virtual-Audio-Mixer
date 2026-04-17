#pragma once
#include <napi.h>
#include <iostream>
#include "audioMixer.h"
#include "worker.h"

namespace API
{
  AudioMixer *audioMixer;
  Worker *worker;

  Napi::Object init(Napi::Env env, Napi::Object exports);
  NODE_API_MODULE(addon, init)

  Napi::Value getPlaybackDevices(const Napi::CallbackInfo &info);
  Napi::Value getCaptureDevices(const Napi::CallbackInfo &info);
  Napi::Value addInputDevice(const Napi::CallbackInfo &info);

  Napi::Value getUpdates(const Napi::CallbackInfo &info);
}