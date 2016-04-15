#ifndef CUBE_H__
#define CUBE_H__

#include "Shape.h"

class Cube : public Shape
{
public:
  Cube(int _id, ngl::Vec3 _size, ngl::Material _mat, Physics *_physics);
  void draw(const std::string &_shader);
  ngl::Mat4 getTransformMatrix();
private :
  ngl::Vec3 m_size;
};

#endif
