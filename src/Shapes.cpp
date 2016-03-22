#include "Shapes.h"
#include <ngl/Obj.h>

Shapes *Shapes::instance()
{
  static Shapes s_instance;

  return &s_instance;
}

void Shapes::addSphere(const std::string &_name, ngl::Real _rad)
{

  btCollisionShape* shape = new btSphereShape(btScalar(_rad));
  m_shapes[_name].reset(shape);
  //m_bodies.push_back(sphere);

}

btCollisionShape* Shapes::getShape(const std::string &_name)
{
  btCollisionShape* shape = nullptr;
  auto shapeFind = m_shapes.find(_name);

  //ensure there's a valid shader
  if(shapeFind!=m_shapes.end())
  {
    shape=shapeFind->second.get();
  }
  return shape;
}
