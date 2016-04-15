#ifndef SPHERE_H__
#define SPHERE_H__

#include "Shape.h"

class Sphere : public Shape
{
public:
  Sphere(int _id, ngl::Real _rad, ngl::Material _mat, Physics *_physics);
  void draw(const std::string &_shader);
  ngl::Mat4 getTransformMatrix();
private :
  ngl::Real m_rad;
};

#endif
