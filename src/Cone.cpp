#include "Cone.h"

Cone::Cone(int _id, ngl::Real _rad, ngl::Real _height, ngl::Material _mat, Physics *_physics, bool _static) :
  Shape(_id, _mat, _physics,_static)
{
  m_rad = _rad;
  m_height = _height;
  m_scaleMat.scale(_rad, _height, _rad);

}

void Cone::draw(const std::string &_shader)
{
  //m_transform = m_physics->getTransformMatrix(m_id);

//  std::cout << "getting matrix: "<<std::endl;
//  for(int j=0; j<4; j++)
//  {
//    for(int k=0; k<4; k++)
//    {
//      std::cout << m_transform.m_m[j][k]<< ", ";
//    }
//    std::cout << std::endl;
//  }
//  std::cout << std::endl;

//  ngl::Mat4 coneRotateMatrix;
//  ngl::Mat4 coneTranslateMatrix;

//  coneTranslateMatrix.translate(0.0f,0.0f,-0.25f);
//  coneRotateMatrix.rotateX(-90);
//  m_transform = coneRotateMatrix * m_transform;
//  m_transform = coneTranslateMatrix * m_transform;

  m_mat.loadToShader(_shader);
  ngl::VAOPrimitives::instance()->draw("cone");
}
