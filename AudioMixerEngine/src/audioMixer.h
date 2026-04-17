#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "miniaudio.h"

struct InputLine
{
  ma_device device;
  std::vector<float> buffer;
  float volume;
};

struct RMSData
{
  int lineId;
  float left;
  float right;
};

class AudioMixer
{
private:
  ma_result result;
  ma_context context;

  ma_device outputDevice;
  std::map<int, InputLine> inputDevices;

  AudioMixer();

  static void inputCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount);
  static void outputCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount);

public:
  static AudioMixer &getInstance();

  AudioMixer(const AudioMixer &) = delete;
  AudioMixer &operator=(const AudioMixer &) = delete;

  int initMiniaudio();
  void startEngine();
  void stopEngine();

  int getPlaybackDevices(ma_device_info **playbackDevices, ma_uint32 *playbackDeviceCount);
  int getCaptureDevices(ma_device_info **captureDevices, ma_uint32 *captureDeviceCount);

  void setOutputDevice(int index);
  void addInputDevice(int index);
  void removeInputDevice(int index);

  std::vector<RMSData> getRMSLevels();
};
