#include "Shape.h"
#include  "Physics.h"

Shape::Shape()
{
  m_id =m_physics->getNumCollisionObjects();
}

Shape *Shape::instance()
{
  static Shape s_instance;
  return &s_instance;
}

ngl::Mat4 Shape::getTransformMatrix()
{
  return m_physics->getTransformMatrix(m_id);
}

int Shape::isStatic()
{
  return m_physics->isStatic(m_id);
}

int Shape::getShapeID()
{
  return m_physics->getCollisionShape(m_id);
}

void Shape::setColour()
{

}
