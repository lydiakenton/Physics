#include "Player.h"
#include "PhysicsLib.h"

Player::Player(ngl::Vec3 _pos)
{
 m_pos = _pos;
}

void Player::update()
{
  //m_pos += ngl::Vec3(0,0,1);
}

void Player::draw()
{
  PhysicsLib *physics = PhysicsLib::instance();
  physics->addCube(m_pos, true, ngl::Vec3(2,2,2));
}
