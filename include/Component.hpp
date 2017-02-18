#pragma once

#include <psp2/types.h>
#include <psp2/ctrl.h>

class GameObject;

class Component
{
public:
  Component(GameObject* gameObject) :
    _gameObject(gameObject)
  {

  }
  
  virtual void setup() = 0;
  virtual void update(float deltaT, const SceCtrlData & pad) = 0;

  void enable()
  {
    _enabled = true;
  }
  
  void disable()
  {
    _enabled = true;
  }
  
  bool isEnabled() const
  {
    return _enabled;
  }
    
protected:
  bool _enabled = true;
  GameObject *_gameObject;
};
