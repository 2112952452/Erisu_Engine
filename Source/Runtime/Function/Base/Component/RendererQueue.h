//
// Created by Adarion on 2022/10/9.
//

#ifndef ERISU_ENGINE_RENDERERQUEUE_H
#define ERISU_ENGINE_RENDERERQUEUE_H

#include <map>
#include "ISortableComponent.h"

namespace Erisu::Function
{
    /// O(log n) insert and O(n) render \n
    /// Priority more higher, render more front \n
    /// Could not add same priority renderer \n TODO: Support same priority renderer
    class RendererQueue : public IComponent
    {
    private:
        friend class GalTextBox;
        using RendererPtr = std::shared_ptr<ISortableComponent>;

    private:
        std::map<int, RendererPtr> renderers_;

    protected:
        void SetGameObject(const std::shared_ptr<GameObject> &gameObject) override;
        void SetScene(const std::shared_ptr<Scene> &scene) override;

        void UpdateSubComponents(); // update all sub components' scene and game object
    public:
        explicit RendererQueue(const std::string &name);
        ~RendererQueue() override;

        void AddRenderer(const RendererPtr &renderer);
        void RemoveRenderer(const RendererPtr &renderer);
        void RemoveRenderer(const std::string &name);
        void RemoveRenderer(int priority);
        void Clear();

        RendererPtr& GetRenderer(int priority);
        std::map<int, RendererPtr>& GetAllRenderers();

        void Update() override;
        void Render() override;
        void Destroy() override;

        void ShowInInspector() override;
    };

}

#endif //ERISU_ENGINE_RENDERERQUEUE_H
