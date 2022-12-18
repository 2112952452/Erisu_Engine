//
// Created by Adarion on 2022/11/30.
//


#include "JsManager.h"
#include "../../../Function/Audio/AudioManager.h"
namespace
{
    using namespace Erisu::Scripts;
    using namespace Erisu::Function;

    void playAudioJs(const std::string &path, float volume, float pitch)
    {
        AudioManager::GetInstance().Play(path, volume, pitch);
    }

    void playAudioJs(const std::string &path)
    {
        AudioManager::GetInstance().Play(path);
    }

    void playAudioJs(Audio* audio)
    {
        auto audioPtr = std::shared_ptr<Audio>(audio);
        AudioManager::GetInstance().Play(audioPtr);
    }

    int REG_ = [] {
        auto &js = JsManager::GetInstance();

        js.RegisterConstructor<Audio, const std::string&>("Audio");

        js.RegisterMethod("Pause", &Audio::Pause);
        js.RegisterMethod("Resume", &Audio::Resume);
        js.RegisterMethod("Stop", &Audio::Stop);
        js.RegisterMethod("ForceStop", &Audio::ForceStop);
        js.RegisterMethod("SetLoop", &Audio::SetLoop);
        js.RegisterMethod("SetVolume", &Audio::SetVolume);
        js.RegisterMethod("SetPitch", &Audio::SetPitch);
        js.RegisterMethod("SetCurrentTime", &Audio::SetCurrentTime);
        js.RegisterMethod("GetDuration", &Audio::GetDuration);
        js.RegisterMethod("Reload", &Audio::Reload);
        // Is it necessary to expose this method?
        //js.RegisterMethod("Unload", &Audio::Unload);

        js.RegisterFunction<void, const std::string&, float, float>("PlayAudio", playAudioJs);
        js.RegisterFunction<void, const std::string&>("PlayAudio", playAudioJs);
        js.RegisterFunction<void, Audio*>("PlayAudio", playAudioJs);

        return 0;
    }();

}