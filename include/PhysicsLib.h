#ifndef PHYSICSLIB_H__
#define PHYSICSLIB_H__

#include <ngl/Singleton.h>
#include "Physics.h"
#include "Shape.h"
#include "Capsule.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cube.h"
#include "GroundPlane.h"
#include "Player.h"

class PhysicsLib : public ngl::Singleton<PhysicsLib>
{
friend class ngl::Singleton<PhysicsLib>;

public :
  void init();
  void setGravity(float _x,float _y,float _z);

  const std::vector<std::unique_ptr<Shape>>& getShapes(){return m_shapes;}
  //const std::vector<std::unique_ptr<Cone>>& getCones(){return m_cone;}

  void addGroundPlane(ngl::Real _yPos);
  void addCube(ngl::Vec3 _pos, bool _static, ngl::Vec3 _size);
  void addSphere(ngl::Vec3 _pos, bool _static, ngl::Real _rad);
  void addCapsule(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height);
  void addCone(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height);

  ngl::Mat4 getShapeTransformMatrix(int _shapeIndex);
  ngl::Mat4 getConeTransformMatrix(int _shapeIndex);

  void drawShape(int _shapeIndex, const std::string &_shader);
  int getNumOfShapes();
  //int getNumOfCones();

  void drawGroundPlane(const std::string &_shader);
  //void drawCone(int _coneIndex, const std::string &_shader);
  void step(float _time, float _step);

  void setMaterial(ngl::Material _mat);

  void reset();
  void moveLeft(int _shapeIndex);
  void moveRight(int _shapeIndex);

  bool collision();

private :
  PhysicsLib();
  ~PhysicsLib();

  Physics m_physics;

  std::unique_ptr<GroundPlane> m_groundPlane;

  std::vector<std::unique_ptr<Shape>> m_shapes;
  //std::vector<std::unique_ptr<Cone>> m_cone;

  ngl::Material m_currentMat;
};

#endif
