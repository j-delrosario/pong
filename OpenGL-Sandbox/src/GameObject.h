#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>

using namespace GLCore;
using namespace GLCore::Utils;

class GameObject
{
public:
    virtual void Update(Timestep ts) {};
    virtual void Draw() {};
};