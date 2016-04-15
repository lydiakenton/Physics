#include "Capsule.h"

Capsule::Capsule(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat,  Physics *_physics) :
  Shape(_id, _mat, _physics)
{
  m_rad = _rad;
  m_height = _height;
}

void Capsule::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("capsule");
}

ngl::Mat4 Capsule::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);
  //transRot.scale();
  return transRot;
}
