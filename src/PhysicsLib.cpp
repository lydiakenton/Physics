#include "PhysicsLib.h"

#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>

PhysicsLib::PhysicsLib()
{
}

PhysicsLib::~PhysicsLib()
{
}

void PhysicsLib::init()
{
  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  prim->createLineGrid("plane",50.0f,50.0f,40.0f);
  prim->createSphere("sphere",1.0f,40.0f);
  prim->createCone("cone",1.0f,1.0f,20.0f,20.0f);
  prim->createCapsule("capsule",1.0f,1.0f,40.0f);

  m_currentMat = ngl::STDMAT::BLACKPLASTIC;
  setGravity(0,-9.81,0);
  addGroundPlane(0);
}

void PhysicsLib::setGravity(float _x, float _y, float _z)
{
  m_physics.setGravity(_x,_y,_z);
}

void PhysicsLib::addGroundPlane(ngl::Real _yPos)
{
  int groundPlaneIndex = m_physics.addGroundPlane(_yPos);
  m_groundPlane.reset(new GroundPlane(groundPlaneIndex, _yPos, m_currentMat, &m_physics));
}

int PhysicsLib::addCone(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height)
{
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = ngl::PI*_rad*_rad*(_height/3);
  }
  int coneID = m_physics.addCone(_pos,mass,_static,_rad,_height);
  m_shapes.push_back(std::unique_ptr<Cone>(new Cone(coneID, _rad, _height, m_currentMat, &m_physics, _static)));
  int coneIndex = m_shapes.size()-1;
  return coneIndex;
}

int PhysicsLib::addCapsule(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height)
{
  // mass = volume of sphere + volume of cylinder
  ngl::Real mass = 0;
  if(!_static)
  {
   mass = ((4/3)* ngl::PI *_rad*_rad*_rad) + (ngl::PI *_rad*_rad*_height);
  }
  int capsuleID = m_physics.addCapsule(_pos,mass,_static,_rad,_height);
  m_shapes.push_back(std::unique_ptr<Capsule>(new Capsule(capsuleID,_rad,_height, m_currentMat, &m_physics, _static)));
  int capsuleIndex = m_shapes.size()-1;
  return capsuleIndex;
}

int PhysicsLib::addCube(ngl::Vec3 _pos, bool _static, ngl::Vec3 _size)
{
  // divide by 2 so that ngl and bullet agree about size
  ngl::Vec3 collisionSize = _size/2;
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = _size.m_x*_size.m_y*_size.m_z;
  }
  int cubeID = m_physics.addCube(_pos,mass,_static,collisionSize);
  m_shapes.push_back(std::unique_ptr<Cube>(new Cube(cubeID,_size, m_currentMat, &m_physics, _static)));
  int cubeIndex = m_shapes.size()-1;
  return cubeIndex;
}

int PhysicsLib::addSphere(ngl::Vec3 _pos, bool _static, ngl::Real _rad)
{
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = (4/3) * ngl::PI *_rad*_rad*_rad;
  }
  int sphereID = m_physics.addSphere(_pos,mass,_static,_rad);
  m_shapes.push_back(std::unique_ptr<Sphere>(new Sphere(sphereID,_rad, m_currentMat, &m_physics, _static)));
  int sphereIndex = m_shapes.size()-1;
  return sphereIndex;
}

void PhysicsLib::step(float _time, float _step)
{
  m_physics.step(_time,_step);
}

ngl::Mat4 PhysicsLib::getShapeTransformMatrix(int _shapeIndex)
{
  return m_shapes[_shapeIndex]->getTransformMatrix();
}

void PhysicsLib::drawShape(int _shapeIndex)
{
  m_shapes[_shapeIndex]->draw();
}

void PhysicsLib::drawGroundPlane()
{
  m_groundPlane->draw();
}

int PhysicsLib::getNumOfShapes()
{
  return m_shapes.size();
}

void PhysicsLib::setMaterial(ngl::Material _mat)
{
  m_currentMat = _mat;
}

void PhysicsLib::push(int _shapeIndex, ngl::Vec3 _dir)
{
  return m_shapes[_shapeIndex]->push(_dir);
}

//void PhysicsLib::removeShape(int _shapeIndex)
//{
//  m_physics.deleteBody(m_shapes[_shapeIndex]->getShapeID());
//  m_shapes.erase(m_shapes.begin()+_shapeIndex);
//  for(int i=_shapeIndex; i<m_shapes.size(); i++)
//  {
//    m_shapes[i]->decrementID();
//  }
//}

//reset()
//{
  //m_physics = Physics();
  //m_shapes.resize(0);
//}









