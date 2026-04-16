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
  setOutputDevice(2);
}

void AudioMixer::stopEngine()
{
  ma_device_stop(&outputDevice);
  ma_device_uninit(&outputDevice);
}

void AudioMixer::outputCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  float *out = (float *)pOutput;
  ma_uint32 channels = pDevice->playback.channels;

  // clear output
  for (ma_uint32 i = 0; i < frameCount * channels; i++)
    out[i] = 0.0f;

  std::map<int, InputLine> inputDevices = getInstance().inputDevices;

  // mix all inputs
  for (const auto &[key, value] : inputDevices)
  {
    {
      auto &buf = value.buffer;
      float vol = value.volume;

      for (ma_uint32 i = 0; i < frameCount * channels && i < buf.size(); i++)
        out[i] += buf[i] * powf(10.0f, vol / 20.0f);
    }

    // clip
    for (ma_uint32 i = 0; i < frameCount * channels; i++)
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

void AudioMixer::addInputDevice(int index)
{
  if (inputDevices.find(index) != inputDevices.end())
  {
    printf("Device already added\n");
    return;
  }

  ma_device_info *captureDevices;
  ma_uint32 captureDeviceCount;

  getCaptureDevices(&captureDevices, &captureDeviceCount);

  if (index >= captureDeviceCount)
  {
    printf("Output Device Not Found\n");
    return;
  }

  ma_device_id outputDeviceId = captureDevices[index].id;

  inputDevices.emplace(index, InputLine());

  InputLine *inputLine = &inputDevices[index];

  inputLine->buffer.resize(48000 * 2);

  ma_device_config captureConfig = ma_device_config_init(ma_device_type_capture);
  captureConfig.capture.pDeviceID = &outputDeviceId;
  captureConfig.capture.format = ma_format_f32;
  captureConfig.capture.channels = 2;
  captureConfig.sampleRate = 48000;
  captureConfig.dataCallback = inputCallback;
  captureConfig.pUserData = &inputLine->buffer;

  result = ma_device_init(&context, &captureConfig, &inputLine->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to init capture device\n");
    stopEngine();
    return;
  }

  result = ma_device_start(&inputLine->device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to start capture device\n");
    stopEngine();
    return;
  }
}

void AudioMixer::removeInputDevice(int index)
{
  inputDevices.erase(index);
}