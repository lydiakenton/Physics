#include "Shape.h"
#include <iostream>

Shape::Shape(int _id, ngl::Material _mat, Physics *_physics, bool _static)
{
  m_id = _id;
  m_physics = _physics;
  m_mat = _mat;
  m_static = _static;
}

ngl::Mat4 Shape::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);

  // ensures that the scaling of shapes is performed correctly
  ngl::Mat4 transformation = m_scaleMat * transRot;

  return transformation;
}

void Shape::push(ngl::Vec3 m_dir)
{
  m_physics->push(m_id, m_dir);
}
