#ifndef CAPSULE_H__
#define CAPSULE_H__

#include "Shape.h"

//! Derived class of Shape
class Capsule : public Shape
{
public:
  //! Capsule constructor
  /*!
   * \brief Capsule
   * \param _id ID number of the shape
   * \param _rad Radius of the capsule
   * \param _height Height of the capsule
   * \param _mat Material to be used on the capsule
   * \param _physics Pointer to Physics class
   * \param _static Dynamics of the shape
   */
  Capsule(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics, bool _static);
  //! Draws the capsule
  void draw();
private :
  //! Radius of the capsule
  ngl::Real m_rad;
  //! Height of the capsule
  ngl::Real m_height;
};

#endif // CAPSULE_H__
