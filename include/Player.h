#ifndef PLAYER_H__
#define PLAYER_H__

#include <ngl/Vec3.h>

class Player
{
public:
  Player(ngl::Vec3 _pos);
  void setPos(const ngl::Vec3 &_v){m_pos=_v;}
  void update();
  void draw();
private:
  ngl::Vec3 m_pos;
};

#endif
