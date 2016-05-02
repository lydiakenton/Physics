#ifndef CUBE_H__
#define CUBE_H__

#include "Shape.h"

//! Derived class of Shape
class Cube : public Shape
{
public:
  //! Cube constructor
  /*!
   * \brief Cube
   * \param _id ID number of the shape
   * \param _size Size of the cube
   * \param _mat Material to be used on the cube
   * \param _physics Pointer to Physics class
   * \param _static Dynamics of the shape
   */
  Cube(int _id, ngl::Vec3 _size, ngl::Material _mat, Physics *_physics, bool _static);
  //! Draws the cube
  void draw();
private :
  //! Size of the cube (l, w, h)
  ngl::Vec3 m_size;
};

#endif // CUBE_H__
