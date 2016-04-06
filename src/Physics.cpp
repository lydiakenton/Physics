#include "Physics.h"
//#include "Shapes.h"
#include <iostream>
#include <ngl/Obj.h>
#include <ngl/Quaternion.h>
#include <ngl/Util.h>


Physics::Physics()
{
  //default setup for memory
  m_collisionConfiguration.reset(new btDefaultCollisionConfiguration());
  //default collision dispatcher
  m_dispatcher.reset(new btCollisionDispatcher(m_collisionConfiguration.get()));
  //general purpose broadphase
  m_overlappingPairCache.reset(new btDbvtBroadphase());
  //default constraint sovler
  m_solver.reset(new btSequentialImpulseConstraintSolver());

  m_dynamicsWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(),
                                                    m_overlappingPairCache.get(),
                                                    m_solver.get(),
                                                    m_collisionConfiguration.get()));
}

Physics::~Physics()
{

}

void Physics::addSphere(const std::string & _shapeName, const ngl::Vec3 &_pos)
{
  //creating dynamic rigid body - sphere
  btCollisionShape* colShape = new btSphereShape(btScalar(0.1));

  //create dynamic objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar mass(2);

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(mass,inertia);

  //motionstate provides interpolation capabilities
  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, colShape, inertia);
  rigidBodyCI.m_friction = 0.25f;
  rigidBodyCI.m_restitution = 0.75f;

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setLinearVelocity(btVector3(0,1,0));
  rigidBody->applyImpulse(btVector3(0,3,0), btVector3(0,1,0));
  rigidBody->setRollingFriction(btScalar(0.75f));

  m_dynamicsWorld->addRigidBody(rigidBody);
  Body s;
  s.name=_shapeName;
  s.body=rigidBody;
  m_bodies.push_back(s);
}

void Physics::addCone(const std::string &_shapeName, const ngl::Vec3 &_pos)
{
  btCollisionShape* colShape = new btConeShape(btScalar(0.5f), btScalar(0.25f));

  btTransform startTransform;
  startTransform.setIdentity();
  btScalar mass(2);

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, colShape, inertia);
  rigidBodyCI.m_friction=0.2f;

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.3f);
  rigidBody->setRollingFriction(0.25f);

  m_dynamicsWorld->addRigidBody(rigidBody);
  Body s;
  s.name=_shapeName;
  s.body=rigidBody;
  m_bodies.push_back(s);
}

void Physics::addCube(const std::string &_shapeName, const ngl::Vec3 &_pos, const btScalar &_mass, const ngl::Vec3 &_size)
{
  btCollisionShape* colShape = new btBoxShape(btVector3(_size.m_x, _size.m_y, _size.m_z));

  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.5f);

  m_dynamicsWorld->addRigidBody(rigidBody);
  Body s;
  s.name=_shapeName;
  s.body=rigidBody;
  m_bodies.push_back(s);
}

void Physics::addStaticCube(const std::string &_shapeName, const ngl::Vec3 &_pos, const btScalar &_mass, const ngl::Vec3 &_size)
{
  btCollisionShape* colShape = new btBoxShape(btVector3(_size.m_x, _size.m_y, _size.m_z));

  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.5f);

  m_dynamicsWorld->addRigidBody(rigidBody);
  Body s;
  s.name=_shapeName;
  s.body=rigidBody;
  m_bodies.push_back(s);
}

void Physics::addGroundPlane(const std::string &_name, const ngl::Vec3 &_pos)
{
  m_groundShape.reset(new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y));

  btTransform groundTransform;
  groundTransform.setIdentity();
  {
    btScalar mass(0);
    btVector3 inertia(0,0,0);

    btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, m_groundShape.get(), inertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
    rigidBody->setRestitution(1.0f);
    rigidBody->setRollingFriction(0.1f);

    m_dynamicsWorld->addRigidBody(rigidBody);
    Body s;
    s.name="groundPlane";
    s.body=rigidBody;
    m_bodies.push_back(s);
  }
}

int Physics::getCollisionShape(unsigned int _index) const
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape* collisionShape = obj->getCollisionShape();

  return collisionShape->getShapeType();
}

ngl::Mat4 Physics::getTransformMatrix(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btRigidBody* rigidBody = btRigidBody::upcast(obj);
  if(rigidBody && rigidBody->getMotionState())
  {
    std::cout << "working" << std::endl;
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    float matrix[16];
    transform.getOpenGLMatrix(matrix);

    return ngl::Mat4(matrix[0], matrix[1], matrix[2], matrix[3],
                     matrix[4], matrix[5], matrix[6], matrix[7],
                     matrix[8], matrix[9], matrix[10], matrix[11],
                     matrix[12], matrix[13], matrix[14], matrix[15]);
  }
  else
  {
    std::cout << "NOT working" << std::endl;
    return ngl::Mat4();
  }
}

void Physics::step(float _time, float _step)
{
  m_dynamicsWorld->stepSimulation(_time,_step);
}

void Physics::reset()
{
  for(unsigned int i=1; i<m_bodies.size(); i++)
  {
    m_dynamicsWorld->removeRigidBody(m_bodies[i].body);
  }
  m_bodies.erase(m_bodies.begin()+1,m_bodies.end());
}

bool Physics::isDynamic(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  obj->isKinematicObject();

  return true;
}

bool Physics::isStatic(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  obj->isStaticObject();

  return true;
}
