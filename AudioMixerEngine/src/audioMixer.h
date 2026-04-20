#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "miniaudio.h"

struct Channel
{
  std::string name;
  ma_device device;
  std::string deviceName;
  std::vector<float> buffer;
  float volume;
};

struct RMSData
{
  float left;
  float right;
};

class AudioMixer
{
private:
  ma_result result;
  ma_context context;

  ma_device outputDevice;
  std::vector<std::unique_ptr<Channel>> channels;

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

  bool getDeviceByName(std::string name, ma_device_id &deviceId, std::string &deviceName);
  std::string getDeviceName(ma_device_id id);

  int getPlaybackDevices(ma_device_info **playbackDevices, ma_uint32 *playbackDeviceCount);
  int getCaptureDevices(ma_device_info **captureDevices, ma_uint32 *captureDeviceCount);

  void setOutputDevice(int index);

  void addChannel(std::string name);

  void setChannelDevice(int index, std::string name);
  // void setChannelName(int index, std::string channelName);
  void setChannelVolume(int index, float volume);

  std::vector<Channel *> getChannels();

  void removeChannel(int index);

  std::vector<RMSData> getRMSLevels();
};
