#include "Cone.h"

Cone::Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics, bool _static) :
  Shape(_id, _mat, _physics,_static)
{
  m_rad = _rad;
  m_height = _height;
  m_scaleMat.scale(_rad, _height, _rad);

}

void Cone::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("cone");
}
