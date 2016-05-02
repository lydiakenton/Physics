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

//! Base class of individual shapes
class Shape
{
public:
  //! Shape constructor
  /*!
   * \brief Shape
   * \param _id ID number of the shape
   * \param _mat Material to be used on the shape
   * \param _physics Pointer to Physics class
   * \param _static Dynamics of the shape
   */
  Shape(int _id, ngl::Material _mat, Physics *_physics, bool _static);
  //! Draws the shapes
  virtual void draw() = 0;
  //! Returns the ID number for each shape
  int getShapeID() {return m_id;}
  //! Returns true and shape is static
  bool isStatic() {return m_static;}
  //! Returns the transformation matrix of the shapes
  virtual ngl::Mat4 getTransformMatrix();
  //! Moves the shapes in a given direction
  void push(ngl::Vec3 _dir);

protected:
  //! ID number of the shape
  unsigned int m_id;
  //! Material of the shape
  ngl::Material m_mat;
  //! Scale matrix
  ngl::Mat4 m_scaleMat;
  //! Dynamics boolean
  bool m_static;
  //! Pointer to Physics class
  Physics *m_physics;
};

#endif //SHAPE_H__
