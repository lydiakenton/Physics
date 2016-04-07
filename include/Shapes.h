#ifndef SHAPES_H__
#define SHAPES_H__

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <string>
#include <ngl/Types.h>
#include <ngl/Vec3.h>
#include <unordered_map>


class Shapes
{
public:
  static Shapes *instance();
  void addSphere(const std::string &_name, ngl::Real _rad);
  void addCone(const std::string &_name, ngl::Real _rad, ngl::Real _height);
  void addCube(const std::string &_name, ngl::Vec3 _size);
  void addStaticCube(const std::string &_name, ngl::Vec3 _size);

  btCollisionShape* getShape(const std::string &_name);

private:
  Shapes(){}

  std::unordered_map <std::string, std::unique_ptr<btCollisionShape>> m_shapes;
};

#endif
