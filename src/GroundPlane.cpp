#include "GroundPlane.h"

GroundPlane::GroundPlane(int _id, ngl::Real _yPos, ngl::Material _mat, Physics *_physics) :
  Shape(_id, _mat, _physics, true)
{
  m_yPos = _yPos;
}

void GroundPlane::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("plane");
}
