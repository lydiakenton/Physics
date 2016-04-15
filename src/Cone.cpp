#include "Cone.h"

Cone::Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics) :
  Shape(_id, _mat, _physics)
{
  m_rad = _rad;
  m_height = _height;
}

void Cone::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("cone");
}

ngl::Mat4 Cone::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);
  //transRot.scale();
  return transRot;
}
