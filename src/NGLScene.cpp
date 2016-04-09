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
#include "Physics.h"
#include <ngl/Random.h>
#include <stdlib.h>
#include "CollisionShape.h"

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
  m_physics=new Physics();
  m_physics->setGravity(0,-10,0);
  m_physics->addGroundPlane("plane",ngl::Vec3(0,0,0));

  m_width=1024;
  m_height=720;
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_physics;
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  // now set the camera size values as the screen size has changed
  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_cam.setShape(45.0f,(float)_w/_h,0.05f,350.0f);
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}

void NGLScene::addSphere()
{
  ngl::Random *rand=ngl::Random::instance();
  ngl::Real _x;
  _x=rand->randomNumber(10.0f);
  ngl::Vec3 pos = ngl::Vec3(_x,8.0f,0.0f);

  m_physics->addSphere("sphere", pos,false);
}

void NGLScene::addCone()
{
  ngl::Random *rand=ngl::Random::instance();
  ngl::Real _x;
  _x=rand->randomNumber(10.0f);
  ngl::Vec3 pos = ngl::Vec3(_x,8.0f,0.0f);

  m_physics->addCone("cone", pos,false);
}

void NGLScene::addCapsule()
{
  ngl::Random *rand=ngl::Random::instance();
  ngl::Real _x;
  _x=rand->randomNumber(10.0f);
  ngl::Vec3 pos = ngl::Vec3(_x,8.0f,0.0f);

  m_physics->addCapsule("capsule", pos,false);
}

void NGLScene::addCube()
{
  ngl::Random *rand=ngl::Random::instance();
  ngl::Real _x;
  _x=rand->randomNumber(10.0f);
  ngl::Vec3 pos = ngl::Vec3(_x,8.0f,0.0f);

  m_physics->addCube("cube",pos,btScalar(1.0f),false);
}

void NGLScene::addPlatform()
{
  ngl::Random *rand=ngl::Random::instance();
  ngl::Vec3 pos;
  pos=rand->getRandomPoint(6.0f,6.0f,0.0f);
  for(int i=0; i<pos.length(); i++)
  {
    if(pos[i] != pos.m_x)
    {
      pos[i] = abs(pos[i]);
    }
  }
  if(pos.m_y>2)
  {
    m_physics->addPlatform("platform",pos,true);
  }
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

  ngl::Material m(ngl::STDMAT::COPPER);
  m.loadToShader("material");
  //shader->setShaderParam4f("colour",1,1,0,1);

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,0,20);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(45.0f,(float)720.0/576.0f,0.5f,350.0f);
  shader->setUniform("viewerPos",m_cam.getEye().toVec3());

  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  prim->createLineGrid("plane",50.0f,50.0f,40.0f);
  prim->createSphere("sphere",0.1f,40.0f);
  prim->createCone("cone",0.5f,0.5f,20.0f,20.0f);
  prim->createCapsule("capsule",0.4f,0.8f,40.0f);

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

  //add shapes from the shapes class
  shape::CollisionShape *shapes = shape::CollisionShape::instance();
  shapes->addSphere("sphere",0.1f);
  shapes->addCone("cone",0.5f,0.5f);
  shapes->addCube("cube",ngl::Vec3(0.5f,0.5f,0.5f));
  shapes->addPlatform("platform",ngl::Vec3(2.0f/2.0f,0.2f/2.0f,1.0f/2.0f));
  shapes->addCapsule("capsule",0.4f,0.8f);
  //set up the text
  m_text = new ngl::Text(QFont ("Helvetica", 12));
  m_text->setScreenSize(width(),height());

  startTimer(10);
  glViewport(0,0,width(),height());
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
  /*shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);*/
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

  m_text->setScreenSize(width(),height());
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

  //get the VBO instance to draw shapes
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  loadMatricesToShader();

  unsigned int bodies = m_physics->getNumCollisionObjects();
  std::cout << "bodies:" << bodies <<std::endl;
  for(unsigned int i=1; i<bodies; i++)
  {
    m_bodyTransform = m_physics->getTransformMatrix(i);
    loadMatricesToShader();

    std::cout << "getting matrix: "<<std::endl;
    for(int j=0; j<4; j++)
    {
      for(int k=0; k<4; k++)
      {
        std::cout << m_bodyTransform.m_m[j][k]<< ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    if((m_physics->isStatic(i))==1)
    {
      m_bodyTransform.scale(2.0f,0.2f,1.0f);
      loadMatricesToShader();
      prim->draw("cube");
    }
    else
    {
      ngl::Mat4 coneRotateMatrix;
      ngl::Mat4 coneTranslateMatrix;

      switch(m_physics->getCollisionShape(i))
      {
        case SPHERE_SHAPE_PROXYTYPE:
          prim->draw("sphere");
        break;

        case CONE_SHAPE_PROXYTYPE:
          coneTranslateMatrix.translate(0.0f,0.0f,-0.25f);
          coneRotateMatrix.rotateX(-90);
          m_bodyTransform = coneRotateMatrix * m_bodyTransform;
          m_bodyTransform = coneTranslateMatrix * m_bodyTransform;
          loadMatricesToShader();

          prim->draw("cone");
        break;

        case BOX_SHAPE_PROXYTYPE:
          prim->draw("cube");
        break;

        case CAPSULE_SHAPE_PROXYTYPE:
          prim->draw("capsule");
        break;
      }
    }
    std::cout << m_physics->isStatic(i) << std::endl;
  }


  m_bodyTransform.identity();
  loadMatricesToShader();
  prim->draw("plane");

  renderTextToScreen();
}

void NGLScene::renderTextToScreen()
{
  unsigned int bodies = m_physics->getNumCollisionObjects();

  m_text->setColour(ngl::Colour(1,1,1));
  m_text->renderText(10,12,"Use keys 1-5 to create different shapes");
  m_text->renderText(10,12*3,"Press C to clear the screen");
  m_text->renderText(10,12*5,"Press Esc to exit");
  m_text->renderText(10,12*9, "Transformation Matrix: ");

  QString text2 = QString("Number of bodies: %2").arg(bodies-1);
  m_text->renderText(10,12*7,text2);
  m_bodyTransform = m_physics->getTransformMatrix(bodies-1);
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
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // turn on wirframe rendering
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  // turn off wire frame
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  // create sphere
  case Qt::Key_1 : addSphere(); break;
  // create cone
  case Qt::Key_2 : addCone(); break;
  //create dynamic box
  case Qt::Key_3 : addCube(); break;
  //create dynamic capsule
  case Qt::Key_4 : addCapsule(); break;
  //create static platform
  case Qt::Key_5 : addPlatform(); break;
  //delete bodies in the window
  case Qt::Key_C : resetSim(); break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  update();
}

void NGLScene::resetSim()
{
  m_physics->reset();
}

void NGLScene::timerEvent(QTimerEvent *_e)
{
  if(m_animate==true)
    {
      m_physics->step(1.0/60.0,10);
    }
  update();
}

void NGLScene::stepAnimation()
{
  std::cout<<"stepping animation"<<std::endl;
  m_physics->step(1.0/20.0,10);
}
