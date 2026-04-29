#pragma once
#include <napi.h>

#include <iostream>
#include <thread>

#include "audioMixer.h"
#include "deviceManager.h"
#include "worker.h"

namespace API
{
AudioMixer* audioMixer;
Worker* worker;
DeviceManager* deviceManager;

Napi::Object init(Napi::Env env, Napi::Object exports);
NODE_API_MODULE(addon, init)

Napi::Value getPlaybackDevices(const Napi::CallbackInfo& info);
Napi::Value getCaptureDevices(const Napi::CallbackInfo& info);

Napi::Value addChannel(const Napi::CallbackInfo& info);
Napi::Value removeChannel(const Napi::CallbackInfo& info);

Napi::Value setChannelDevice(const Napi::CallbackInfo& info);
Napi::Value setChannelName(const Napi::CallbackInfo& info);
Napi::Value setChannelVolume(const Napi::CallbackInfo& info);

Napi::Value getUpdates(const Napi::CallbackInfo& info);

Napi::Value connectDevice(const Napi::CallbackInfo& info);
}  // namespace API