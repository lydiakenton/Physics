#ifndef NGLSCENE_H__
#define NGLSCENE_H__

#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <ngl/Obj.h>
#include <btBulletDynamicsCommon.h>
#include <QOpenGLWindow>
#include <QElapsedTimer>
#include <QApplication>
#include <memory>
#include <QTime>
#include "Player.h"

//! This class inherits from the Qt OpenGLWindow and allows use of NGL to draw OpenGL
//! The main glwindow widget for NGL applications all drawing elements are put in this file

class NGLScene : public QOpenGLWindow
{
  Q_OBJECT
  public:
    //! Ctor for our NGL drawing class
    //! @param [in] Parent the parent window to the class
    NGLScene();
    //! Dtor must close down ngl and release OpenGL resources
    ~NGLScene();
    //! The initialize class is called once when the window is created and we have a valid GL context
    void initializeGL();
    //! This is called everytime we want to draw the scene
    void paintGL();
    //! This is called everytime we want to draw the scene
    void resizeGL(QResizeEvent *_event);

    void resizeGL(int _w, int _h);
    //! Steps the animation
    void stepAnimation();
    //! Renders the text to screen dependant on whether the player is alive
    void renderTextToScreen();
    //! Draws the physics shapes
    void drawPhysicsShapes();
    //! Adds the stairs on which the shapes fall down
    /*!
     * \brief addStairs
     * \param _floorHeight Height of the player's platform
     */
    void addStairs(float _floorHeight);
    //! Adds the physics shapes to the game
    void addPhysicsShape();

    void togglePauseSim();

    int getConfigSetting(std::ifstream *_settings);

private:
    float m_x;
    float m_y;
    float m_z;
    //! Used to store the x rotation mouse value
    int m_spinXFace;
    //! Used to store the y rotation mouse value
    int m_spinYFace;
    //! Flag to indicate if the mouse button is pressed when dragging
    bool m_rotate;
    //! Flag to indicate if the Right mouse button is pressed when dragging
    bool m_translate;
    //! The previous x mouse value
    int m_origX;
    //! The previous y mouse value
    int m_origY;
    //! The previous x mouse value for Position changes
    int m_origXPos;
    //! The previous y mouse value for Position changes
    int m_origYPos;
    //! Window width
    int m_width;
    //! Window height
    int m_height;
    //! Small text used for when the player is alive
    ngl::Text *m_text;
    //! Large text used for when the player is killed
    ngl::Text *m_largeText;
    //! Transformation matrix of the body
    ngl::Mat4 m_bodyTransform;
    //! The player
    std::unique_ptr<Player> m_player;
    //! Timer ID for each update
    int m_updateTimerID;
    //! Timer ID for dropping shapes
    int m_shapeDropTimerID;
    //! The rate at which the shapes are dropped
    int m_shapeDropRate;
    //! The initial drop rate value
    int m_initialDropRate;
    //! The minimum drop rate value
    int m_minDropRate; //can't go too low because it'll crash - 128 crashes
    //! The initial level that the player starts on
    int m_level;
    //! Player's floor platform height
    float m_floorHeight;
    //! Used to store the global mouse transforms
    ngl::Mat4 m_mouseGlobalTX;
    //! The Camera
    ngl::Camera m_cam;
    //! The model position for mouse movement
    ngl::Vec3 m_modelPos;
    //! Method to load transform matrices to the shader
    void loadMatricesToShader();
    //! Qt Event called when a key is pressed
    //! @param [in] _event the Qt event to query for size etc
    void keyPressEvent(QKeyEvent *_event);
    //! Qt Event called when a key is released
    void keyReleaseEvent(QKeyEvent *_event);
    //! This method is called every time a mouse is moved
    //! @param _event the Qt Event structure
    void mouseMoveEvent (QMouseEvent * _event );
    //! This method is called everytime the mouse button is pressed
    //! Inherited from QObject and overridden here.
    //! @param _event the Qt Event structure
    void mousePressEvent ( QMouseEvent *_event);
    //! This method is called everytime the mouse button is released
    //! Inherited from QObject and overridden here.
    //! @param _event the Qt Event structure
    void mouseReleaseEvent ( QMouseEvent *_event );
    //! This method is called everytime the mouse wheel is moved
    //! Inherited from QObject and overridden here.
    //! @param _event The Qt Event structure
    void wheelEvent( QWheelEvent *_event);
    //! Animation within the window
    bool m_animate;
    //! Timer event
    void timerEvent(QTimerEvent *);
    //! Time variable
    QTime m_time;
    //! Player's score in seconds
    int m_score;
};

#endif // NGLSCENE_H__
