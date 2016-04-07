#include "Shapes.h"
#include <ngl/Obj.h>

Shapes *Shapes::instance()
{
  static Shapes s_instance;

  return &s_instance;
}

void Shapes::addSphere(const std::string &_name, ngl::Real _rad)
{
  btCollisionShape* sphere = new btSphereShape(btScalar(_rad));
  m_shapes[_name].reset(sphere);
}

void Shapes::addCone(const std::string &_name, ngl::Real _rad, ngl::Real _height)
{
  btCollisionShape* cone = new btConeShape(btScalar(_rad),btScalar(_height));
  m_shapes[_name].reset(cone);
}

void Shapes::addCube(const std::string &_name, ngl::Vec3 _size)
{
  btCollisionShape* cube = new btBoxShape(btVector3(_size.m_x,_size.m_y,_size.m_z));
  m_shapes[_name].reset(cube);
}

void Shapes::addStaticCube(const std::string &_name, ngl::Vec3 _size)
{
  btCollisionShape* staticCube = new btBoxShape(btVector3(_size.m_x,_size.m_y,_size.m_z));
  m_shapes[_name].reset(staticCube);
}

btCollisionShape* Shapes::getShape(const std::string &_name)
{
  btCollisionShape *shape=nullptr;
  auto shapeIt=m_shapes.find(_name);
    if(shapeIt!=m_shapes.end())
    {
      shape=shapeIt->second.get();
    }
  return shape;
}
