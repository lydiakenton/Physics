#include "Shape.h"
#include <iostream>

Shape::Shape(int _id, ngl::Material _mat, Physics *_physics, bool _static)
{
  m_id = _id;
  m_physics = _physics;
  m_mat = _mat;
  m_static = _static;
}

void Shape::draw(const std::string &_shader)
{
  std::cout << "WARNING: default draw call, shape type unknown" << std::endl;
}

ngl::Mat4 Shape::getTransformMatrix()
{
  ngl::Mat4 transRot = m_physics->getTransformMatrix(m_id);

  ngl::Mat4 transformation = m_scaleMat * transRot;

  return transformation;
}

ngl::Mat4 Shape::fixCone()
{
  ngl::Mat4 transform = m_physics->getTransformMatrix(m_id);
  ngl::Mat4 coneRotateMatrix;
  ngl::Mat4 coneTranslateMatrix;

  coneTranslateMatrix.translate(0.0f,0.0f,-0.5f);
  coneRotateMatrix.rotateX(-90);
  transform = coneRotateMatrix * transform;
  transform = coneTranslateMatrix * transform;

  return transform;
}

void Shape::moveLeft()
{
  m_physics->moveLeft(m_id);
}

void Shape::moveRight()
{
  m_physics->moveRight(m_id);
}
