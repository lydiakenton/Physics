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

  void reset();
  void setGravity(float _x, float _y, float _z)
  {
    m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
  }
  void addGroundPlane(const std::string &_name, const ngl::Vec3 &_pos);
  void addSphere(const std::string &_shapeName, const ngl::Vec3 &_pos);
  void addCone(const std::string &_shapeName, const ngl::Vec3 &_pos);
  void addCube(const std::string &_shapeName, const ngl::Vec3 &_pos);
  void step(float _time, float _step);

  inline unsigned int getNumCollisionObjects()const
  {
    return m_dynamicsWorld->getNumCollisionObjects();
  }

  int getCollisionShape(unsigned int _index) const;
  ngl::Mat4 getTransformMatrix(unsigned int _index);

private:
  typedef struct
  {
    std::string name;
    btRigidBody* body;
  }Body;

 //btBroadphaseInterface* m_broadphase;
 std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
 std::unique_ptr<btCollisionShape> m_groundShape;
 std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
 std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache;
 std::unique_ptr<btCollisionDispatcher> m_dispatcher;
 std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
 std::vector<Body> m_bodies;


};


#endif
