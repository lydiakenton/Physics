#include "Shapes.h"
#include <ngl/Obj.h>

Shapes *Shapes::instance()
{
  static Shapes s_instance;

  return &s_instance;
}

void Shapes::addSphere(const std::string &_name, ngl::Real _rad)
{
  btCollisionShape* sphere = new btSphereShape(btScalar(_rad/2.0));
  m_shapes[_name].reset(shape);
}

btCollisionShape* Shape::getShape(const std::string &_name)
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
