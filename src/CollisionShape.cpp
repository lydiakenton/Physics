#include "CollisionShape.h"
#include <ngl/Obj.h>

namespace shape
{
  CollisionShape *shape::CollisionShape::instance()
  {
    static CollisionShape s_instance;

    return &s_instance;
  }

  void CollisionShape::addSphere(const std::string &_name, ngl::Real _rad)
  {
    btCollisionShape* sphere = new btSphereShape(btScalar(_rad));
    m_shapes[_name].reset(sphere);
  }

  void CollisionShape::addCone(const std::string &_name, ngl::Real _rad, ngl::Real _height)
  {
    btCollisionShape* cone = new btConeShape(btScalar(_rad),btScalar(_height));
    m_shapes[_name].reset(cone);
  }

  void CollisionShape::addCube(const std::string &_name, ngl::Vec3 _size)
  {
    btCollisionShape* cube = new btBoxShape(btVector3(_size.m_x,_size.m_y,_size.m_z));
    m_shapes[_name].reset(cube);
  }

  void CollisionShape::addPlatform(const std::string &_name, ngl::Vec3 _size)
  {
    btCollisionShape* platform = new btBoxShape(btVector3(_size.m_x,_size.m_y,_size.m_z));
    m_shapes[_name].reset(platform);
  }

  btCollisionShape* CollisionShape::getShape(const std::string &_name)
  {
    btCollisionShape *shape=nullptr;
    auto shapeIt=m_shapes.find(_name);
      if(shapeIt!=m_shapes.end())
      {
        shape=shapeIt->second.get();
      }
    return shape;
  }
}
