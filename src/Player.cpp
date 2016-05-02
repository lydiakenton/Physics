#include "Player.h"
#include "PhysicsLib.h"

Player::Player(ngl::Vec3 _pos)
{
 m_pos = _pos;
 m_left = false;
 m_right = false;
 m_speed = 4;
 m_alive = true;
}

void Player::init()
{
  // create a cube to act as the player
  PhysicsLib *physics = PhysicsLib::instance();
  m_id = physics->addCube(m_pos, false, ngl::Vec3(2,2,2));
}

void Player::update()
{
  PhysicsLib *physics = PhysicsLib::instance();
  int dir = 0;
  // controls the movement of the player
  if(m_left == true)
  {
    dir -= m_speed;
  }
  if(m_right == true)
  {
    dir += m_speed;
  }
  physics->push(m_id, ngl::Vec3(dir, 0, 0));

  ngl::Mat4 transformation;
  transformation = physics->getShapeTransformMatrix(m_id);
  m_pos = ngl::Vec3(transformation.m_30, transformation.m_31, transformation.m_32);
}

void Player::setLeft(bool _left)
{
  m_left = _left;
}

void Player::setRight(bool _right)
{
  m_right = _right;
}

ngl::Vec3 Player::getPosition()
{
  return m_pos;
}

int Player::getID()
{
  return m_id;
}

bool Player::isAlive()
{
  return m_alive;
}

void Player::kill()
{
  m_alive = false;
}
