#include "Physics.h"
#include "Shapes.h"
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
  btCollisionShape* colShape = Shapes::instance()->getShape(_shapeName);

  //create dynamic objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar mass(1);

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(mass,inertia);

  //motionstate provides interpolation capabilities
  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, colShape, inertia);
  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  //rigidBody->setLinearVelocity(btVector3(0,1,0));

  rigidBody->setActivationState(DISABLE_DEACTIVATION);

  m_dynamicsWorld->addRigidBody(rigidBody);
  Body sphere;
  sphere.name=_shapeName;
  sphere.body=rigidBody;
  m_bodies.push_back(sphere);
}

void Physics::addGroundPlane(const std::string &_name, const ngl::Vec3 &_pos)
{
  m_groundShape.reset(new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y));

  btTransform groundTransform;
  groundTransform.setIdentity();
  //groundTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  {
    btScalar mass(0);
    btVector3 inertia(0,0,0);

    btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, m_groundShape.get(), inertia);
    btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

    m_dynamicsWorld->addRigidBody(rigidBody);
    Body ground;
    ground.name="groundPlane";
    ground.body=rigidBody;
    m_bodies.push_back(ground);
  }
}

ngl::Vec3 Physics::getPosition(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btRigidBody* rigidBody = btRigidBody::upcast(obj);
  if(rigidBody && rigidBody->getMotionState())
  {
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    return ngl::Vec3(transform.getOrigin().getX(),
                     transform.getOrigin().getY(),
                     transform.getOrigin().getZ());
  }
  else return ngl::Vec3();
}

int Physics::getShape(unsigned int _index) const
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape *shape = obj->getCollisionShape();

  return shape->getShapeType();
}

ngl::Mat4 Physics::getTransformMatrix(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btRigidBody* rigidBody = btRigidBody::upcast(obj);
  std::cout << "getting matrix 1" << std::endl;
  if(rigidBody && rigidBody->getMotionState())
  {
    std::cout << "working" << std::endl;
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    float matrix[16];
    transform.getOpenGLMatrix(matrix);

    for(int i=0; i<16; i++)
    {
      std::cout << matrix[i] << ", ";
    }
    std::cout << std::endl;

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
