#ifndef SPHERE_H__
#define SPHERE_H__

#include "Shape.h"

class Sphere : public Shape
{
public:
  Sphere(int _id, ngl::Real _rad, ngl::Material _mat, Physics *_physics, bool _static);
  void draw();
private :
  ngl::Real m_rad;
};

#endif
