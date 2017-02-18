#include "ScriptSystem.hh"

void ScriptSystem::setup()
{
  for (auto s : _components)
    {
      s->setup();
    }
}

void ScriptSystem::update(float deltaT, const SceCtrlData & pad)
{
  for (auto s : _components)
    {
      s->update(deltaT, pad);
    }
}
