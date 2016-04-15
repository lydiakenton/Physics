#ifndef CAPSULE_H__
#define CAPSULE_H__

#include "Shape.h"

class Capsule : public Shape
{
public:
  Capsule(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics);
  void draw(const std::string &_shader);
  ngl::Mat4 getTransformMatrix();
private :
  ngl::Real m_rad;
  ngl::Real m_height;
};

#endif
