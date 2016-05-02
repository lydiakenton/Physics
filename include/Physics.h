#ifndef PHYSICS_H__
#define PHYSICS_H__

#include <vector>
#include <btBulletDynamicsCommon.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Obj.h>
#include <memory>

//! Physics class to handle Bullet operations
class Physics
{
public:
  //! Physics constructor
  Physics();
  //! Physics destructor
  ~Physics();
  //!Sets the gravity of the world
  /*!
   * \brief setGravity
   * \param _x Value for the x axis
   * \param _y Value for the y axis
   * \param _z Value for the z axis
   */
  void setGravity(float _x, float _y, float _z)
  {
    m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
  }
  //! Adds the ground plane physics to the world and returns the index
  /*!
   * \brief addGroundPlane
   * \param _yPos Position on the y axis
   * \return Body index number
   */
  int addGroundPlane(ngl::Real _yPos);
  //! Creates a bullet sphere and returns the index
  /*!
   * \brief addSphere
   * \param _pos Position along the x, y and z axis
   * \param _mass Mass of the sphere
   * \param _isStatic Dynamics of the sphere
   * \param _rad Radius of the sphere
   * \return Body index number
   */
  int addSphere(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad);
  //! Creates a bullet cone and returns the index
  /*!
   * \brief addCone
   * \param _pos Position alone the x, y and z axis
   * \param _mass Mass of the cone
   * \param _isStatic Dynamics of the cone
   * \param _rad Radius of the cone
   * \param _height Height of the cone
   * \return Body index number
   */
  int addCone(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Real _rad, ngl::Real _height);
  //! Creates a bullet cube and returns the index
  /*!
   * \brief addCube
   * \param _pos Position along the x, y and z axis
   * \param _mass Mass of the cube
   * \param _isStatic Dynamics of the cube
   * \param _size Size of the cube
   * \return Body index number
   */
  int addCube(ngl::Vec3 _pos, ngl::Real _mass, bool _isStatic, ngl::Vec3 _size);
  //! Creates a bullet capsule and returns the index
  /*!
   * \brief addCapsule
   * \param _pos Position alonf the x, y and z axis
   * \param _mass Mass of the capsule
   * \param _isStatic Dyamics of the capsule
   * \param _rad Radius of the capsule
   * \param _height Height of the capsule
   * \return Body index number
   */
  int addCapsule(ngl::Vec3 _pos, ngl::Real _mass,bool _isStatic, ngl::Real _rad, ngl::Real _height);
  //! Steps the time for the simulation
  /*!
   * \brief step
   * \param _time Time value
   * \param _step Number of steps
   */
  void step(float _time, float _step);
  //! Adds the rigid body of each physics shape to the dynamics world
  /*!
   * \brief addRigidBodyToDW
   * \param _rigidBody
   * \return Body index number
   */
  int addRigidBodyToDW(btRigidBody* _rigidBody);
  //! Pushes bullet shapes by applying central impulse in a given direction
  /*!
   * \brief push
   * \param _index Index number of a bullet shape
   * \param _dir Direction in which to push
   */
  void push(unsigned int _index, ngl::Vec3 _dir);
  //! Returns the number of collision objects in the dynamics world
  unsigned int getNumCollisionObjects()const
  {
    return m_dynamicsWorld->getNumCollisionObjects();
  }
  //! Returns the shape type at a given index in the array
  /*!
   * \brief getCollisionShape
   * \param _index Index number of a bullet shape
   * \return Collision shape type
   */
  int getCollisionShape(unsigned int _index) const;
  //! Returns the transformation matrix for a bullet shape at a given index
  /*!
   * \brief getTransformMatrix
   * \param _index Index number of a bullet shape
   * \return Transformation matrix
   */
  ngl::Mat4 getTransformMatrix(unsigned int _index);

private:
  //! Ground plane collision shape
  std::unique_ptr<btCollisionShape> m_groundShape;
  //! Impulse constraint solver
  std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
  //! Broadphase interface
  std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache;
  //! Collision dispatcher
  std::unique_ptr<btCollisionDispatcher> m_dispatcher;
  //! Collision configuration
  std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
  //! Dynamics world
  std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

};


#endif // PHYSICS_H__
