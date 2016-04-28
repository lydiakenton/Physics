#ifndef SHAPE_H__
#define SHAPE_H__

#include <ngl/Mat4.h>
#include <vector>
#include <ngl/Vec4.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include "Physics.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>

class Shape
{
public:

  Shape(int _id, ngl::Material _mat, Physics *_physics, bool _static);

  virtual void draw(const std::string &_shader);

  int getShapeID();
  bool isStatic() {return m_static;}
  virtual ngl::Mat4 getTransformMatrix();
  void moveLeft();
  void moveRight();
  //void setColour();
  //int getShapeType() const {return m_shapeType;}

protected:

  unsigned int m_id;
  ngl::Material m_mat;

  ngl::Mat4 m_scaleMat;

  bool m_static;

  Physics *m_physics;
};

#endif
