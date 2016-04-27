#include "Player.h"
#include "PhysicsLib.h"

Player::Player(ngl::Vec3 _pos)
{
 m_pos = _pos;
}

void Player::draw()
{
  PhysicsLib *physics = PhysicsLib::instance();
  physics->addCube(m_pos, true, ngl::Vec3(2,2,2));
}
