// Made by Donovan Colen
#pragma once
#include "Dots.h"

class Fruit : public Dots
{
private:

public:
    Fruit() = default;
    virtual int Eat() override;
    void Disable(); // disables the fruit because it doesnt start spawned
};