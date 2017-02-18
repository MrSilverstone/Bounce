#pragma once

#include <psp2/types.h>
#include <psp2/ctrl.h>
#include "Component.hpp"



class LogicScript : public Component
{
public:
  LogicScript(GameObject *gameObject) : Component(gameObject) {}
};
