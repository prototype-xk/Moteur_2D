#pragma once
#include<iostream>


class Camera{
private:
    float m_screenWidth, m_screenHeight;
    float m_x, m_y;

public:
    Camera(float screenWidth, float screenHeight)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight),
          m_x(0.0f), m_y(0.0) {}

    void update(float playerX, float playerY) {
        m_x = playerX - m_screenWidth * 0.5f;
        m_y = playerY - m_screenHeight * 0.5f;
    }

    float getX() const { return m_x; }
    float getY() const { return m_y; }
};

