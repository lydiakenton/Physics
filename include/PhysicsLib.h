#ifndef PHYSICSLIB_H__
#define PHYSICSLIB_H__

#include <ngl/Singleton.h>

class PhysicsLib : public ngl::Singleton<PhysicsLib>
{
friend class ngl::Singleton<PhysicsLib>;

public :
  void draw();


private :
  PhysicsLib();
  ~PhysicsLib();

};

//PhysicsLib *physics = PhysicsLib::instance();

#endif
