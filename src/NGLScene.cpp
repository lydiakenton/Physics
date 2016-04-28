#include <QMouseEvent>
#include <QGuiApplication>
#include <QFont>

#include "NGLScene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Quaternion.h>
#include <ngl/Random.h>
#include <stdlib.h>
#include "PhysicsLib.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01f;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=0.1f;

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in that case the GLFrame passed in on construction)
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0.0f;
  m_spinYFace=0.0f;
  setTitle("Simple Physics");
  m_animate=true;
  
  m_width=1024;
  m_height=720;

  m_playerPos.set(0,1,0);
  m_player.push_back( std::unique_ptr<Player>(new Player(m_playerPos)));
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  // now set the camera size values as the screen size has changed
  m_cam.setShape(70.0f,(float)width()/height(),0.5f,350.0f);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_cam.setShape(70.0f,(float)_w/_h,0.5f,350.0f);
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}

void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::instance();
  glClearColor(0.6f, 0.6f, 0.6f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
   // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated that data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  // the shader will use the currently active material and light0 so set them

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,20);
  ngl::Vec3 to = ngl::Vec3::zero();
  ngl::Vec3 up = ngl::Vec3::up();
  // now load to our new camera
  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(70.0f,(float)720.0/576.0f,0.5f,350.0f);
  shader->setUniform("viewerPos",m_cam.getEye().toVec3());

  PhysicsLib::instance()->init();

  // now create our light that is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  ngl::Light light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT );
  light.setTransform(iv);
  // load these values to the shader as well
  light.loadToShader("light");
  // as re-size is not explicitly called we need to do that.
  // set the viewport for openGL we need to take into account retina display

  //set up the text
  m_text = new ngl::Text(QFont ("Helvetica", 12));

  startTimer(10);
  glViewport(0,0,width(),height());

  addStairs();

  updatePlayerPos(0,1,0);
  for(auto &player : m_player)
  {
    loadMatricesToShader();
    player->draw();
  }

  playerCollision();

}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_mouseGlobalTX;
  MV=  m_bodyTransform*M*m_cam.getViewMatrix();
  MVP= m_bodyTransform*M*m_cam.getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();

  shader->setRegisteredUniform("MV",MV);
  shader->setRegisteredUniform("MVP",MVP);
  shader->setRegisteredUniform("normalMatrix",normalMatrix);
  shader->setRegisteredUniform("M",M);
}

void NGLScene::paintGL()
{
  glViewport(0,0,m_width,m_height);
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_text->setScreenSize(m_width,m_height);
  m_text->setTransform(m_x,m_y);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  drawPhysicsShapes();

  m_bodyTransform.identity();
  loadMatricesToShader();

  renderTextToScreen();
}

void NGLScene::addStairs()
{
    PhysicsLib *physics = PhysicsLib::instance();
    for(int pos=20; pos<60; pos++)
    {
      physics->addCube(ngl::Vec3(0, pos, -pos), true, ngl::Vec3(15, 0.5, 2));
    }
}

void NGLScene::drawPhysicsShapes()
{
  PhysicsLib *physics = PhysicsLib::instance();
  for(int i=0; i< physics->getNumOfShapes(); i++)
  {
    m_bodyTransform = physics->getShapeTransformMatrix(i);
    loadMatricesToShader();
    physics->drawShape(i,"material");
  }
  m_bodyTransform.identity();
  loadMatricesToShader();
  physics->drawGroundPlane("material");
}

void NGLScene::addPhysicsShapes()
{
  PhysicsLib *physics = PhysicsLib::instance();

  ngl::Random *rng = ngl::Random::instance();
  float rad = rng->randomPositiveNumber(2);
  float height = rng->randomPositiveNumber(2);
  float width = rng->randomPositiveNumber(2);
  float length = rng->randomPositiveNumber(2);

  physics->addSphere(ngl::Vec3(0, 30, -20), false, rad);

  //used rad twice to make cones uniform. trying to get them to not clip through floor
  physics->addCone(ngl::Vec3(0, 30, -20), false, 1.0, 1.0);

  physics->addCube(ngl::Vec3(0, 30, -20), false, ngl::Vec3(width, height, length));

  //create dynamic capsule
  //if height and rad are set to the same it scales correctly, if they are different values it will draw incorrectly
  physics->addCapsule(ngl::Vec3(0,30, -20), false, rad, rad);
}

void NGLScene::renderTextToScreen()
{
  PhysicsLib *physics = PhysicsLib::instance();
  unsigned int bodies = physics->getNumOfShapes();

  m_text->setColour(ngl::Colour(1,1,1));
  m_text->renderText(10,12,"Use keys 1-5 to create different shapes");
  m_text->renderText(10,12*3,"Press C to clear the shapes");
  m_text->renderText(10,12*5,"Press Esc to exit");
  m_text->renderText(10,12*9, "Transformation Matrix: ");

  QString text2 = QString("Number of bodies: %2").arg(bodies-1);
  m_text->renderText(10,12*7,text2);
  m_bodyTransform = physics->getShapeTransformMatrix(bodies-1);
  QString text;

  for(int j=0; j<4; j++)
  {
    for(int k=0; k<4; k++)
    {
      text.sprintf("[%+0.4f]",m_bodyTransform.m_m[j][k]);
      m_text->renderText(10+(75*j),130+(24*k),text);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // note the method buttons() is the button state when event was called
  // that is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    update();
  }
  // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
   }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  // that method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

	// check the diff of the wheel position (0 means no change)
	if(_event->delta() > 0)
	{
		m_modelPos.m_z+=ZOOM;
	}
	else if(_event->delta() <0 )
	{
		m_modelPos.m_z-=ZOOM;
	}
	update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  PhysicsLib *physics = PhysicsLib::instance();
  ngl::Material mat(ngl::STDMAT::BLACKPLASTIC);

  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wireframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  //clear screen
  case Qt::Key_C : resetSim(); break;

  case Qt::Key_1 : addPhysicsShapes(); break;

  case Qt::Key_Up :
    mat.setDiffuse(ngl::Colour(1.0, 0.5, 0.5, 1.0));
    physics->setMaterial(mat);
    break;
  case Qt::Key_Down : physics->setMaterial(ngl::STDMAT::PEWTER); break;

  case Qt::Key_Left :
    physics->moveLeft(40);
  break;
  case Qt::Key_Right :
    physics->moveRight(40);
  break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  update();
}

void NGLScene::resetSim()
{
  PhysicsLib::instance()->reset();
}

void NGLScene::updatePlayerPos(float _dx, float _dy, float _dz)
{
  for(auto &player : m_player)
  {
    m_playerPos.m_x = _dx;
    m_playerPos.m_y = _dy;
    m_playerPos.m_z = _dz;
    player->setPos(m_playerPos);
  }
}

void NGLScene::playerCollision()
{
  if(PhysicsLib::instance()->collision())
  {
    resetSim();
  }
}

void NGLScene::timerEvent(QTimerEvent *_e)
{
  if(m_animate==true)
    {
      PhysicsLib::instance()->step(1.0/60,10);
    }
  update();
}
