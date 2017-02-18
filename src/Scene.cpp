#include <stdexcept>
#include "Scene.hh"

Scene::Scene(Context & context, ShaderFactory & factory) :
  _context(context), _shaderFactory(factory)
{
  if (!loadRessources())
    throw new std::runtime_error("Error while loading ressources");

  if (!_screenClearer.initialize(_context.gxmContext(), _shaderFactory.shaderPatcher()))
    throw new std::runtime_error("Error while initializing screen clearer");
}

Scene::~Scene()
{
  _screenClearer.release(_shaderFactory.shaderPatcher());
  _assetsManager.release();
}

bool Scene::loadRessources()
{
    Shader lightShader;
      
    _assetsManager.addShader("light", lightShader);
  if (!_shaderFactory.loadShader(_assetsManager.getShader("light"),
			   "app0:res/Shaders/light_v.gxp",
			   "app0:res/Shaders/light_f.gxp"))
    return false;

  _assetsManager.getShader("light").addUniform(VERTEX, "wvp");
  _assetsManager.getShader("light").addUniform(VERTEX, "normalMatrix");
  
  Shader basicShader;

  _assetsManager.addShader("color", basicShader);
  if (!_shaderFactory.loadShader(_assetsManager.getShader("color"),
			   "app0:res/Shaders/color_v.gxp",
			   "app0:res/Shaders/color_f.gxp"))
    return false;

  _assetsManager.getShader("color").addUniform(FRAGMENT, "u_color");
  _assetsManager.getShader("color").addUniform(VERTEX, "wvp");

  if (!_assetsManager.addMesh("app0:res/Mesh/sphere.obj"))
    return false;
  /*
  if (!_assetsManager.addMesh("app0:res/Mesh/car/chassis.obj"))
    return false;
  if (!_assetsManager.addMesh("app0:res/Mesh/car/wheel.obj"))
    return false;
  */  

  return true;
}

void Scene::clearScreen()
{
  _screenClearer.clear(_context.gxmContext());
}

void Scene::setup()
{  
  auto camObj = addGameObject("Main Camera");

  _camera = camObj->addComponent<Camera>();
  _scriptSystem.registerComponent(_camera);

  _scriptSystem.setup();
}

void   Scene::update(const SceCtrlData & pad, float totalT, float elapsedT)
{
  _scriptSystem.update(elapsedT, pad);
}

void   Scene::draw()
{
  clearScreen();

  Shader & colorShader = _assetsManager.getShader("color");

  glm::mat4 wvp = _camera->getProjectionMatrix() * _camera->getViewMatrix();
  
  colorShader.bind(_context.gxmContext());
  colorShader.setUniformMat4(VERTEX, "wvp", _context.gxmContext(), wvp);
  colorShader.setUniformVec4(FRAGMENT, "u_color", _context.gxmContext(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

  _assetsManager.getMesh("Sphere").draw(_context.gxmContext());
}
