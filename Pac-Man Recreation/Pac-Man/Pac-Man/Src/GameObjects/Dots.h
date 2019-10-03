// Made by Donovan Colen
#pragma once
#include <string>
#include <Application/Application.h>


class Dots : public GameObject
{
protected:
    bool m_eatten;
public:
    Dots();
    virtual ~Dots() = default;
    void Init(int x, int y, int w, int h, float speed, TextureEvent textureEvent, std::string name);
    virtual int Eat();  // returns the score to add when eatten
    void Reset();
};