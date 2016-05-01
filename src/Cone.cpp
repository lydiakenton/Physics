#include "Cone.h"

Cone::Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics, bool _static) :
  Shape(_id, _mat, _physics,_static)
{
  m_rad = _rad;
  m_height = _height;
  m_scaleMat.scale(_rad, _height, _rad);

}

void Cone::draw()
{
  m_mat.loadToShader("material");
  ngl::VAOPrimitives::instance()->draw("cone");
}

ngl::Mat4 Cone::getTransformMatrix()
{
  ngl::Mat4 transformation = m_physics->getTransformMatrix(m_id);

  // translate and rotate
  ngl::Mat4 offsetTransMatrix;
  offsetTransMatrix.translate(0.0f, -m_height/2.0f, 0.0f);
  transformation = offsetTransMatrix * transformation;

  // scale
  transformation = m_scaleMat * transformation;

  // offset rotation, so ngl and bt agree about rotation
  ngl::Mat4 offsetRotMatrix;
  offsetRotMatrix.rotateX(-90);
  transformation = offsetRotMatrix * transformation;

  return transformation;
}
