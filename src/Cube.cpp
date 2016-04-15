#include "Cube.h"

Cube::Cube(int _id, ngl::Vec3 _size, ngl::Material _mat, Physics *_physics):
  Shape(_id, _mat, _physics)
{
  m_size = _size;
}

void Cube::draw(const std::string &_shader)
{
  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("cube");
}

ngl::Mat4 Cube::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);
  //transRot.scale();
  return transRot;
}
