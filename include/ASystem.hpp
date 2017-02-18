#pragma once

#include <list>
#include "Component.hpp"

class ASystem
{
public:
  virtual void setup() = 0;
  virtual void update(float deltaT, const SceCtrlData & pad) = 0;
  void registerComponent(Component *c)
  {
    _components.push_back(c);
  }
  
protected:
  std::list<Component*> _components;
};
