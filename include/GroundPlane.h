#ifndef GROUNDPLANE_H__
#define GROUNDPLANE_H__

#include "Shape.h"

class GroundPlane : public Shape
{
public:
  GroundPlane(int _id, ngl::Real _yPos, ngl::Material _mat, Physics *_physics);
  void draw();
private :
  ngl::Real m_yPos;
};

#endif
