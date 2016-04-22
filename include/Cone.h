#ifndef CONE_H__
#define CONE_H__

#include "Shape.h"

class Cone : public Shape
{
public:
  Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics * _physics, bool _static);
  void draw(const std::string &_shader);
private :
  ngl::Real m_rad;
  ngl::Real m_height;
};

#endif
