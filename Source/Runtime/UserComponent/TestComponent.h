//
// Created by Adarion on 2022/10/9.
//

#ifndef ERISU_ENGINE_TESTCOMPONENT_H
#define ERISU_ENGINE_TESTCOMPONENT_H

#include <IComponent.h>
#include <Timer/Timer.h>

class TestComponent : public Erisu::Function::IComponent
{
public:
    explicit TestComponent(const std::string &name);
    ~TestComponent() override;

    void Update() override;
    void Render() override;
private:
    Erisu::Core::Timer timer_;
};

#endif //ERISU_ENGINE_TESTCOMPONENT_H
