#include "Sphere.h"


Sphere::Sphere(int _id, ngl::Real _rad, ngl::Material _mat, Physics *_physics, bool _static) :
  Shape(_id, _mat,_physics, _static)
{
  m_rad = _rad;
  m_scaleMat.scale(_rad, _rad, _rad);
}

void Sphere::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("sphere");
}

