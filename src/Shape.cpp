#include "Shape.h"
#include <iostream>

Shape::Shape(int _id, ngl::Material _mat, Physics *_physics)
{
  m_id = _id;
  m_physics = _physics;
  m_mat = _mat;
}

void Shape::draw(const std::string &_shader)
{
  std::cout << "WARNING: default draw call, shape type unknown" << std::endl;
}

ngl::Mat4 Shape::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);
  return transRot;
}
