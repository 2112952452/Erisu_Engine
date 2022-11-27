//
// Created by Adarion on 2022/11/19.
//

#include <JsManager.h>
#include <../../../Function/Animation/Timeline/Timeline.h>
#include "../../../Function/UI/Core/Manager/UIAnimationManager.h"
#include "../../../../Function/Animation/AnimationCurve.h"
#include "../../../../Function/Animation/Javascript/PositionClipJs.h"
#include "../../../../Function/Animation/Javascript/ColorClipJs.h"

namespace
{
    using namespace Erisu::Scripts;
    using namespace Erisu::Function;

    void SetOnGameStartJs(const std::string &func)
    {
        UIAnimationManager::OnStart = [func] {
            JsManager::GetInstance().Execute(func.c_str());
        };
    }

    int __REG__ = [] {
        auto &js = JsManager::GetInstance();

        js.RegisterFunction("SetOnGameStart", SetOnGameStartJs);

        js.RegisterConstructor<AnimationCurve, const std::string&>("AnimationCurve");
        js.RegisterConstructor<Timeline>("Timeline");
        js.RegisterConstructor<PositionClipJs, float, UIComponent*, Vector2*, Vector2*, AnimationCurve*>("PositionClip");
        js.RegisterConstructor<ColorClipJs, float, UIComponent*, Vector4*, Vector4*, AnimationCurve*>("ColorClip");

        js.RegisterInheritance<IClip, Timeline>();
        js.RegisterInheritance<IClip, PositionClipJs>();
        js.RegisterInheritance<IClip, ColorClipJs>();

        js.RegisterMethod("SetOnStarted", &Timeline::SetOnStartedJs);
        js.RegisterMethod("SetOnFinished", &Timeline::SetOnFinishedJs);
        js.RegisterMethod("AddClip", &Timeline::AddClipJs);
        js.RegisterMethod("Activate", &Timeline::ActivateJs);
        js.RegisterMethod("Play", &IClip::Play);
        js.RegisterMethod("Pause", &IClip::Pause);
        js.RegisterMethod("Resume", &IClip::Resume);
        js.RegisterMethod("Stop", &IClip::Stop);
        js.RegisterMethod("ForceStop", &IClip::ForceStop);

        return 0;
    }();



}
