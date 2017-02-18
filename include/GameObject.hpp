#pragma once

#include <list>
#include <string>
#include <glm/glm.hpp>
#include "Component.hpp"

struct Transform
{
  glm::vec3	position;
  glm::vec3	rotation;
  glm::vec3	scale;
};

class GameObject
{
public:
  GameObject(const std::string & name) : _name(name)
  { }
  
  ~GameObject()
  {
    for (auto component : _components)
      {
	delete component;
      }
    _components.clear();
  }
  
  template<typename T>
  T* addComponent()
  {
    T *component = new T(this);
    _components.push_back(component);
    return component;
  }
  
  template<typename T>
  void removeComponent()
  {

  }
  
  template<typename T>
  T* getComponent()
  {
    for (auto component : _components)
      {
	auto s = dynamic_cast<T*>(component);
	if (s)
	  return s;
      }  
    return nullptr;
  }
  
  std::string getName() const
  {
    return _name;
  }

  Transform & getTransform()
  {
    return _transform;
  }
  
private:
  std::list<Component*> _components;
  std::string _name;
  Transform _transform;
};
