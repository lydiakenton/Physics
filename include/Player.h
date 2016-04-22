#ifndef PLAYER_H__
#define PLAYER_H__

#include "Shape.h"

class Player
{
public:
  Player(ngl::Vec3 _pos);
  void update();
private:
  ngl::Vec3 m_pos;
};

#endif
