#include "main.h"

Napi::Object API::init(Napi::Env env, Napi::Object exports)
{
  audioMixer = &AudioMixer::getInstance();
  audioMixer->initMiniaudio();
  audioMixer->startEngine();

  worker = &Worker::getInstance();

  exports.Set("getPlaybackDevices", Napi::Function::New(env, API::getPlaybackDevices));
  exports.Set("getCaptureDevices", Napi::Function::New(env, API::getCaptureDevices));
  exports.Set("addInputDevice", Napi::Function::New(env, API::addInputDevice));
  exports.Set("getUpdates", Napi::Function::New(env, API::getUpdates));

  return exports;
}

Napi::Value API::getPlaybackDevices(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  ma_device_info *playbackDevices;
  ma_uint32 playbackDeviceCount;

  audioMixer->getPlaybackDevices(&playbackDevices, &playbackDeviceCount);

  Napi::Array array = Napi::Array::New(info.Env(), playbackDeviceCount);

  for (int i = 0; i < playbackDeviceCount; i++)
  {
    ma_device_info device = playbackDevices[i];
    Napi::Object deviceObj = Napi::Object::New(env);
    deviceObj.Set("id", Napi::Number::New(env, i));
    deviceObj.Set("name", Napi::String::New(env, device.name));

    array.Set(i, deviceObj);
  }

  return array;
}

Napi::Value API::getCaptureDevices(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  ma_device_info *captureDevices;
  ma_uint32 captureDeviceCount;

  audioMixer->getCaptureDevices(&captureDevices, &captureDeviceCount);

  Napi::Array array = Napi::Array::New(info.Env(), captureDeviceCount);

  for (int i = 0; i < captureDeviceCount; i++)
  {
    ma_device_info device = captureDevices[i];
    Napi::Object deviceObj = Napi::Object::New(env);
    deviceObj.Set("id", Napi::Number::New(env, i));
    deviceObj.Set("name", Napi::String::New(env, device.name));

    array.Set(i, deviceObj);
  }

  return array;
}

Napi::Value API::addInputDevice(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  int index = info[0].As<Napi::Number>().Int32Value();

  audioMixer->addInputDevice(index);

  return env.Undefined();
}

Napi::Value API::getUpdates(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Function cb = info[0].As<Napi::Function>();

  worker->start(env, cb);

  return env.Undefined();
}
