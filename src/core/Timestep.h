#pragma once

class Timestep
{
public:
    Timestep(float time = 0.0f)
        : m_Time(time)
    {}
    float operator/= (float value) { m_Time /= value; return m_Time; }
    operator float() const { return m_Time; }
    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000.f; }
private:
    float m_Time;
};