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

//! A simple physics library allowing a user to add physics shapes to their program
class PhysicsLib : public ngl::Singleton<PhysicsLib>
{
friend class ngl::Singleton<PhysicsLib>;

public :
  //! Initialises the physics library
  void init();
  //! Sets the gravity
  /*!
   * \brief setGravity
   * \param _x Value in the x axis
   * \param _y Value in the y axis
   * \param _z Value in the z axis
   */
  void setGravity(float _x,float _y,float _z);
  //! Returns a vector containing the shapes
  const std::vector<std::unique_ptr<Shape>>& getShapes(){return m_shapes;}
  //! Adds the ground plane
  /*!
   * \brief addGroundPlane
   * \param _yPos Position in the y axis
   */
  void addGroundPlane(ngl::Real _yPos);
  //! Adds a cube and returns the index
  /*!
   * \brief addCube
   * \param _pos Position in the x, y and z axis
   * \param _static Dynamics of the shape
   * \param _size Size of the cube
   * \return cube index
   */
  int addCube(ngl::Vec3 _pos, bool _static, ngl::Vec3 _size);
  //! Adds a sphere and returns the index
  /*!
   * \brief addSphere
   * \param _pos Position in the x, y and z axis
   * \param _static Dynamics of the shape
   * \param _rad Radius of the sphere
   * \return sphere index
   */
  int addSphere(ngl::Vec3 _pos, bool _static, ngl::Real _rad);
  //! Adds a capsule and returns the index
  /*!
   * \brief addCapsule
   * \param _pos Position in the x, y and z axis
   * \param _static Dynamics of the shape
   * \param _rad Radius of the capsule
   * \param _height Height of the capsule
   * \return capsule index
   */
  int addCapsule(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height);
  //! Adds a cone and returns the index
  /*!
   * \brief addCone
   * \param _pos Position in the x, y and z axis
   * \param _static Dynamics of the shape
   * \param _rad Radius of the cone
   * \param _height Height of the cone
   * \return cone index
   */
  int addCone(ngl::Vec3 _pos, bool _static, ngl::Real _rad, ngl::Real _height);
  //! Returns the transformation matrix at a given shape index
  /*!
   * \brief getShapeTransformMatrix
   * \param _shapeIndex shape index number
   * \return shape transformation matrix
   */
  ngl::Mat4 getShapeTransformMatrix(int _shapeIndex);
  //! Draws the shape at a given shape index
  /*!
   * \brief drawShape
   * \param _shapeIndex shape index number
   */
  void drawShape(int _shapeIndex);
  //! Returns the number of shapes in the shapes vector
  int getNumOfShapes();
  //! Draws the ground plane
  void drawGroundPlane();
  //! Steps the simulation
  /*!
   * \brief step
   * \param _time Time value
   * \param _step Number of steps
   */
  void step(float _time, float _step);
  //! Sets the material
  /*!
   * \brief setMaterial
   * \param _mat Material to be used
   */
  void setMaterial(ngl::Material _mat);
  //! Pushes the shape at a given index in a given direction
  /*!
   * \brief push
   * \param _shapeIndex shape index number
   * \param _dir Direction in the x, y and z axis
   */
  void push(int _shapeIndex, ngl::Vec3 _dir);
  //! Resets the simulation

private :
  //! PhysicsLib constructor
  PhysicsLib();
  //! PhysicsLib destructor
  ~PhysicsLib();
  //! Physics class
  Physics m_physics;
  //! Ground plane
  std::unique_ptr<GroundPlane> m_groundPlane;
  //! Vector to hold the shapes
  std::vector<std::unique_ptr<Shape>> m_shapes;
  //! The current material
  ngl::Material m_currentMat;
};

#endif // PHYSICSLIB_H__
