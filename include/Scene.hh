#pragma once

#include <psp2/types.h>
#include <psp2/ctrl.h>
#include <vector>
#include "AssetsManager.hh"
#include "ScreenClearer.hh"
#include "ShaderFactory.hh"
#include "Context.hh"
#include "GameObject.hpp"
#include "Camera.hh"
#include "ScriptSystem.hh"

class Scene
{
public:
  Scene(Context & context, ShaderFactory & factory);
  ~Scene();
  bool loadRessources();

  void setup();
  void update(const SceCtrlData & pad, float totalT, float elapsedT);
  void draw();
  
private:
  void	clearScreen();
  GameObject *addGameObject(const std::string & name)
  {
    GameObject *go = new GameObject(name);
    _gameObjects.push_back(go);
    return go;
  }
  
private:
  Context		& _context;
  ShaderFactory		& _shaderFactory;
  AssetsManager		_assetsManager;
  ScreenClearer		_screenClearer;

  Camera*		_camera;
  
  ScriptSystem		_scriptSystem;
  std::vector<GameObject*> _gameObjects;
};
