//
// Created by Adarion on 2022/11/17.
//

#include "JsManager.h"


#include "../../../../Function/UI/Core/Base/UIObject.h"
#include "../../../../Function/UI/Core/Component/UIText.h"
#include "../../../../Function/UI/Core/Component/UIImage.h"
#include "../../../../Function/UI/Component/UIButton.h"


using namespace Erisu::Scripts;
using namespace Erisu::Function;
using namespace Eigen;


namespace
{
    void DestroyComponent(UIComponent* component)
    {
        auto ptr = std::dynamic_pointer_cast<IUIComponent>(component->shared_from_this());
        UIObject::RemoveUIComponent(ptr);
    }

    int __REG__UI__ = []() {
        auto &js = JsManager::GetInstance();

        js.RegisterFunction("DestroyComponent", DestroyComponent);

        js.RegisterConstructor<UIComponent, std::string, int>("UIComponent");           // UIComponent(name, priority)
        js.RegisterConstructor<UIImage, std::string, std::string, int>("UIImage");      // UIImage(name, path, priority)
        js.RegisterConstructor<UIText, std::string, std::string, int>("UIText");        // UIText(name, text, priority)
        js.RegisterConstructor<UIButton, std::string, int>("UIButton");                 // UIButton(name, priority)
        js.RegisterConstructor<UIContainer, std::string, int, float, float>("UIContainer");  // UIContainer(name, priority)
        js.RegisterConstructor<UIInputJs, UIComponent*>("UIInput"); // UIInput()

        js.RegisterInheritance<UIComponent, UIImage>();
        js.RegisterInheritance<UIComponent, UIText>();
        js.RegisterInheritance<UIImage, UIButton>();
        js.RegisterInheritance<UIComponent, UIContainer>();

        // UIComponent Methods
        js.RegisterMethod("SetPosition", &UIComponent::SetPositionJs);       // SetPosition(x, y)
        js.RegisterMethod("SetSize", &UIComponent::SetSizeJs);               // SetSize(width, height)
        js.RegisterMethod("SetScale", &UIComponent::SetScaleJs);           // SetScale(scaleX, scaleY)
        js.RegisterMethod("SetColor", &UIComponent::SetColorJs);           // SetColor(r, g, b, a)
        js.RegisterMethod("SetRect", &UIComponent::SetRectJs);             // SetRect(x, y, width, height)
        js.RegisterMethod("SetEnabled", &UIComponent::SetEnabledJs);         // SetEnabled(enabled)
        js.RegisterMethod("SetOnScreenPosition", &UIComponent::SetOnScreenPositionJs); // SetOnScreenPosition(x, y)
        js.RegisterMethod("SetRotation", &UIComponent::SetRotation);       // SetRotation(rotation)
        js.RegisterMethod("SetVisible", &UIComponent::SetVisible);         // SetVisible(visible)
        js.RegisterMethod("SetParent", &UIComponent::SetParent);           // SetParent(parent)
        js.RegisterMethod("SetPriority", &UIComponent::SetPriority);       // SetPriority(priority)
        js.RegisterMethod("SetBlendMode", &UIComponent::SetBlendModeJs);     // SetBlendMode(blendMode)

        js.RegisterMethod("GetPosition", &UIComponent::GetPositionJs);       // GetPosition()
        js.RegisterMethod("GetSize", &UIComponent::GetSizeJs);               // GetSize()
        js.RegisterMethod("GetScale", &UIComponent::GetScaleJs);           // GetScale()
        js.RegisterMethod("GetColor", &UIComponent::GetColorJs);           // GetColor()
        js.RegisterMethod("GetRect", &UIComponent::GetRectJs);             // GetRect()
        js.RegisterMethod("GetScreenPosition", &UIComponent::GetScreenPositionJs); // GetOnScreenPosition()

        js.RegisterMethod("IsPointInRect", &UIComponent::IsPointInRectJs); // IsPointInRect(x, y)
        js.RegisterMethod("IsVisible", &UIComponent::IsVisible);           // IsVisible()
        js.RegisterMethod("SetAnchor", &UIComponent::SetAnchorJs);           // SetAnchor(anchor)
        js.RegisterMethod("GetAnchor", &UIComponent::GetAnchorJs);           // GetAnchor()
        js.RegisterMethod("GetAnchorPosition", &UIComponent::GetAnchorOffsetJs); // GetAnchorPosition()

        js.RegisterMethod("AddToUIObject", &UIComponent::AddToUIObjectJs);   // AddToUIObject(uiObject)

        // UIImage Methods
        js.RegisterMethod("SetTexture", &UIImage::SetTextureJs);           // SetTexture(path)
        js.RegisterMethod("SetShader", &UIImage::SetShaderJs);             // SetShader(shader)
        js.RegisterMethod("SetShaderFromFile", &UIImage::SetShaderFromFileJs); // SetShaderFromFile(path)

        // UIText Methods
        js.RegisterMethod("SetText", &UIText::SetText);                     // SetText(text)
        js.RegisterMethod("SetFontSize", &UIText::SetFontSize);             // SetFontSize(size)
        js.RegisterMethod("SetLineSpacing", &UIText::SetLineSpacing);       // SetLineSpacing(spacing)
        js.RegisterMethod("SetCharacterSpacing", &UIText::SetLetterSpacing); // SetCharacterSpacing(spacing)
        js.RegisterMethod("SetOutline", &UIText::SetOutline);
        js.RegisterMethod("SetColor", &UIText::SetColorJs);
        js.RegisterMethod("SetOutlineColor", &UIText::SetOutlineColorJs);
        js.RegisterMethod("SetOutlineSize", &UIText::SetOutlineThickness);
        js.RegisterMethod("SetShader", &UIText::SetShaderJs);
        js.RegisterMethod("SetShaderFromFile", &UIText::SetShaderFromFileJs);
        js.RegisterMethod("SetOutlineShader", &UIText::SetOutlineShaderJs);
        js.RegisterMethod("SetOutlineShaderFromFile", &UIText::SetOutlineShaderFromFileJs);
        js.RegisterMethod("SetFontPath", &UIText::SetFontPath);

        // UIContainer Methods
        js.RegisterMethod("AddComponent", &UIContainer::AddComponentJs);             // AddChild(child)
        js.RegisterMethod("RemoveComponent", &UIContainer::RemoveComponentJs);       // RemoveChild(child)
        js.RegisterMethod<UIContainer, void, float, float, float, float>("SetRect", &UIContainer::SetRect);                       // SetRect(x, y, width, height)
        js.RegisterMethod("SetSize", &UIContainer::SetSize);                       // SetSize(width, height)

        // UIInput
        js.RegisterMethod("SetOnClick", &UIInputJs::SetOnClick);               // SetOnClicked(function)
        js.RegisterMethod("SetOnHoverEnter", &UIInputJs::SetOnHoverEnter);     // SetOnHoverEnter(function)
        js.RegisterMethod("SetOnHoverExit", &UIInputJs::SetOnHoverExit);       // SetOnHoverExit(function)
        js.RegisterMethod("SetOnPressed", &UIInputJs::SetOnPressed);           // SetOnPressed(function)
        js.RegisterMethod("SetOnPressedExit", &UIInputJs::SetOnPressedExit);   // SetOnPressedExit(function)
        js.RegisterMethod("SetOnDrag", &UIInputJs::SetOnDrag);                 // SetOnDrag(function)
        js.RegisterMethod("SetOnDragExit", &UIInputJs::SetOnDragExit);         // SetOnDragExit(function)
        js.RegisterMethod("SetOnDragStart", &UIInputJs::SetOnDragStart);       // SetOnDragEnter(function)
        js.RegisterMethod("SetLayer", &UIInputJs::SetLayer);                   // SetLayer(layer)
        js.RegisterMethod("SetShared", &UIInputJs::SetShared);                  // SetShare(share)
        js.RegisterMethod("GetBeforeDragPosition", &UIInputJs::GetBeforeDragPosition); // GetBeforeDragPosition()
        js.RegisterMethod("GetLastMousePosition", &UIInputJs::GetLastMousePosition);   // GetDragPosition()

        // UIButton
        js.RegisterMethod("SetOnClick", &UIButton::SetOnClickJs);             // SetOnClicked(function)
        js.RegisterMethod<UIButton, void, const std::string&>("SetNormalTexture", &UIButton::SetNormalTexture); // SetNormalTexture(path)
        js.RegisterMethod<UIButton, void, const std::string&>("SetHoverTexture", &UIButton::SetHoverTexture);   // SetHoverTexture(path)
        js.RegisterMethod<UIButton, void, const std::string&>("SetPressedTexture", &UIButton::SetPressedTexture); // SetPressedTexture(path)
        js.RegisterMethod("SetLayer", &UIButton::SetLayer);                   // SetLayer(layer)

        return 0;
    }();

}