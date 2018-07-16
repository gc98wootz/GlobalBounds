
#include "Proyectil.h"



Proyectil::Proyectil(Vector2f pos,Vector2f vel){
  txt_proyectil = new Texture;

  txt_proyectil->loadFromFile("Sprites/bomba.png");

  spr_proyectil = new Sprite(*txt_proyectil);
  spr_proyectil->setPosition(pos);
  spr_proyectil->setOrigin(spr_proyectil->getTexture()->getSize().x/2.f,spr_proyectil->getTexture()->getSize().y/2.f);

  txt_proyectil2 = new Texture;

  txt_proyectil2->loadFromFile("Sprites/tnt.png");
  spr_proyectil2 = new Sprite(*txt_proyectil2);
  spr_proyectil2->setPosition(pos);
  spr_proyectil2->setOrigin(spr_proyectil2->getTexture()->getSize().x/2.f,spr_proyectil2->getTexture()->getSize().y/2.f);

  velocidad=vel;

  aceleracion.x=0;
  aceleracion.y=9.8;

}
void Proyectil::actualizar(float tiemp){
  tiemp/=10;
  velocidad.x+=aceleracion.x *tiemp;
  velocidad.y+=aceleracion.y *tiemp;
  spr_proyectil->setPosition(spr_proyectil->getPosition().x+velocidad.x*tiemp,spr_proyectil->getPosition().y+velocidad.y*tiemp);
  spr_proyectil->rotate(5);
  // borrar este IF para que solo se dibuje una vez
  /*if(spr_proyectil->getPosition().x> 500 && spr_proyectil-> getPosition().y >400 ){

    velocidad.x=50;
    velocidad.y=-50;
    aceleracion.x=0;
    aceleracion.y=9.8;
    spr_proyectil->setPosition(120,320);
  }*/
}
void Proyectil::actualizar2(float tiemp){
  tiemp/=10;
  velocidad.x+=aceleracion.x *tiemp;
  velocidad.y+=aceleracion.y *tiemp;
  spr_proyectil2->setPosition(spr_proyectil2->getPosition().x+velocidad.x*tiemp,spr_proyectil2->getPosition().y+velocidad.y*tiemp);
  spr_proyectil2->rotate(5);
  // borrar este IF para que solo se dibuje una vez
  /*if(spr_proyectil->getPosition().x> 500 && spr_proyectil-> getPosition().y >400 ){

    velocidad.x=50;
    velocidad.y=-50;
    aceleracion.x=0;
    aceleracion.y=9.8;
    spr_proyectil->setPosition(120,320);
  }*/
}
