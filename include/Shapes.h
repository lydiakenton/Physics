#ifndef SHAPES_H__
#define SHAPES_H__

#include <btBulletDynamicsCommon.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <ngl/Types.h>


class Shapes
{
public:
  static Shapes *instance();
  void addSphere(const std::string &_name, ngl::Real _rad);

  btCollisionShape* getShape(const std::string &_name);

private:
  Shapes(){}

  Shapes(const Shapes &_c)=delete;
  Shapes & operator=(const Shapes &_c)=delete;
  std::unordered_map < std::string,std::unique_ptr <btCollisionShape> >m_shapes;
};

#endif
