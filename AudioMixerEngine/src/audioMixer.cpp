#include "audioMixer.h"

AudioMixer &AudioMixer::getInstance()
{
  static AudioMixer instance;
  return instance;
}

AudioMixer::AudioMixer() {}

int AudioMixer::initMiniaudio()
{
  result = ma_context_init(NULL, 0, NULL, &context);
  if (result != MA_SUCCESS)
    return -1;

  return 0;
}

void AudioMixer::startEngine()
{
  stopEngine();
  setOutputDevice(5);
}

void AudioMixer::stopEngine()
{
  ma_device_stop(&outputDevice);
  ma_device_uninit(&outputDevice);

  for (auto &ch : channels)
  {
    Channel *channel = ch.get();
    ma_device_stop(&channel->device);
    ma_device_uninit(&channel->device);
  }
}

bool AudioMixer::getDeviceByName(std::string name, ma_device_id &deviceId, std::string &deviceName)
{
  ma_device_info *captureDevices;
  ma_uint32 captureDeviceCount;

  getCaptureDevices(&captureDevices, &captureDeviceCount);

  for (int i = 0; i < captureDeviceCount; i++)
  {
    if (captureDevices[i].name == name)
    {
      deviceId = captureDevices[i].id;
      deviceName = captureDevices[i].name;
      return true;
    }
  }

  return false;
}

std::string AudioMixer::getDeviceName(ma_device_id id)
{
  ma_device_info *captureDevices;
  ma_uint32 captureDeviceCount;

  getCaptureDevices(&captureDevices, &captureDeviceCount);

  for (int i = 0; i < captureDeviceCount; i++)
  {
    if (ma_device_id_equal(&captureDevices[i].id, &id))
    {
      return captureDevices[i].name;
    }
  }
  return "Unknown Device";
}

void AudioMixer::outputCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  float *out = (float *)pOutput;
  ma_uint32 deviceChannels = pDevice->playback.channels;

  // clear output
  for (ma_uint32 i = 0; i < frameCount * deviceChannels; i++)
    out[i] = 0.0f;

  // mix all inputs
  for (const auto &channel : getInstance().channels)
  {
    {
      auto &buf = channel->buffer;
      float vol = channel->volume;

      for (ma_uint32 i = 0; i < frameCount * deviceChannels && i < buf.size(); i++)
      {
        // out[i] += buf[i] * powf(10.0f, vol / 20.0f);
        out[i] += buf[i] * vol;
      }
    }

    // clip
    for (ma_uint32 i = 0; i < frameCount * deviceChannels; i++)
    {
      if (out[i] > 1.0f)
        out[i] = 1.0f;
      if (out[i] < -1.0f)
        out[i] = -1.0f;
    }
  }
}

void AudioMixer::inputCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  (void)pOutput;

  auto *buf = (std::vector<float> *)pDevice->pUserData;
  if (buf == nullptr || pInput == nullptr)
  {
    return;
  }

  float *in = (float *)pInput;
  buf->assign(in, in + frameCount * pDevice->capture.channels);
}

int AudioMixer::getPlaybackDevices(ma_device_info **playbackDevices, ma_uint32 *playbackDeviceCount)
{
  return ma_context_get_devices(&context, playbackDevices, playbackDeviceCount, nullptr, nullptr);
}

int AudioMixer::getCaptureDevices(ma_device_info **captureDevices, ma_uint32 *captureDeviceCount)
{
  return ma_context_get_devices(&context, nullptr, nullptr, captureDevices, captureDeviceCount);
}

void AudioMixer::setOutputDevice(int index)
{
  ma_device_info *playbackDevices;
  ma_uint32 playbackDeviceCount;

  getPlaybackDevices(&playbackDevices, &playbackDeviceCount);

  if (index >= playbackDeviceCount)
  {
    printf("Output Device Not Found\n");
    return;
  }

  ma_device_id outputDeviceId = playbackDevices[index].id;

  ma_device_config playbackConfig = ma_device_config_init(ma_device_type_playback);
  playbackConfig.playback.pDeviceID = &outputDeviceId;
  playbackConfig.playback.format = ma_format_f32;
  playbackConfig.playback.channels = 2;
  playbackConfig.sampleRate = 48000;
  playbackConfig.dataCallback = outputCallback;

  result = ma_device_init(&context, &playbackConfig, &outputDevice);

  if (result != MA_SUCCESS)
  {
    printf("Failed to init output device\n");
    return;
  }

  result = ma_device_start(&outputDevice);
  if (result != MA_SUCCESS)
  {
    printf("Failed to start output device\n");
    stopEngine();
    return;
  }
}

void AudioMixer::addChannel(std::string name)
{
  auto ch = std::make_unique<Channel>();
  Channel *channel = ch.get();

  channel->name = name;
  channel->volume = 1;

  ma_device_info *captureDevices;
  ma_uint32 captureDeviceCount;

  getCaptureDevices(&captureDevices, &captureDeviceCount);

  ma_device_id deviceId = captureDevices[2].id;

  channel->buffer.resize(48000 * 2);
  channel->deviceName = captureDevices[2].name;

  ma_device_config captureConfig = ma_device_config_init(ma_device_type_capture);
  captureConfig.capture.pDeviceID = &deviceId;
  captureConfig.capture.format = ma_format_f32;
  captureConfig.capture.channels = 2;
  captureConfig.sampleRate = 48000;
  captureConfig.dataCallback = inputCallback;
  captureConfig.pUserData = &channel->buffer;

  result = ma_device_init(&context, &captureConfig, &channel->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to init capture device\n");
    stopEngine();
    return;
  }

  result = ma_device_start(&channel->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to start capture device\n");
    stopEngine();
    return;
  }

  channels.push_back(std::move(ch));
}

void AudioMixer::setChannelDevice(int index, std::string name)
{
  auto &ch = channels[index];
  Channel *channel = ch.get();

  ma_device_id deviceId;
  std::string deviceName;

  if (!getDeviceByName(name, deviceId, deviceName))
  {
    printf("Err: Device Not Found");
    return;
  }

  if (&channel->device != nullptr)
  {
    ma_device_stop(&channel->device);
    ma_device_uninit(&channel->device);
  }

  channel->deviceName = deviceName;

  ma_device_config captureConfig = ma_device_config_init(ma_device_type_capture);
  captureConfig.capture.pDeviceID = &deviceId;
  captureConfig.capture.format = ma_format_f32;
  captureConfig.capture.channels = 2;
  captureConfig.sampleRate = 48000;
  captureConfig.dataCallback = inputCallback;
  captureConfig.pUserData = &channel->buffer;

  result = ma_device_init(&context, &captureConfig, &channel->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to init capture device\n");
    stopEngine();
    return;
  }

  result = ma_device_start(&channel->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to start capture device\n");
    stopEngine();
    return;
  }
}

void AudioMixer::setChannelVolume(int index, float volume)
{
  auto &ch = channels[index];
  Channel *channel = ch.get();
  channel->volume = volume;
}

std::vector<Channel *> AudioMixer::getChannels()
{
  std::vector<Channel *> result;

  for (auto &ch : channels)
    result.push_back(ch.get());

  return result;
}

void AudioMixer::removeChannel(int index)
{
  channels.erase(channels.begin() + index);
}

std::vector<RMSData> AudioMixer::getRMSLevels()
{
  std::vector<RMSData> lines = std::vector<RMSData>();

  for (const auto &channel : channels)
  {
    RMSData data = RMSData();

    data.left = 0.0f;
    data.right = 0.0f;

    if (!channel->buffer.empty())
    {
      double sumL = 0.0;
      double sumR = 0.0;

      const auto &buf = channel->buffer;
      size_t frameCount = buf.size() / 2;

      for (size_t i = 0; i < frameCount; i++)
      {
        float l = buf[i * 2 + 0];
        float r = buf[i * 2 + 1];

        sumL += l * l;
        sumR += r * r;
      }

      float rmsL = std::sqrt(sumL / frameCount);
      float rmsR = std::sqrt(sumR / frameCount);

      // → dB
      float dbL = 20.0f * log10f(rmsL + 1e-6f);
      float dbR = 20.0f * log10f(rmsR + 1e-6f);

      float minDb = -60.0f;
      float maxDb = 0.0f;

      float normL = (dbL - minDb) / (maxDb - minDb);
      float normR = (dbR - minDb) / (maxDb - minDb);

      normL *= channel->volume;
      normR *= channel->volume;

      data.left = std::clamp(normL, 0.0f, 1.0f);
      data.right = std::clamp(normR, 0.0f, 1.0f);
    }

    lines.push_back(data);
  }
  return lines;
}
