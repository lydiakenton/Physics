#include "Sphere.h"


Sphere::Sphere(int _id, ngl::Real _rad, ngl::Material _mat, Physics *_physics) :
  Shape(_id, _mat,_physics)
{
  m_rad = _rad;
}

void Sphere::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("sphere");
}

ngl::Mat4 Sphere::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);
  //transRot.scale();
  return transRot;
}
