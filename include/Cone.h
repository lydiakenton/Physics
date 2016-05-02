#ifndef CONE_H__
#define CONE_H__

#include "Shape.h"

//! Derived class of Shape
class Cone : public Shape
{
public:
  //! Cone constructor
  /*!
   * \brief Cone
   * \param _id ID number of the shape
   * \param _rad Radius of the cone
   * \param _height Height of the cone
   * \param _mat Material to be used on the cone
   * \param _physics Pointer to Physics class
   * \param _static Dynamics of the shape
   */
  Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics * _physics, bool _static);
  //! Draws the cone
  void draw();
  //! Gets the transformation Matrix of the cones
  //! Fixes rotation and translation so that NGL and Bullet agree on how cones are drawn
  ngl::Mat4 getTransformMatrix();
private :
  //! Radius of the cone
  ngl::Real m_rad;
  //! Height of the cone
  ngl::Real m_height;
};

#endif // CONE_H__
