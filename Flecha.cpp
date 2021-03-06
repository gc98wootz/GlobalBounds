#include "Flecha.h"

Flecha::Flecha(Vector2f pos){
  txt_flecha = new Texture;

  txt_flecha->loadFromFile("Sprites/flecha.png");

  spr_flecha = new Sprite(*txt_flecha);
  //spr_personaje->getPosition().x+32;
  spr_flecha->setPosition(pos);
  spr_flecha->setOrigin(spr_flecha->getTexture()->getSize().x/2.f,spr_flecha->getTexture()->getSize().y/2.f);
  //angulo=0;
}
void Flecha::rotarNegativo(){
  spr_flecha->rotate(-10);
}
void Flecha::rotarPositivo(){
  spr_flecha->rotate(10);
}

void Flecha::setPosition(Vector2f pos) {
  spr_flecha->setPosition(pos);
}

Vector2f Flecha::getPosition() {
  return spr_flecha->getPosition();
}
