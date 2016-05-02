#include "Physics.h"
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

int Physics::addSphere(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad)
{
  //creating dynamic rigid body - sphere
  btCollisionShape* colShape = new btSphereShape(btScalar(_rad));

  //create dynamic objects
  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  //motionstate provides interpolation capabilities
  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);
  // giving the sphere restitution and friction
  rigidBodyCI.m_friction = 0.25f;
  rigidBodyCI.m_restitution = 0.75f;

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->applyImpulse(btVector3(0,3,0), btVector3(0,1,0));
  // rolling friction ensures that the spheres do not continue rolling forever
  rigidBody->setRollingFriction(btScalar(0.75f));
  rigidBody->setDamping(0.1,0.1);
  if(_isStatic)
  {
    rigidBody->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
  }
  int index = addRigidBodyToDW(rigidBody);
  return index;
}

int Physics::addCone(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad, ngl::Real _height)
{
  //creating dynamic rigid body - cone
  btCollisionShape* colShape = new btConeShape(btScalar(_rad),btScalar(_height));
  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  startTransform.setRotation(btQuaternion(btVector3(1,0,0),btScalar(ngl::PI/2)));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);
  rigidBodyCI.m_friction=0.2f;

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.3f);
  rigidBody->setDamping(0.1,0.1);
  rigidBody->setRollingFriction(0.25f);

  // if the user would like the shapes to be static, a collision flag is set
  if(_isStatic)
  {
    rigidBody->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
  }

  // adding the rigid body to the dynamics world
  int index = addRigidBodyToDW(rigidBody);
  return index;
}

int Physics::addCube(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Vec3 _size)
{
  //creating dynamic rigid body - cube
  btCollisionShape* colShape = new btBoxShape(btVector3(_size.m_x,_size.m_y,_size.m_z));

  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.5f);
  rigidBody->setDamping(0.1,0.1);
  rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  if(_isStatic)
  {
    rigidBody->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
  }

  int index = addRigidBodyToDW(rigidBody);
  return index;
}

int Physics::addCapsule(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad, ngl::Real _height)
{
  //creating dynamic rigid body - capsule
  btCollisionShape* colShape = new btCapsuleShape(btScalar(_rad),btScalar(_height));

  btTransform startTransform;
  startTransform.setIdentity();

  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //calculates intertia by passing by reference
  btVector3 inertia(0,0,0);
  colShape->calculateLocalInertia(_mass,inertia);

  btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, motionState, colShape, inertia);

  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.5f);
  rigidBody->setDamping(0.1,0.1);
  if(_isStatic)
  {
    rigidBody->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
  }

  int index = addRigidBodyToDW(rigidBody);
  return index;
}

int Physics::addGroundPlane(ngl::Real _yPos)
{
  //creating dynamic rigid body - ground plane
  m_groundShape.reset(new btStaticPlaneShape(btVector3(0,1,0),_yPos));

  btTransform groundTransform;
  groundTransform.setIdentity();

  btScalar mass(0);
  btVector3 inertia(0,0,0);

  btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, m_groundShape.get(), inertia);
  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  rigidBody->setRestitution(0.75f);
  rigidBody->setRollingFriction(0.1f);

  int index = addRigidBodyToDW(rigidBody);
  return index;
}

int Physics::getCollisionShape(unsigned int _index) const
{
  // retrieves the collision shape type
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape* collisionShape = obj->getCollisionShape();

  return collisionShape->getShapeType();
}

void Physics::push(unsigned int _index, ngl::Vec3 _dir)
{
  // applies central impulse in order to move the rigid body
  btCollisionObject* _obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btRigidBody* _rigidBody = btRigidBody::upcast(_obj);
  _rigidBody->activate(true);
  _rigidBody->setRestitution(0.0f);
  _rigidBody->setDamping(0.8f,0.8f);
  _rigidBody->applyCentralImpulse(btVector3(_dir.m_x,_dir.m_y,_dir.m_z));

}

ngl::Mat4 Physics::getTransformMatrix(unsigned int _index)
{
  // gets the transformation matrix
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btRigidBody* rigidBody = btRigidBody::upcast(obj);
  if(rigidBody && rigidBody->getMotionState())
  {
    btTransform transform;
    rigidBody->getMotionState()->getWorldTransform(transform);
    float matrix[16];
    transform.getOpenGLMatrix(matrix);

    // uses european matrix standard
    return ngl::Mat4(matrix[0], matrix[1], matrix[2], matrix[3],
                     matrix[4], matrix[5], matrix[6], matrix[7],
                     matrix[8], matrix[9], matrix[10], matrix[11],
                     matrix[12], matrix[13], matrix[14], matrix[15]);
  }
  else
  {
    return ngl::Mat4();
  }
}

void Physics::step(float _time, float _step)
{
  m_dynamicsWorld->stepSimulation(_time,_step);
}

int Physics::addRigidBodyToDW(btRigidBody* _rigidBody)
{
  // adds the rigid bodies to the dynamics world
  int bodyIndex =  m_dynamicsWorld->getNumCollisionObjects();
  m_dynamicsWorld->addRigidBody(_rigidBody);
  return bodyIndex;
}
