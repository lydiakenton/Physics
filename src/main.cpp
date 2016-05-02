/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"

/*! \mainpage
 * \section PhysicsLibrary Physics Library & Demo Game
 *
 * For my physics library, I decided to use the Bullet Detection & Physics Library to handle the physics involved in the falling and colliding of different shapes.
 *
 * This library aims to be a user friendly wrapper around the Bullet Physics Library, giving the user a straight forward way to create shapes with Physics attached to them.
 *
 * I also created a simple game to demonstrate my physics library and I wanted my main focus to remain on the library and the different ways in which I could make it useful for a potential user.
 *
 * \subsection game Game Instructions
 *
 * Use the left and right arrow keys to move the player to avoid oncoming shapes.
 *
 * The game is over and you lose when the player falls off the platform.
 *
 * Press Esc to exit.
 *
 * Press spacebar to pause.
 */

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    // with luck we have the latest GL version so set to that
    format.setMajorVersion(4);
    format.setMinorVersion(5);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  // set that as the default format for all windows
  QSurfaceFormat::setDefaultFormat(format);

  // now we are going to create our scene window
  NGLScene window;

  // we can now query the version to see if it worked
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";

  // and finally show
  window.show();

  return app.exec();
}



