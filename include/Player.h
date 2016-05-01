#ifndef PLAYER_H__
#define PLAYER_H__

#include <ngl/Vec3.h>

class Player
{
public:
  Player(ngl::Vec3 _pos);
  void init();
  void setLeft(bool _left);
  void setRight(bool _right);
  void update();
  ngl::Vec3 getPosition();
  int getID();
  bool isAlive();
  void kill();

private:
  ngl::Vec3 m_pos;
  bool m_left;
  bool m_right;
  float m_speed;
  unsigned int m_id;
  bool m_alive;
};

#endif
