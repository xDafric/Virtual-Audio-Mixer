#include "worker.h"

Worker &Worker::getInstance()
{
  static Worker instance;
  return instance;
}

Worker::Worker()
{
  running = false;
}

Worker::~Worker()
{
  stop();
}

void Worker::start(Napi::Env env, Napi::Function callback)
{
  callbackFunction = Napi::ThreadSafeFunction::New(
      env,
      callback,
      "AudioStreamer",
      0,
      1);

  running = true;
  workerThread = std::thread(&Worker::run, this);
}

void Worker::stop()
{
  if (running)
  {
    running = false;

    if (workerThread.joinable())
    {
      workerThread.join();
    }

    callbackFunction.Release();
  }
}

void Worker::run()
{
  while (running)
  {
    callbackFunction.BlockingCall([](Napi::Env env, Napi::Function jsCallback)
                                  {
                          
      AudioMixer& audioMixer = AudioMixer::getInstance();
      std::vector<RMSData> data = audioMixer.getRMSLevels();
      std::vector<Channel*> channels = audioMixer.getChannels(); 
      Napi::Array array = Napi::Array::New(env, data.size());

      for (int i = 0; i < data.size(); i++) {
        RMSData line = data[i];
        Channel* channel = channels[i];
        Napi::Object lineObj = Napi::Object::New(env);

        lineObj.Set("name", channel->name);
        lineObj.Set("volume", channel->volume);
        lineObj.Set("left", line.left);
        lineObj.Set("right", line.right);
        lineObj.Set("device", channel->deviceName);
        array.Set(i, lineObj);
      }

      jsCallback.Call({array}); });

    std::this_thread::sleep_for(std::chrono::microseconds(16));
  }
}