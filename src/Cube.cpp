#include "Cube.h"

Cube::Cube(int _id, ngl::Vec3 _size, ngl::Material _mat, Physics *_physics, bool _static):
  Shape(_id, _mat, _physics,_static)
{
  m_size = _size;
  m_scaleMat.scale(_size.m_x, _size.m_y, _size.m_z);
}

void Cube::draw()
{
  m_mat.loadToShader("material");
  ngl::VAOPrimitives::instance()->draw("cube");
}
