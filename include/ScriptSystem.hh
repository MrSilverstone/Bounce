#pragma once

#include "ASystem.hpp"
#include "LogicScript.hpp"

class ScriptSystem : public ASystem
{
public:
  void setup() override;
  void update(float deltaT, const SceCtrlData & pad) override;    
};
