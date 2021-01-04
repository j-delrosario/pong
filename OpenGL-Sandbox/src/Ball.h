#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GameObject.h"

using namespace GLCore;
using namespace GLCore::Utils;

class Ball : GameObject
{
public:
    Ball(glm::vec2 velocity, float speed, glm::vec2 size, glm::vec2 position, glm::vec4 color);
    ~Ball();
    void Start();
    void Update(Timestep ts);
    void CheckCollision();
    void Draw();
    void Reset();

    float     GetSpeed()    const   { return m_Speed; };
    glm::vec2 GetVelocity() const   { return m_Velocity; };
    glm::vec2 GetSize()     const   { return m_Size; };
    glm::vec2 GetPosition() const   { return m_Position; };
    glm::vec2 GetColor()    const   { return m_Color; };
    int8_t    HasScored()   const   { return m_Scored; };

    void ChangeVelocity(glm::vec2 velocity) { m_Velocity = velocity; };

private:
    float m_Speed;
    glm::vec2 m_Velocity;
    glm::vec2 m_Size;
    glm::vec2 m_Position;
    glm::vec4 m_Color;
    int8_t m_Scored = 0;
};
