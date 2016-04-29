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

void PhysicsLib::addCone(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height)
{
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = ngl::PI*_rad*_rad*(_height/3);
  }
  int coneIndex = m_physics.addCone(_pos,mass,_static,_rad,_height);
  //m_cone.push_back(std::unique_ptr<Cone>(new Cone(coneIndex, _rad, _height, m_currentMat, &m_physics, _static)));
  m_shapes.push_back(std::unique_ptr<Cone>(new Cone(coneIndex, _rad, _height, m_currentMat, &m_physics, _static)));
}

void PhysicsLib::addCapsule(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height)
{
  // mass = volume of sphere + volume of cylinder
  ngl::Real mass = 0;
  if(!_static)
  {
   mass = ((4/3)* ngl::PI *_rad*_rad*_rad) + (ngl::PI *_rad*_rad*_height);
  }
  int capsuleIndex = m_physics.addCapsule(_pos,mass,_static,_rad,_height);
  m_shapes.push_back(std::unique_ptr<Capsule>(new Capsule(capsuleIndex,_rad,_height, m_currentMat, &m_physics, _static)));
}

void PhysicsLib::addCube(ngl::Vec3 _pos, bool _static, ngl::Vec3 _size)
{
  // divide by 2 so that ngl and bullet agree about size
  ngl::Vec3 collisionSize = _size/2;
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = _size.m_x*_size.m_y*_size.m_z;
  }
  int cubeIndex = m_physics.addCube(_pos,mass,_static,collisionSize);
  m_shapes.push_back(std::unique_ptr<Cube>(new Cube(cubeIndex,_size, m_currentMat, &m_physics, _static)));
}

void PhysicsLib::addSphere(ngl::Vec3 _pos, bool _static, ngl::Real _rad)
{
  ngl::Real mass = 0;
  if(!_static)
  {
    mass = (4/3) * ngl::PI *_rad*_rad*_rad;
  }
  int sphereIndex = m_physics.addSphere(_pos,mass,_static,_rad);
  m_shapes.push_back(std::unique_ptr<Sphere>(new Sphere(sphereIndex,_rad, m_currentMat, &m_physics, _static)));
}

void PhysicsLib::step(float _time, float _step)
{
  m_physics.step(_time,_step);
}

ngl::Mat4 PhysicsLib::getShapeTransformMatrix(int _shapeIndex)
{
  for(int i=0; i<m_shapes.size(); i++)
  {
    if(i==CONE_SHAPE_PROXYTYPE)
      return m_shapes[_shapeIndex]->getTransformMatrix();
  }
}

ngl::Mat4 PhysicsLib::getConeTransformMatrix(int _shapeIndex)
{
  //return m_cone[coneIndex]->fixCone();
  return m_shapes[_shapeIndex]->fixCone();
}

void PhysicsLib::drawShape(int _shapeIndex, const std::string &_shader)
{
  return m_shapes[_shapeIndex]->draw(_shader);
}

void PhysicsLib::drawGroundPlane(const std::string &_shader)
{
  m_groundPlane->draw(_shader);
}

//void PhysicsLib::drawCone(int _coneIndex, const std::string &_shader)
//{
//  return m_cone[_coneIndex]->draw(_shader);
//}

int PhysicsLib::getNumOfShapes()
{
  return m_shapes.size();
}

//int PhysicsLib::getNumOfCones()
//{
//  return m_cone.size();
//}

void PhysicsLib::setMaterial(ngl::Material _mat)
{
  m_currentMat = _mat;
}

void PhysicsLib::moveLeft(int _shapeIndex)
{
  return m_shapes[_shapeIndex]->moveLeft();
}

void PhysicsLib::moveRight(int _shapeIndex)
{
  return m_shapes[_shapeIndex]->moveRight();
}

bool PhysicsLib::collision()
{
  bool collision;
  for(int i=40; i<m_shapes.size(); i++)
  {
    std::cout<<"hello"<<std::endl;
    collision = m_physics.isCollision(i);
  }
  return collision;
}

void PhysicsLib::reset()
{
  m_shapes.erase(m_shapes.begin()+41,m_shapes.end());
}
