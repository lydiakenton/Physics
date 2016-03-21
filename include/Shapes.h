#ifndef SHAPES_H__
#define SHAPES_H__

#include <btBulletDynamicsCommon.h>
#include <ngl/Types.h>
#include <memory>
#include <string>
#include <unordered_map>

class Shapes
{
public:
  static Shapes *instance();
  void addSphere(const std::string &_name, ngl::Real _rad);

  btCollisionShape* getShape(const std::string &_name);

private:
  Shapes(){}

  std::unordered_map < std::string,std::unique_ptr <btCollisionShape> >m_shapes;
};

#endif
