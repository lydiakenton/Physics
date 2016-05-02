#ifndef GROUNDPLANE_H__
#define GROUNDPLANE_H__

#include "Shape.h"

//!Derived class of Shape
class GroundPlane : public Shape
{
public:
  //! Ground plane contructor
  /*!
   * \brief GroundPlane
   * \param _id ID number of the ground plane
   * \param _yPos Position of the plane on the y axis
   * \param _mat Material to be used on the ground plane
   * \param _physics Pointer to Physics class
   */
  GroundPlane(int _id, ngl::Real _yPos, ngl::Material _mat, Physics *_physics);
  //! Draws the ground plane
  void draw();
private :
  //! Position on the y axis
  ngl::Real m_yPos;
};

#endif // GROUNDPLANE_H__
