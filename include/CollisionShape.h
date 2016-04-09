#ifndef COLLISIONSHAPE_H__
#define COLLISIONSHAPE_H__

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <string>
#include <ngl/Vec3.h>
#include <unordered_map>

namespace shape
{
  class CollisionShape
  {
  public:
    static CollisionShape *instance();
    void addSphere(const std::string &_name, ngl::Real _rad);
    void addCone(const std::string &_name, ngl::Real _rad, ngl::Real _height);
    void addCube(const std::string &_name, ngl::Vec3 _size);
    void addCapsule(const std::string &_name, ngl::Real _rad, ngl::Real _height);
    void addPlatform(const std::string &_name, ngl::Vec3 _size);

    btCollisionShape* getShape(const std::string &_name);

  private:
    CollisionShape(){}

    std::unordered_map <std::string, std::unique_ptr<btCollisionShape>> m_shapes;
  };
}

#endif
