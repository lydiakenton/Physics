#include "Capsule.h"

Capsule::Capsule(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat,  Physics *_physics, bool _static) :
  Shape(_id, _mat, _physics, _static)
{
  m_rad = _rad;
  m_height = _height;
  m_scaleMat.scale(_rad, _height, _rad);
}

void Capsule::draw()
{
  m_mat.loadToShader("material");
  ngl::VAOPrimitives::instance()->draw("capsule");
}
