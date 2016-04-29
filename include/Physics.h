#ifndef PHYSICS_H__
#define PHYSICS_H__

#include <vector>
#include <btBulletDynamicsCommon.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Obj.h>
#include <memory>

class Physics
{
public:
  Physics();
  ~Physics();

  void setGravity(float _x, float _y, float _z)
  {
    m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
  }
  int addGroundPlane(ngl::Real _yPos);
  int addSphere(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad);
  int addCone(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad, ngl::Real _height);
  int addCube(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Vec3 _size);
  int addCapsule(ngl::Vec3 _pos, ngl::Real _mass,bool _isStatic, ngl::Real _rad, ngl::Real _height);
  void step(float _time, float _step);

  int addRigidBodyToDW(btRigidBody* _rigidBody);
  bool isCollision(unsigned int _index);
  //void move(unsigned int _index, ngl::Vec3 _dir);
  void moveLeft(unsigned int _index);
  void moveRight(unsigned int _index);
  void moveToOrigin(unsigned int _index);

  unsigned int getNumCollisionObjects()const
  {
    return m_dynamicsWorld->getNumCollisionObjects();
  }

  int getCollisionShape(unsigned int _index) const;
  int isStatic(unsigned int _index);
  ngl::Mat4 getTransformMatrix(unsigned int _index);

private:

 std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
 std::unique_ptr<btCollisionShape> m_groundShape;
 std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
 std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache;
 std::unique_ptr<btCollisionDispatcher> m_dispatcher;
 std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;

};


#endif
