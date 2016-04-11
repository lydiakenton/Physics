#ifndef SHAPE_H__
#define SHAPE_H__

#include <ngl/Mat4.h>
#include <vector>
#include <ngl/Vec4.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <unordered_map>

class Physics;

class Shape
{
public:

  Shape();

  static Shape *instance();
  int getShapeID();
  int isStatic();
  ngl::Mat4 getTransformMatrix();
  void setColour();

private:

  unsigned int m_id;
  btCollisionShape *m_shapes;
  ngl::Vec4 m_colour;

  Physics *m_physics;
};

#endif
