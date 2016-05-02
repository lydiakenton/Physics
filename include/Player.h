#ifndef PLAYER_H__
#define PLAYER_H__

#include <ngl/Vec3.h>

//! Player class to use within the demo game
class Player
{
public:
  //! Player constructor
  /*!
   * \brief Player
   * \param _pos Position of the player
   */
  Player(ngl::Vec3 _pos);
  //! Initialises the player
  void init();
  //! Sets the left condition for movement
  /*!
   * \brief setLeft
   * \param _left
   */
  void setLeft(bool _left);
  //! Sets the right condition for movement
  /*!
   * \brief setRight
   * \param _right
   */
  void setRight(bool _right);
  //! Updates the player for steady movement across the screen
  void update();
  //! Returns the player position
  ngl::Vec3 getPosition();
  //! Returns the ID number for the player
  int getID();
  //! Returns true and player is alive
  bool isAlive();
  //! Returns false and player is killed
  void kill();

private:
  //! Position (x,y,z)
  ngl::Vec3 m_pos;
  //! Left movement
  bool m_left;
  //! Right movement
  bool m_right;
  //! Speed of movement
  float m_speed;
  //! ID of player
  unsigned int m_id;
  //! Player life
  bool m_alive;
};

#endif // PLAYER_H__
