#include <napi.h>
#include <iostream>
#include "audioMixer.h"

namespace API
{
  AudioMixer *audioMixer;

  Napi::Value getPlaybackDevices(const Napi::CallbackInfo &info);
  Napi::Value getCaptureDevices(const Napi::CallbackInfo &info);
  Napi::Value addInputDevice(const Napi::CallbackInfo &info);

  Napi::Object init(Napi::Env env, Napi::Object exports);
  NODE_API_MODULE(addon, init)
}