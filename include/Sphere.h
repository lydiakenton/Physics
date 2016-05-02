#ifndef SPHERE_H__
#define SPHERE_H__

#include "Shape.h"

//! Derived class of Shape
class Sphere : public Shape
{
public:
  //! Sphere constructor
  /*!
   * \brief Sphere
   * \param _id ID number of the shape
   * \param _rad Radius of the sphere
   * \param _mat Material to be used on the sphere
   * \param _physics Pointer to Physics class
   * \param _static Dynamics of the shape
   */
  Sphere(int _id, ngl::Real _rad, ngl::Material _mat, Physics *_physics, bool _static);
  //! Draws the sphere
  void draw();
private :
  //! Radius of the sphere
  ngl::Real m_rad;
};

#endif // SPHERE_H__
