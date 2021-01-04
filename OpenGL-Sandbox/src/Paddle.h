#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include "GameObject.h"

using namespace GLCore;
using namespace GLCore::Utils;

class Paddle : GameObject
{
public:
    Paddle(float direction, float speed, glm::vec2 size, glm::vec2 position, glm::vec4 color);
    ~Paddle();
    void Draw();
    void Update(Timestep ts);
    void Activate() { m_Active = true; };
    void Reset();
    glm::vec2 GetSize()     const { return m_Size; };
    glm::vec2 GetPosition() const { return m_Position; };

private:
    bool m_Active;
    float m_Direction;
    float m_Speed;
    glm::vec2 m_Size;
    glm::vec2 m_Position;
    glm::vec4 m_Color;
    
};
