#include "Bomb.h"

Bomb::Bomb():m_active(true),count(0){

}

void Bomb::update(float dt){
	if((count%2)==0){
	  this->setOpacity(150);
	}else{
	  this->setOpacity(250);
	}
	count++;
}

Bomb::~Bomb(){

}

void Bomb::destroy(){
	this->removeFromParent();
}

bool Bomb::isActive(){
	return m_active;
}

void Bomb::setActivity(bool isactive){
	m_active = isactive;
}

void Bomb::hurt(){

}

CCRect Bomb::collideRect(){
	CCPoint pos = getParent()->getPosition();
	CCSize cs = getContentSize();
	//CCLog("boom width ======================================>%f",cs.height);

	return CCRectMake(pos.x - cs.width/2,pos.y-60,cs.width,cs.height);
}