#include "GroundPlane.h"

GroundPlane::GroundPlane(int _id, ngl::Real _yPos, ngl::Material _mat, Physics *_physics) :
  Shape(_id, _mat, _physics, true)
{
  m_yPos = _yPos;
}

void GroundPlane::draw()
{
  m_mat.loadToShader("material");
  ngl::VAOPrimitives::instance()->draw("plane");
}
